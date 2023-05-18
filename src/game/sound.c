#include <stdio.h>
#include <pthread.h>
#include "sound.h"

void *play_sound_thread(void *arg) {
    Sound *param = (Sound*) arg;
    play_sound(param->filename, param->time_ms, param->loop);
    free(param);
    return NULL;
}

void play_sound(const char *filename, int time_ms, int loop){
    SDL_AudioSpec spec; // uma estrutura com informação do áudio
    Uint8 *audio_buf; // apontador para o buffer de áudio
    Uint32 audio_len; // o tamanho do buffer de áudio carregado na memória

    if (SDL_LoadWAV(filename, &spec, &audio_buf, &audio_len) == NULL) { // carrega um ficherio de som .WAV
        printf("Erro ao carregar o ficheiro de som: %s\n", SDL_GetError());
        return;
    }

    SDL_AudioDeviceID audio_dev = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0); // abre o dispositivo de áudio e retorna o ID do dispositivo
    SDL_QueueAudio(audio_dev, audio_buf, audio_len); // coloca o buffer de áudio no dispositivo de áudio
    SDL_PauseAudioDevice(audio_dev, 0); // reprodução do áudio

    if (loop) {
        while (1) {
            SDL_Delay(time_ms);
            SDL_ClearQueuedAudio(audio_dev);
            SDL_QueueAudio(audio_dev, audio_buf, audio_len);
        }
    } else {
        SDL_Delay(time_ms); // espera o tempo necessário para a reprodução do áudio
    }
    
    SDL_CloseAudioDevice(audio_dev); // fecha o dispositivo de áudio
    SDL_FreeWAV(audio_buf); // liberta buffer de áudio
    //SDL_Quit(); // finaliza SDL 
}