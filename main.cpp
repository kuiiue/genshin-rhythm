#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;

#include "element.hpp"
#include "data.hpp"
#include "display.hpp"

Texture noteTexture, holdTexture;
Sound music;
Sound chunk;

int arrayPos[3][7][2] = {
    {
        { 420, 131},
        { 494, 156},
        { 566, 170},
        { 639, 174},
        { 712, 170},
        { 785, 156},
        { 858, 130},
    },
    {
        { 116, 486},
        { 291, 548},
        { 466, 581},
        { 640, 593},
        { 814, 581},
        { 989, 548},
        {1163, 486},
    },
    {
        {  82, 526},
        { 268, 591},
        { 454, 626},
        { 640, 639},
        { 824, 626},
        {1010, 591},
        {1196, 525},
    },
};
float angles[7] = {40.57, 27.38, 13.67, 0, -13.67, -27.38, -40.57};

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
    window = SDL_CreateWindow("Genshin Rhythm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    noteTexture.load("assets/note-base.png");
    holdTexture.load("assets/hold-base.png");
    music.load(SOUND_MUSIC, "assets/music.mp3");
    music.play();
    //chunk.load(SOUND_CHUNK, "assets/chunk.wav");

    Texture background("assets/background.png");
    background.setColor(0x80, 0x80, 0x80);
    Texture bottom("assets/bottom-base.png");

    Chart chart;
    //chart.load("assets/example.chart");

    ChartDisplay display(&chart);

    Texture note(noteTexture);
    Anime noteAnime(&note);
    noteAnime.addAnimetion(ANIMATION_MOVE, arrayPos[0][0][0], arrayPos[0][0][1], arrayPos[1][0][0], arrayPos[1][0][1], 0, 2000);
    noteAnime.setLoop(true);
    noteAnime.play();
    Texture hold(holdTexture);
    Anime holdAnime(&hold);
    holdAnime.addAnimetion(ANIMATION_MOVE, arrayPos[0][1][0], arrayPos[0][1][1], arrayPos[1][1][0], arrayPos[1][1][1], 1000, 3000);
    holdAnime.setLoop(true);
    holdAnime.play();

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            display.event(e);
            if (e.type == SDL_QUIT)
                quit = true;
        }
        Uint32 tick = SDL_GetTicks();
        //printf("tick: %I32u\n", tick);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

        background.render();
        bottom.render();
        //noteTexture.renderAt(arrayPos[1][0][0], arrayPos[1][0][1], angles[0]);
        //holdTexture.renderAt(arrayPos[1][1][0], arrayPos[1][1][1], angles[1]);

        holdAnime.drive(tick);
        hold.render();
        noteAnime.drive(tick);
        note.render();

        display.current(tick);

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return 0;
}