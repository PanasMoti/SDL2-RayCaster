#include "Pixel3u.hpp"

Pixel3u::Pixel3u() {
    this->r = 0;
    this->g = 0;
    this->b = 0;
}
Pixel3u::Pixel3u(const Pixel3u& cp) {
    *this = cp;
}
Pixel3u::Pixel3u(int r,int g,int b) {
    this->r = uint8_t(r);
    this->g = uint8_t(g);
    this->b = uint8_t(b);
}
Pixel3u Pixel3u::operator*(float v) const {
    v = Pixel3u::clampF(v);
    return Pixel3u(this->r*v,this->g*v,this->b*v);
}
Pixel3u Pixel3u::operator~() const {
    return Pixel3u(255-this->r,255-this->g,255-this->b);
}

std::ostream& operator<<(std::ostream& os, const Pixel3u& pixel) {
    return os << "[" << (int)pixel.r << "," << (int)pixel.g << "," << (int)pixel.b << "]" ;
}

uint32_t Pixel3u::ToSurfacePixel() const {
    return (this->r << 16) | (this->g << 8) | (this->b << 0);
        //? [R] [G] [B]
}

Pixel3u::Pixel3u(const std::string& hex) 
{
    const char* format = (hex.find('#') != std::string::npos)?  "#%02hhx%02hhx%02hhx" : "%02hhx%02hhx%02hhx";
    sscanf(hex.c_str(),format,&this->r,&this->g,&this->b);
}
