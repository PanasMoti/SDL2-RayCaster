#ifndef CLOCK_HPP_
#define CLOCK_HPP_

#include <cstdint>
#include <SDL2/SDL.h>


class Clock {
    public:
        Clock(void);
        float DeltaTime() const;
        void Update();
    private:
        uint64_t currentFrame;
        uint64_t lastFrame;
        float deltaTime;
        inline void CalcDeltaTime() {
            this->deltaTime = (float)
            ((this->currentFrame-this->lastFrame)*1000 / (float)SDL_GetPerformanceFrequency());
        }
};


#endif // CLOCK_HPP_