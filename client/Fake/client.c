#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <ncurses.h>

#define MSG_ACK             0
#define MSG_NEXT            1
#define MSG_START           2
#define MSG_STOP            3
#define MSG_CUSTOM          4
#define MSG_KICK            5
#define MSG_POSITION        6
#define MSG_BALL            7

#define INET_PORT   8888

#define LOGLINESIZE    200

#define KNRM            0
#define KRED            1
#define KGRN            2
#define KYEL            3
#define KBLU            4
#define KMAG            5
#define KCYN            6
#define KWHT            7

struct linePart {
    struct linePart *next;
    int color;
    int length;
    char content[LOGLINESIZE];
};

struct outputLine {
    struct outputLine *next;
    struct outputLine *previous;
    struct linePart *firstPart;
    struct linePart *lastPart;
};

struct outputLine *first;
struct outputLine *last;
int nbLines;

#define TOTLINES        100

WINDOW *logwin, *comwin, *infowin;

WINDOW * create_window (int height, int width, int starty, int startx) {
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    wrefresh(local_win);
    return local_win;
}

int const curses_colors [] = {
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE
};

char curses_init () {
    WINDOW *sepwin;
    int i;

    if (initscr() == NULL) {
        fprintf (stderr, "Error initialising ncurses.\n");
        return 0;
    }

    start_color();

    if (LINES < 25 || COLS < 60) {
        endwin ();
        fprintf (stderr, "Window is too small! (%dx%d)\n", LINES, COLS);
        return 0;
    }

    noecho();
    cbreak();

    init_color(COLOR_WHITE, 1000, 1000, 1000);

    for (i=0; i<8; i++)
        init_pair (i+1, curses_colors[i], COLOR_WHITE);
    init_pair(10, COLOR_WHITE, COLOR_BLACK);

    logwin = create_window (LINES-1, COLS-45, 0, 0);
    comwin = create_window (LINES-1, 44, 0, COLS-44);
    infowin = create_window(1, COLS, LINES-1, 0);

    sepwin = create_window(LINES-1, 1, 0, COLS-45);
    wbkgd(sepwin,COLOR_PAIR(10));
    for (i=0; i<LINES-1; i++)
        mvwprintw (sepwin, i, 0, "|");
    wrefresh(sepwin);

    first = (struct outputLine *) malloc (sizeof (struct outputLine));
    last = first;
    first->firstPart = NULL;
    first->lastPart = NULL;
    nbLines = 1;

    keypad(comwin, TRUE);

    wbkgd(logwin,COLOR_PAIR(1));
    wrefresh(logwin);

    return 1;
}

void printLine (int color, const char *content, char newLine) {
    size_t l = strlen (content);
    struct linePart *part = (struct linePart *) malloc (sizeof (struct linePart));
    part->next = NULL;
    part->length = l;
    part->color = color;
    strncpy (part->content, content, LOGLINESIZE-1);

    if (first->firstPart == NULL) {
        first->firstPart = part;
        first->lastPart = part;
    } else {
        first->lastPart->next = part;
        first->lastPart = part;
    }

    if (newLine) {
        struct outputLine *prevFirst = first;
        first = (struct outputLine *) malloc (sizeof (struct outputLine));
        first->next = prevFirst;
        prevFirst->previous = first;
        first->firstPart = NULL;
        first->lastPart = NULL;
        if (++nbLines > TOTLINES) {
            struct outputLine *prevLast = last;
            struct linePart *part, *nextPart;

            last = last->previous;

            for (part = prevLast->firstPart; part != NULL; part = nextPart) {
                nextPart = part->next;
                free (part);
            }

            free (prevLast);
            nbLines --;
        }
    }
}

