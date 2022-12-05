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
    this->ren = SDL_CreateRenderer(this->win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    this->O = 0.5f*this->res;
    this->H = {{1.0f,0.0f},{0.0f,1.0f}};
    this->bgcolor = {0.4f,0.4f,0.4f};
    this->shouldClose = false;
    this->keymn = KeyboardManager("wasd");
    this->mouse = {0,0};
    this->player = new Player();
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

void RenderWindow::Update(float deltaTime) {
    float dt = 0.001f*deltaTime;
    SDL_GetMouseState(&this->mouse.x,&this->mouse.y);
    for(unsigned char ch : "wasd") {
        auto k = this->keymn[ch];
        if(k != NULL) {
            // if(*k) std::cout << ch << std::endl;
            //todo add thingies here
        }
    }
    if(*this->keymn['w']) {
        player->pos += player->dir*player->speed.x*dt;
    }
    if(*this->keymn['s']) {
        player->pos -= player->dir*player->speed.x*dt;
    }
    float rotSpeed = player->speed.y*dt;
    
    if(*this->keymn['d']) {
        // both camera direction and camera plane must be rotated
        float oldDirX = player->dir.x;
        player->dir.x = player->dir.x * cosf(-rotSpeed) - player->dir.y * sinf(-rotSpeed);
        player->dir.y = oldDirX * sinf(-rotSpeed) + player->dir.y * cosf(-rotSpeed);
        float oldPlaneX = player->plane.x;
        player->plane.x = player->plane.x * cosf(-rotSpeed) - player->plane.y * sinf(-rotSpeed);
        player->plane.y = oldPlaneX * sinf(-rotSpeed) + player->plane.y * cosf(-rotSpeed);
    }
    if(*this->keymn['a']) {
        // both camera direction and camera plane must be rotated
        float oldDirX = player->dir.x;
        player->dir.x = player->dir.x * cosf(rotSpeed) - player->dir.y * sinf(rotSpeed);
        player->dir.y = oldDirX * sinf(rotSpeed) + player->dir.y * cosf(rotSpeed);
        float oldPlaneX = player->plane.x;
        player->plane.x = player->plane.x * cosf(rotSpeed) - player->plane.y * sinf(rotSpeed);
        player->plane.y = oldPlaneX * sinf(rotSpeed) + player->plane.y * cosf(rotSpeed);
    }

    
}

float2 RenderWindow::GetMouse() {
    return 1.0f*this->mouse; //note ! the mouse point doesn't need to be translated to the center of the screen
}


void RenderWindow::TranslateOrigin(float2 o) {
    this->O = o;
}
void RenderWindow::TranslateOrigin(float x,float y) {
    this->O = {x,y};
}
void RenderWindow::ResetOrigin() {
    this->O = 0.5f*this->res; // center of the screen
}


void RenderWindow::DrawPoint(float2 pos,float2 size) {
    pos = this->TranslatePoint(pos);
    SDL_FRect r = {.x = pos.x,.y=pos.y,.w=size.x,.h=size.y};
    SDL_RenderFillRectF(this->ren,&r);
}

void RenderWindow::DrawMouse() {
    //note the mouse position shouldn't be translated to fit the origin
    this->SetColor({1.0f,0.0f,0.0f});
    SDL_Rect r = {.x=this->mouse.x-4,.y=this->mouse.y-4,.w=8,.h=8};
    SDL_RenderFillRect(this->ren,&r);
}


void RenderWindow::VertLine(int x,int y1,int y2,float3 color) {
    this->SetColor(color);
    SDL_RenderDrawLine(this->ren,x,y1,x,y2);
}

void RenderWindow::DrawRays() {
    int w = this->res.x;
    int h = this->res.y;
    for(int x = 0; x < w; x++) {
        float cameraX = 2*x/float(w)-1;
        float2 rayDir = player->dir + player->plane*cameraX;
        int2 map = {int(player->pos.x),int(player->pos.y)};
        float2 sideDist;

        float2 deltaDist = {
            (rayDir.x == 0) ? 1e30 : std::abs(1/rayDir.x),
            (rayDir.y == 0) ? 1e30 : std::abs(1/rayDir.y)
        };
        float perpWallDist;
        //*calculate step and initial sideDist
        int2 step;
        int hit = 0;
        int side;
        if (rayDir.x < 0)
        {
            step.x = -1;
            sideDist.x = (player->pos.x - map.x) * deltaDist.x;
        }
        else
        {
            step.x = 1;
            sideDist.x = (map.x + 1.0f - player->pos.x) * deltaDist.x;
        }
        if (rayDir.y < 0)
        {
            step.y = -1;
            sideDist.y = (player->pos.y - map.y) * deltaDist.y;
        }
        else
        {
            step.y = 1;
            sideDist.y = (map.y + 1.0f - player->pos.y) * deltaDist.y;
        }

        //* DDA
        while (hit == 0)
        {
            //*jump to the next map square, either in x coords or y coords
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                map.x += step.x;
                side = 0;
            }
            else
            {
                sideDist.y += deltaDist.y;
                map.y += step.y;
                side = 1;
            }
            //* check if ray has hit a wall
            if (worldMap[map.x][map.y] > 0)
                hit = 1;
        }
        //*Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
        if(side == 0) perpWallDist = (sideDist.x - deltaDist.x);
        else          perpWallDist = (sideDist.y - deltaDist.y);

        //?Calculate height of line to draw on screen
        int lineHeight = (int)(h / perpWallDist);

        //?calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + h / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + h / 2;
        if (drawEnd >= h)
            drawEnd = h - 1;

        //*choose wall color:
        float3 color;
        switch (worldMap[map.x][map.y])
        {
        case 1:
            color = {1.0f, 0.0f, 0.0f}; //? red
            break;
        case 2:
            color = {0.0f, 1.0f, 0.0f}; //? green
            break;
        case 3:
            color = {0.0f, 0.0f, 1.0f}; //? blue
            break;
        case 4:
            color = {1.0f, 1.0f, 1.0f}; //? white
            break;
        default:
            color = {1.0f, 1.0f, 0.0f}; //? yellow
            break;
        }
        if (side == 1)
        {
            color *= 0.5f; //?shade
        }
        this->VertLine(x,drawStart,drawEnd,color);
    }
}