#include <stdio.h>
#include <SDL2/SDL.h>

void play_sound(int sound_type){

    SDL_Init(SDL_INIT_AUDIO); //inicializa o áudio da SDL

    SDL_AudioSpec spec; // uma estrutura com informção do áudio
    Uint8 *audio_buf; // apontador para o buffer de áudio
    Uint32 audio_len; // o tamanho do buffer de áudio carregado na memória

    if(sound_type == 0){
        if (SDL_LoadWAV("assets/sound/door_opening.wav", &spec, &audio_buf, &audio_len) == NULL) { // carrega um ficherio de som .WAV
            printf("Erro ao carregar o ficheiro de som: %s\n", SDL_GetError());
            return 1;
         }
    }

    SDL_AudioDeviceID audio_dev = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0); // abre o dispositivo de áudio e retorna o ID do dispositivo

    SDL_QueueAudio(audio_dev, audio_buf, audio_len); // coloca o buffer de áudio no dispositivo de áudio
    SDL_PauseAudioDevice(audio_dev, 0); // reprodução do áudio

    SDL_Delay(audio_len / spec.freq * 200); // espera o tempo necessário para a reprodução do áudio

    SDL_CloseAudioDevice(audio_dev); // fecha o dispositivo de áudio
    SDL_FreeWAV(audio_buf); // liberta buffer de áudio
    SDL_Quit(); // finaliza SDL
}