void curses_log (int color, const char *fmt, ...) {
    char *p1, *p2;
    char buffer[LOGLINESIZE];
    int i;
    struct outputLine *l;

    va_list argp;

    va_start (argp, fmt);

    vsnprintf (buffer, LOGLINESIZE, fmt, argp);

    for (p1 = buffer; (p2 = strchr (p1, '\n')) != NULL; p1 = p2+1) {
        *p2 = '\0';
        printLine (curses_colors[color], p1, 1);
    }
    printLine (curses_colors[color], p1, 0);

    werase (logwin);
    for (i=LINES-2, l=first; i>=0 && i>=LINES-1-TOTLINES && i>=LINES-1-nbLines; i--, l=l->next) {
        /* FIXME: what happens if it overflows? */
        struct linePart *part;
        wmove (logwin, i, 1);
        for (part = l->firstPart; part != NULL; part = part->next) {
            wattron (logwin, COLOR_PAIR(1+part->color));
            wprintw (logwin, "%s", part->content);
        }
    }
    wrefresh(logwin);

    va_end (argp);
}

unsigned char ally = 0;
unsigned char side = 0;
unsigned char hasStarted = 0;

int s;
int teamID;

uint16_t msgId = 0;
uint16_t servMsgId = 0;

int read_from_server (int sock, char *buffer, size_t maxSize) {
    int bytes_read = read (sock, buffer, maxSize);

    if (bytes_read <= 0) {
        curses_log (KRED, "Server unexpectedly closed connection...\n");
        close (s);
        endwin ();
        exit (EXIT_FAILURE);
    }

    return bytes_read;
}

uint16_t lastMsgID = 0;
unsigned char lastSender = 0;
char isLeader = 0;

void monitor_com ();

void parseMessage (const unsigned char *buf, int nbbytes) {
    uint16_t id;

    if (buf[3] != teamID) {
        curses_log (KRED, "*** This message is not for me (%d) ***\n", buf[3]);
        return;
    }

    id = *((uint16_t *) buf);
    lastMsgID = id;

    curses_log (KNRM, "[id: %4d, from: %3d]  ", (unsigned char) id, buf[2]);
    if (buf[2] != 255)
        lastSender = buf[2];

    switch (buf[4]) {
        case MSG_ACK:
            {
                /* ACK */
                uint16_t idAck;

                idAck = *((uint16_t *) &buf[5]);

                curses_log (KNRM, "ACK      idAck=%d status=%d\n", idAck, buf[7]);

                break;
            }
        case MSG_NEXT:
            {
                /* NEXT */
                curses_log (KNRM, "NEXT\n");

                isLeader = 1;

                wbkgd(infowin,COLOR_PAIR(10));
                mvwprintw (infowin, 0, 1, "ally: %2d   side: %s                                %s", ally, (side ? "LEFT " : "RIGHT"), (isLeader ? "LEAD" : ""));
                wrefresh (infowin);

                break;
            }
        case MSG_START:
            {
                /* START */
                isLeader = !buf[5];
                side = buf[6];
                ally = buf[7];

                curses_log (KRED, "START    role=%s   side=%s   ally=%d\n", (isLeader ? "BEGINNER" : "FINISHER"), (side ? "LEFT " : "RIGHT"), ally);

                hasStarted = 1;

                wbkgd(infowin,COLOR_PAIR(10));
                mvwprintw (infowin, 0, 1, "ally: %2d   side: %s                                %s", ally, (side ? "LEFT " : "RIGHT"), (isLeader ? "LEAD" : ""));
                wrefresh (infowin);

                monitor_com ();

                return;
            }
        case MSG_STOP:
            {
                /* STOP */
                curses_log (KRED, "STOP\n");

                close(s);
                endwin ();
                exit (0);

                return;
            }
        case MSG_CUSTOM:
            {
                /* CUSTOM */
                int i;

                curses_log (KNRM, "CUSTOM   content=");
                for (i=5; i<nbbytes; i++) {
                    curses_log (KNRM, "%02X", buf[i]);
                    if ((i-5) % 4 == 3)
                        curses_log (KNRM, " ");
                }
                curses_log (KNRM, "\n");

                break;
            }
        case MSG_KICK:
            {
                /* KICK */
                curses_log (KRED, "KICK     id=%d\n", buf[5]);

                if (buf[5] == teamID) {
                    close(s);
                    endwin ();
                    exit (0);
                } else if (buf[5] == ally) {
                    isLeader = 1;

                    wbkgd(infowin,COLOR_PAIR(10));
                    mvwprintw (infowin, 0, 1, "ally: %2d   side: %s                                %s", ally, (side ? "LEFT " : "RIGHT"), (isLeader ? "LEAD" : ""));
                    wrefresh (infowin);
                }

                return;
            }
        case MSG_BALL:
            {
                /* BALL */
                int16_t x, y;

                x = *((int16_t *) &buf[6]);
                y = *((int16_t *) &buf[8]);

                curses_log (KRED, "BALL     %s x=%d y=%d\n", buf[5] ? "PICK" : "DROP", x, y);

                break;
            }
        default:
            {
                curses_log (KRED, "*** unkown message type 0x%02X ***\n", buf[4]);
                return;
            }
    }
}

