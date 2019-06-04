#ifndef UTILS_H
#define UTILS_H

#include "portable/ppmimage.h"

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class Image
{
private:
public:    uint32_t _component;

    uint16_t _width;
    uint16_t _height;

    Pixel *_pixels;

public:
    Image( PPMImage &image, Pixel *pixels )
    {
        this->_component = image.getComponent();

        this->_width = image.getWidth();
        this->_height = image.getHeight();

        this->_pixels = pixels;
    }

    ~Image()
    {
        if( this->_pixels != NULL )
        {
            delete [] this->_pixels;
            this->_pixels = NULL;
        }
    }

    uint32_t getComponent()
    {
        return this->_component;
    }

    uint16_t getWidth()
    {
        return this->_width;
    }

    uint16_t getHeight()
    {
        return this->_height;
    }

    Pixel *getPixels()
    {
        return this->_pixels;
    }

    static Image *load( const char *path )
    {
        PPMImage image;

        if( image.open( path ) )
        {
            uint32_t dataLength = image.getWidth() * image.getHeight(); // ilosc pikseli
            Pixel *data = new Pixel[ dataLength ];

            if( image.readPixels( (uint8_t *)data, 3 * dataLength ) )
            {
                image.close();

                return new Image( image, data );
            }

            image.close();
        }

        return NULL;
    }
};

#endif // UTILS_H
