#include "RenderWindow.hpp"
#include <iostream>

RenderWindow::RenderWindow(const char* title,int width,int height)
    :ren(NULL),win(NULL)
{
    this->res = {width,height};
    this->win = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
    if(win == NULL) {
        fprintf(stderr,"%s\n",SDL_GetError());
    }
    this->ren = SDL_CreateRenderer(this->win,-1,SDL_RENDERER_ACCELERATED);
    this->O = 0.5f*this->res;
    this->H = {{-1.0f,0.0f},{0.0f,1.0f}};
    this->bgcolor = {0.4f,0.4f,0.4f};
    this->shouldClose = false;
    this->keymn = KeyboardManager("wasd");
}

void RenderWindow::SetColor(linalg::aliases::float3 color) {
    Uint8 rgb[3];
    for(int i = 0; i < 3; i++) {
        color[i] = SDL_clamp(color[i],0.0f,1.0f); // capping the color between 0 and 1
        rgb[i] = Uint8(255*color[i]);
    }
    SDL_SetRenderDrawColor(this->ren,rgb[0],rgb[1],rgb[2],255);
}
void RenderWindow::BeginDraw() {
    SDL_RenderClear(this->ren);
}
void RenderWindow::EndDraw() {
    this->SetColor(this->bgcolor);
    SDL_RenderPresent(this->ren);
}
void RenderWindow::CleanUp() {
    SDL_DestroyRenderer(this->ren);
    SDL_DestroyWindow(this->win);
}

void RenderWindow::HandleEvent(SDL_Event* event) {
    if(event->type == SDL_QUIT) this->shouldClose = true;
    if(event->type == SDL_KEYDOWN ||  event->type == SDL_KEYUP) this->HandleInput(&event->key);
}
SDL_Renderer* RenderWindow::GetRen() {
    return this->ren;
}
linalg::aliases::float2 RenderWindow::TranslatePoint(linalg::aliases::float2 v) {
    return this->O + linalg::mul(this->H,v);
}

bool RenderWindow::ShouldClose() {
    return this->shouldClose;
}

void RenderWindow::DrawLineF(float2 start,float2 end) {
    start = this->TranslatePoint(start);
    end = this->TranslatePoint(end);
    SDL_RenderDrawLineF(this->ren,start.x,start.y,end.x,end.y);
}

void RenderWindow::HandleInput(SDL_KeyboardEvent* event) {
    keycode k = static_cast<keycode>(event->keysym.sym);
    switch(event->type) {
        case SDL_KEYDOWN:
            this->keymn.Press(k);
            break;
        case SDL_KEYUP:
            this->keymn.Release(k);
            break;
    }
}

void RenderWindow::Update() {
    for(unsigned char ch : "wasd") {
        auto k = this->keymn[ch];
        if(k != NULL) {
            // if(*k) std::cout << ch << std::endl;
            //todo add thingies here
        }
    }
}