pthread_t tid;
char actionSelected;
int rankSelect;

#define MAXPARAM        5

unsigned int fields[MAXPARAM];

struct {
    char *name;
    struct {
        char *name;
        int size;
    } params [MAXPARAM];
} protocol [] = {
    {"ACK", {
                {"dst", 1},
                {"ID ack", 2},
                {"state", 1},
                {NULL, 0},
                {NULL, 0}
            }
    },
    {"NEXT", {
                 {"dst", 1},
                 {NULL, 0},
                 {NULL, 0},
                 {NULL, 0},
                 {NULL, 0}
             }
    },
    {"START", {
                  {"dst", 1},
                  {"role", 1},
                  {"side", 1},
                  {"ally", 1},
                  {NULL, 0}
              }
    },
    {"STOP", {
                 {"dst", 1},
                 {NULL, 0},
                 {NULL, 0},
                 {NULL, 0},
                 {NULL, 0}
             }
    },
    {NULL, {
                 {NULL, 0},
                 {NULL, 0},
                 {NULL, 0},
                 {NULL, 0},
                 {NULL, 0}
             }
    },
    {"KICK", {
                 {"dst", 1},
                 {"id", 1},
                 {NULL, 0},
                 {NULL, 0},
                 {NULL, 0}
             }
    },
    {"POSITION", {
                   {"dst", 1},
                   {"x", 2},
                   {"y", 2},
                   {NULL, 0},
                   {NULL, 0}
               }
    },
    {"BALL", {
                   {"dst", 1},
                   {"act", 1},
                   {"x", 2},
                   {"y", 2},
                   {NULL, 0}
               }
    }
};

void monitor_com () {
    int i;
    wbkgd(comwin,COLOR_PAIR(1));
    for (i=1; i<=9; i++)
        mvwprintw (comwin, i,  1, "                                            ");

    curs_set(0);

    if (hasStarted) {
        if (actionSelected == -1) {
            rankSelect = 0;
            mvwprintw (comwin, 1,  1, "Send message:");
            for (i=0; i<8; i++)
                if (protocol[i].name != NULL)
                    mvwprintw (comwin, 2+i,  1, "     %d: %s", i, protocol[i].name);
        } else {
            char dummy[6];
            mvwprintw (comwin, 1,  1, "%s", protocol[actionSelected].name);
            for (i=0; protocol[actionSelected].params[i].name != NULL; i++)
                mvwprintw (comwin, 2+i,  1, "     %10s: %d", protocol[actionSelected].params[i].name, fields[i]);

            if (i != 0) {
                curs_set(1);
                if (rankSelect < 0)
                    rankSelect += i;
                else if (rankSelect >= i)
                    rankSelect -= i;

                sprintf (dummy, "%d", fields[rankSelect]);
                wmove (comwin, rankSelect+2, 18+strlen(dummy));
            }
        }
    }

    wrefresh (comwin);

}

