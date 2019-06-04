#ifndef PPMIMAGE_H
#define PPMIMAGE_H

#include "binaryportableimage.h"


/*

    PPMImage ppmImage;

    if( ppmImage.open( "image.ppm" ) )
    {
        uint32_t pixelsSize = ppmImage.getWidth() * ppmImage.getHeight();
        Pixel *pixelsData = new Pixel[ pixelsSize ];

        if( ppmImage.readPixels( (uint8_t *)pixelsData, pixelsSize ) )
        {
            for( uint32_t i = 0; i < pixelsSize; ++i )
            {
                std::cout << " R: " << (int)pixelsData[ i ].r
                          << " G: " << (int)pixelsData[ i ].g
                          << " B: " << (int)pixelsData[ i ].b
                          << std::endl;
            }

            std::cout << "PPM ok!" << std::endl;
        }

        ppmImage.close();
    }

*/

class PPMImage : public BinaryPortableImage
{
private:
    uint32_t _component; // ilosc kolorow przypadajacych na jedna skladowa R, G i B jednego piksela

public:
    PPMImage();

    uint32_t getComponent();

    virtual bool open( const char *path );
};

#endif // PPMIMAGE_H
