#ifndef COLOR_HPP_
#define COLOR_HPP_

#include <cstdint>
#include <ostream>


struct Pixel3u
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Pixel3u(int,int,int);
    Pixel3u();
    Pixel3u(const Pixel3u&);
    Pixel3u(const std::string&);

    Pixel3u operator*(float v) const;
    Pixel3u operator~() const;
    
    void Shade(float shade);
    void Shade(float shadeR,float shadeG,float shadeB);
    void Tint(float tint);
    void Tint(float tintR,float tintG,float tintB);

    uint32_t ToSurfacePixel() const;


    friend std::ostream& operator<<(std::ostream& os, const Pixel3u& pixel);

    private:
        static inline auto clampF = [](float v) {
            if(v > 1.0f) return 1.0f;
            if(v < 0.0f) return 0.0f;
            return v;
        };
        
};




#endif // COLOR_HPP_