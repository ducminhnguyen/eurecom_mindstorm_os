#include <SDL2/SDL.h>
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <pthread.h>
#include <string.h>

#include "graphics.h"
#include "ui.h"

#define CMTOPXRATIO         3
#define PXBORDER            20
#define COORD_PER_PLAYER    100
#define PX(v)               ((int) (v*CMTOPXRATIO))

#define DELAY_FRAME_RENDER  1000

#define log     __mylog

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

    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
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

void * glLoop (void * __dummy) {
    while (window) {
        pthread_mutex_lock (&lock);

        glClear (GL_COLOR_BUFFER_BIT);

        pthread_mutex_unlock (&lock);

        SDL_GL_SwapWindow (window);
        SDL_Delay (DELAY_FRAME_RENDER);
    }

    return NULL;
}

/* Init SDL Window */
int graphicsInitWindow (int team1, int team2, int team3, int team4) {
    int i;

    nbPlayers = (team3 >= 0) ? 4 : 2;

    window = SDL_CreateWindow (
            "OS Contest",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            PX(60*nbPlayers) + 2*PXBORDER,
            PX(100*nbPlayers) + 2*PXBORDER,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
            );

    if (!window) {
        return -1;
    }

    if (pthread_mutex_init(&lock, NULL) != 0) {
        SDL_DestroyWindow (window);
        window = NULL;

        return -1;
    }

    if (!(openGLContext = SDL_GL_CreateContext (window))) {
        SDL_DestroyWindow (window);
        pthread_mutex_destroy (&lock);
        window = NULL;
        log (KRED, SDL_GetError());
        log (KRED, "\n");

        return -1;
    }

    pthread_mutex_lock (&lock);

    if (pthread_create (&tid, NULL, &glLoop, NULL) != 0) {
        SDL_GL_DeleteContext (openGLContext);
        SDL_DestroyWindow (window);
        pthread_mutex_destroy (&lock);
        window = NULL;

        return -1;
    }

    coordinates = (struct coordinate *) malloc (COORD_PER_PLAYER*nbPlayers*sizeof(struct coordinate));
    for (i=0; i < 4; i++) {
        newCoordinates[i] = &coordinates[COORD_PER_PLAYER*i];
    }

    memset (teamIndex, -1, sizeof(teamIndex));
    teamIndex[team1] = 0;
    teamIndex[team2] = 1;
    if (team3 >= 0) {
        teamIndex[team3] = 2;
        teamIndex[team4] = 3;
    }

    pthread_mutex_unlock (&lock);

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
