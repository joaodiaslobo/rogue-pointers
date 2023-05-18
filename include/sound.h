#include <SDL2/SDL.h>

typedef struct {
    char *filename;
    int time_ms;
    int loop;
} Sound;

void play_sound(const char *filename, int time_ms, int loop);

void *play_sound_thread(void *arg);
