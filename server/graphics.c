#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>

#include <pthread.h>
#include <string.h>

#include "graphics.h"
#include "ui.h"

#define CMTOPXRATIO         1.5
#define PXBORDER            20
#define COORD_PER_PLAYER    100
#define PX(v)               ((int) (v*CMTOPXRATIO))

#define DELAY_FRAME_RENDER  1000

#define log __mylog

struct coordinate {
    int x;
    int y;
};

/* Init SDL */
int graphicsInit () {
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        SDL_Quit ();

        return -1;
    }

    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    return 0;
}

SDL_Window *window = NULL;
SDL_GLContext openGLContext;
struct coordinate * coordinates;
struct coordinate * newCoordinates[4];
unsigned char teamIndex[15];
int nbPlayers;
pthread_mutex_t lock;
pthread_t tid;

void glLoop () {
    while (window) {
        pthread_mutex_lock (&lock);
        if (!window) break;

        /* Clear buffer */
        glClear (GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /* Draw arena */
        glColor3f (1,1,1);
        if (nbPlayers == 2) {
            glBegin(GL_QUADS);
                glVertex2f(0, 200);
                glVertex2f(0, 0);
                glVertex2f(120, 0);
                glVertex2f(120, 200);
            glEnd();
        } else {
            glBegin(GL_QUADS);
                glVertex2f(-120, 400);
                glVertex2f(-120, 0);
                glVertex2f(120, 0);
                glVertex2f(120, 400);
            glEnd();
        }

        /* TODO: Draw path */

        pthread_mutex_unlock (&lock);

        SDL_GL_SwapWindow (window);

        SDL_Delay (DELAY_FRAME_RENDER);
    }
}

void * graphicsInitWindowAux (void * teams) {
    int i;

    window = SDL_CreateWindow (
            "OS Contest",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            PX(60*nbPlayers) + 2*PXBORDER,
            PX(100*nbPlayers) + 2*PXBORDER,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
            );

    if (!window) {
        return (void *) -1;
    }

    if (pthread_mutex_init(&lock, NULL) != 0) {
        SDL_DestroyWindow (window);
        window = NULL;

        return (void *) -1;
    }

    if (!(openGLContext = SDL_GL_CreateContext (window))) {
        SDL_DestroyWindow (window);
        pthread_mutex_destroy (&lock);
        window = NULL;

        return (void *) -1;
    }

    glewExperimental = GL_TRUE;
    if (glewInit () != GLEW_OK) {
        SDL_GL_DeleteContext (openGLContext);
        SDL_DestroyWindow (window);
        pthread_mutex_destroy (&lock);
        window = NULL;

        return (void *) -1;
    }

    coordinates = (struct coordinate *) malloc (COORD_PER_PLAYER*nbPlayers*sizeof(struct coordinate));
    for (i=0; i < 4; i++) {
        newCoordinates[i] = &coordinates[COORD_PER_PLAYER*i];
    }

    memset (teamIndex, -1, sizeof(teamIndex));
    for (i=0; i<4; i++) {
        if (((int *) teams)[i] >= 0) {
            teamIndex[((int *) teams)[i]] = i;
        }
    }
    free (teams);

    if (nbPlayers == 4) {
        glViewport(PXBORDER, PXBORDER, PX(240), PX(400));
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-120, 120, 0, 400);
    } else {
        glViewport(PXBORDER, PXBORDER, PX(120), PX(200));
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, 120, 0, 200);
    }


    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor (0, 0, 0, 1);

    glLoop ();

    return NULL;
}

/* Init SDL Window */
int graphicsInitWindow (int team1, int team2, int team3, int team4) {
    int * teams = (int *) malloc (4*sizeof(int));
    nbPlayers = (team3 >= 0) ? 4 : 2;
    teams[0] = team1;
    teams[1] = team2;
    teams[2] = team3;
    teams[3] = team4;

    if (pthread_create (&tid, NULL, &graphicsInitWindowAux, teams) != 0) {
        return -1;
    }

    return 0;
}

void addCoordinate (int team, int x, int y) {
    if (!window)
        return;
    if (x > 120 || x < -120 || (x < 0 && nbPlayers == 2) || y < 0 || y > 100 * nbPlayers)
        return;
    if (teamIndex[team] < 0)
        return;

    pthread_mutex_lock (&lock);

    newCoordinates[teamIndex[team]]->x = x;
    newCoordinates[teamIndex[team]]->y = y;
    newCoordinates[teamIndex[team]]++;

    pthread_mutex_unlock (&lock);
}

/* Destroy SDL Window */
void graphicsDestroyWindow () {
    if (window) {
        pthread_mutex_lock (&lock);
        free (coordinates);
        SDL_GL_DeleteContext (openGLContext);
        pthread_mutex_destroy (&lock);
        SDL_DestroyWindow (window);
    }
    window = NULL;
}

/* Quit SDL */
void graphicsQuit () {
    SDL_Quit ();
}
