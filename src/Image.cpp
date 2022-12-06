#include "Image.hpp"
#include <iostream>

Image::Image() {
    this->NotFound();
}

void Image::NotFound() {
    std::ifstream ifs(NOT_FOUND);
    PNM::Info info;
    std::vector<uint8_t> temp;
    ifs >> PNM::load(temp,info);
    this->width = NOT_FOUND_WIDTH; this->height = NOT_FOUND_HEIGHT;
    for(int i = 0; i < this->width*this->height*3; i+=3) {
        this->data.push_back(Pixel3u(temp[i],temp[i+1],temp[i+2]));
    }
}

Pixel3u Image::operator[](size_t i) const {
    return this->data[i];
}
Pixel3u Image::operator()(size_t x, size_t y) const {
    return this->data[x+this->width*y];
}
Image::Image(const char* path) {
    try
    {
        std::ifstream ifs(path);
        if(!ifs) throw std::exception();
        PNM::Info info;
        std::vector<uint8_t> temp;
        ifs >> PNM::load(temp, info);
        if(!info.valid()) throw std::exception();
        this->width = info.width();
        this->height = info.height();
        for (int i = 0; i < this->width * this->height * 3; i += 3)
        {
            this->data.push_back(Pixel3u(temp[i], temp[i + 1], temp[i + 2]));
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "\"" << path << "\" was not found" << std::endl;
        this->NotFound();
    }
    
}

int Image::Width() const {
    return this->width;
}
int Image::Height() const {
    return this->height;
}
