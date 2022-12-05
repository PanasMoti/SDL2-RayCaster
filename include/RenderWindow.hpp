#ifndef RENDERWINDOW_HPP_
#define RENDERWINDOW_HPP_

#include <SDL2/SDL.h>
#include "imports/linalg.h"
#include "KeyboardManager.hpp"

using namespace linalg::aliases;

class RenderWindow 
{
    public:
        RenderWindow(const char*,int,int);
        void SetColor(float3);
        SDL_Renderer* GetRen();
        void BeginDraw();
        void EndDraw();
        float2 TranslatePoint(float2);
        void CleanUp();
        bool ShouldClose();
        void HandleEvent(SDL_Event*);
        void DrawLineF(float2,float2);
        void HandleInput(SDL_KeyboardEvent*);
        void KeysDown();
        void Update(float deltaTime);
    private:
        SDL_Window* win;
        SDL_Renderer* ren;
        uint2 res;
        float2 O;
        linalg::mat<float,2,2> H;
        float3 bgcolor;
        bool shouldClose;
        KeyboardManager keymn;

};



#endif // RENDERWINDOW_HPP_