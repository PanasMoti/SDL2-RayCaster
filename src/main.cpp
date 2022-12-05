#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2/SDL.h>
#include "RenderWindow.hpp"
#include "Key.hpp"
#include "Clock.hpp"



int main(int argc,char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    RenderWindow win("title",500,500);
    auto timer = new Clock();
    while(!win.ShouldClose()) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            win.HandleEvent(&event);
        }
        timer->Update();
        win.Update(timer->DeltaTime());
        win.BeginDraw();
        win.SetColor({1.0f,0.0f,0.0f});
        win.DrawLineF({0.0f,0.0f},{100.0f,100.0f});
        win.EndDraw();
    }
    win.CleanUp();
    SDL_Quit();
    return 0;
}