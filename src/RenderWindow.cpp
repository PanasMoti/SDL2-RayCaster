#include "RenderWindow.hpp"
#include <iostream>



#define FLOOR_INDEX 5
#define CEIL_INDEX 6



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
    this->shouldClose = false;
    this->keymn = KeyboardManager("wasd");
    this->mouse = {0,0};
    this->player = new Player();
    this->textures[0] = Image("res/Brick3.ppm");
    this->textures[1] = Image("res/limestone.ppm");
    this->textures[2] = Image("res/pattern.ppm");
    this->textures[3] = Image("res/Wood1.ppm");
    this->textures[4] = Image("res/pattern5.ppm");
    this->textures[5] = Image("res/floor.ppm"); //? floortiles
    // this->textures[6] = Image("res/ceil.ppm"); //? ceiltiles
    this->surface = SDL_CreateRGBSurface(
        0,this->res.x,this->res.y,32,0,0,0,0
    );
    this->bg = Pixel3u("#012453");
    
}

void RenderWindow::Dark(Uint8 c) {
    SDL_SetSurfaceColorMod(this->surface,c,c,c);
}

void RenderWindow::SetPixel(int x,int y, Uint32 pixel) {
    Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
                                             + y * surface->pitch
                                             + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}
void RenderWindow::SetColor(linalg::aliases::float3 color) {
    Uint8 rgb[3];
    for(int i = 0; i < 3; i++) {
        color[i] = SDL_clamp(color[i],0.0f,1.0f); // capping the color between 0 and 1
        rgb[i] = Uint8(255*color[i]);
    }
    SDL_SetRenderDrawColor(this->ren,rgb[0],rgb[1],rgb[2],255);
}

void RenderWindow::ClearSurfaceBuffer() {
    SDL_LockSurface(this->surface);
    SDL_memset(surface->pixels, 0, surface->h * surface->pitch);
    
    SDL_UnlockSurface(this->surface);
    SDL_FillRect(this->surface,nullptr,this->bg.ToSurfacePixel());
}

void RenderWindow::BeginDraw() {
    SDL_RenderClear(this->ren);
    this->ClearSurfaceBuffer();
    
}
void RenderWindow::EndDraw() {
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
    float moveSpeed = player->speed.x*dt;
    //todo: need to check if there is a wall infront of the player or behind them
    if(*this->keymn['w']) {
        player->pos += player->dir*moveSpeed;
    }
    if(*this->keymn['s']) {
        player->pos -= player->dir*moveSpeed;
    }
    float rotSpeed = player->speed.y*dt;

    //note to self : this needs to be refactored to a rotation matrix mult
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
    SDL_FRect r = {pos.x,pos.y,size.x,size.y};
    SDL_RenderFillRectF(this->ren,&r);
}

void RenderWindow::DrawMouse() {
    //note the mouse position shouldn't be translated to fit the origin
    this->SetColor({1.0f,0.0f,0.0f});
    SDL_Rect r = {this->mouse.x-4,this->mouse.y-4,8,8};
    SDL_RenderFillRect(this->ren,&r);
}


void RenderWindow::VertLine(int x,int y1,int y2,float3 color) {
    this->SetColor(color);
    SDL_RenderDrawLine(this->ren,x,y1,x,y2);
}

void RenderWindow::SetColorP(const Pixel3u& color) {
    SDL_SetRenderDrawColor(this->ren,color.r,color.g,color.b,255);
}

void RenderWindow::DrawSurface() {
    SDL_Texture* tex = SDL_CreateTextureFromSurface(this->ren,this->surface);
    SDL_RenderCopy(this->ren,tex,nullptr,nullptr);
    SDL_DestroyTexture(tex);
}

void RenderWindow::DrawRays() {
    int w = this->res.x;
    int h = this->res.y;
    //* FLOOR CASTING
    for(int y = h/2 + 1; y < h; y++) {
        //? rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
        float2 rayDir0 = player->dir - player->plane;
        float2 rayDir1 = player->dir + player->plane;
        int p = y - screenHeight / 2; //? Current y position compared to the center of the screen (the horizon)
        float posZ = 0.5 * screenHeight;//? Vertical position of the camera.

        //? Horizontal distance from the camera to the floor for the current row.
        //? 0.5 is the z position exactly in the middle between floor and ceiling.
        float rowDistance = posZ / p;

        // calculate the real world step vector we have to add for each x (parallel to camera plane)
        // adding step by step avoids multiplications with a weight in the inner loop
        float2 floorStep = rowDistance* (rayDir1-rayDir0)/screenWidth;
  

        // real world coordinates of the leftmost column. This will be updated as we step to the right.
        float2 floorP = player->pos + rowDistance*rayDir0;
        for(int x = 0; x < screenWidth; x++)
        {
            int texWidth = this->textures[FLOOR_INDEX].Width();
            int texHeight = this->textures[FLOOR_INDEX].Height();
            //? the cell coord is simply got from the integer parts of floorX and floorY
            int cellX = (int)floorP.x;
            int cellY = (int)floorP.y;
            //? get the texture coordinate from the fractional part
            int tx = (int)(texWidth * (floorP.x - cellX)) & (texWidth - 1);
            int ty = (int)(texHeight * (floorP.y - cellY)) & (texHeight - 1);
            floorP += floorStep;
            //? choose texture and draw the pixel
            Pixel3u color;
            color = this->textures[FLOOR_INDEX][texWidth*ty+tx];
            color = color*0.7; //? making the floors darker
            this->SetPixel(x,y,color.ToSurfacePixel());


            // //? ceiling (symmetrical, at screenHeight - y - 1 instead of y)
            // //? recalculating texture sizes for the ceil texture since they maybe of different sizes
            // texWidth = this->textures[CEIL_INDEX].Width();
            // texHeight = this->textures[FLOOR_INDEX].Height();
            // tx = (int)(texWidth * (floorP.x - cellX)) & (texWidth - 1);
            // ty = (int)(texHeight * (floorP.y - cellY)) & (texHeight - 1);
            // color = this->textures[CEIL_INDEX][texWidth*ty+tx];
            // this->SetPixel(x,screenHeight-1-y,color.ToSurfacePixel());
        }
    }



    //* WALL CASTING
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


        int texNum = worldMap[map.x][map.y] - 1;//1 subtracted from it so that texture 0 can be used!
        int texWidth = this->textures[texNum].Width();
        int texHeight = this->textures[texNum].Height();
        float wallX;
        if(side == 0) wallX = player->pos.y + perpWallDist*rayDir.y;
        else          wallX = player->pos.x + perpWallDist*rayDir.x;
        wallX -= floor(wallX);

        int texX = int(wallX*float(texWidth));
        if(side == 0 && rayDir.x > 0) texX = texWidth - texX - 1;
        if(side == 1 && rayDir.y < 0) texX = texWidth - texX - 1;

        float stepT = 1.0f*texHeight/lineHeight;
        float texPos = (drawStart - h / 2 + lineHeight / 2) * stepT;

        //* storing the colors of the pixels to a buffer (surface)
        for(int y = drawStart; y < drawEnd; y++) {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos & (texHeight - 1);
            texPos += stepT;
            float s = (side == 1) ? 0.5f : 1.0f;
            Pixel3u color = this->textures[texNum](texX,texY)*s;
            this->SetPixel(x,y,color.ToSurfacePixel());
        }
    }
    this->DrawSurface(); //? draw all the pixels at one go from the buffer
}