void sendMessage () {
    char string[58];
    int index = 5;
    int i;

    if (actionSelected == MSG_POSITION) {
        *((uint16_t *) string) = servMsgId++;
    } else {
        *((uint16_t *) string) = msgId++;
    }
    string[2] = teamID;
    string[3] = fields[0];
    string[4] = actionSelected;

    for (i=1; protocol[actionSelected].params[i].name != NULL; i++) {
        if (protocol[actionSelected].params[i].size == 2)
            *((uint16_t *) (string+index)) = fields[i];
        else
            string[index] = fields[i];

        index += protocol[actionSelected].params[i].size;
    }

    write(s, string, index);
}

void * userMonitor (void * __dummy) {
    int i;

    rankSelect = 0;
    actionSelected = -1;

    for (;;) {
        int key = wgetch (comwin);

        if (key == 'q') {
            if (actionSelected == -1) {
                close(s);
                endwin ();
                exit (0);
            } else
                actionSelected = -1;
        }

        else if (key == KEY_DOWN) {
            if (actionSelected != -1)
                rankSelect ++;
        } else if (key == KEY_UP) {
            if (actionSelected != -1)
                rankSelect --;
        }

        else if (key == '\n') {
            if (actionSelected != -1) {
                sendMessage ();
                actionSelected = -1;
            }
        }

        else if (key == KEY_BACKSPACE) {
            if (actionSelected != -1)
                fields[rankSelect] /= 10;
        }

        else if (key < '0' || key > '9')
            continue;

        else {

            key -= '0';

            if (actionSelected == -1) {
                if (key < 8 && key != MSG_CUSTOM) {
                    actionSelected = key;
                    fields[0] = ally;
                    for (i=1; i<MAXPARAM; i++)
                        fields[i] = 0;
                    if (key == MSG_ACK) {
                        fields[1] = lastMsgID;
                        fields[0] = lastSender;
                    } else if (key == MSG_POSITION) {
                        fields[0] = 0xFF;
                    }
                }
            } else {
                int r = fields[rankSelect]*10 + key;
                if (r < 1 << (8*protocol[actionSelected].params[rankSelect].size))
                    fields[rankSelect] = r;
            }
        }

        monitor_com ();
    }
}

void usage (const char * execname) {
    fprintf (stderr, "Usage: %s <IP addr> <Team ID>\n", execname);
    exit (EXIT_FAILURE);
}

int main(int argc, char **argv) {
    struct sockaddr_in addr = { 0 };
    int status;

    if (argc < 3)
        usage (argv[0]);

    teamID = atoi (argv[2]);
    if (teamID > 254 || teamID <= 0)
        usage (argv[0]);

    /* allocate a socket */
    s = socket(AF_INET, SOCK_STREAM, 0);

    /* set the connection parameters (who to connect to) */
    addr.sin_family = AF_INET;
    addr.sin_port = htons (INET_PORT);
    /* FIXME: allow for IPv6 */
    if (inet_aton(argv[1], &addr.sin_addr) == 0) {
        fprintf (stderr, "Invalid IP address\n");
        exit (EXIT_FAILURE);
    }

    /* connect to server */
    status = connect(s, (struct sockaddr *) &addr, sizeof(addr));

    /* if connected */
    if( status == 0 ) {
        char string[58];
        int nbbytes;

        curses_init ();

        if (pthread_create (&tid, NULL, &userMonitor, NULL) != 0) {
            close(s);
            endwin ();
            fprintf (stderr, "Failed to create thread!\n");
            exit (EXIT_FAILURE);
        }

        for (;;) {
            nbbytes = read_from_server (s, string, 20);
            parseMessage ((unsigned char *) string, nbbytes);
        }

    } else {
        fprintf (stderr, "Failed to connect to server...\n");
        exit (EXIT_FAILURE);
    }

    close(s);
    endwin ();

    return 0;
}
