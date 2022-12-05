#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include "imports/pnm.hpp"
#include "Pixel3u.hpp"

#include <vector>
#include <fstream>


#define NOT_FOUND "res/notfound.ppm"
#define NOT_FOUND_WIDTH 64
#define NOT_FOUND_HEIGHT 64

class Image
{
    using PixelData = std::vector<Pixel3u>;
    public:
        Image(const char* );
        Image();
        Pixel3u operator()(size_t x,size_t y) const;
        Pixel3u operator[](size_t i) const;
        int Width() const;
        int Height() const;
    protected:
        int width;
        int height;
        PixelData data;
        void NotFound();

};



#endif // IMAGE_HPP_