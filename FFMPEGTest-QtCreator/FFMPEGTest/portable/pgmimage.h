#ifndef PGMIMAGE_H
#define PGMIMAGE_H

#include "binaryportableimage.h"


/*

    PGMImage pgmImage;

    if( pgmImage.open( "image.pgm" ) )
    {
        uint32_t pixelsSize = pgmImage.getWidth() * pgmImage.getHeight();
        uint8_t *pixelsData = new uint8_t[ pixelsSize ];

        if( pgmImage.readPixels( pixelsData, pixelsSize ) )
        {
            for( uint32_t i = 0; i < pixelsSize; ++i )
            {
                std::cout << (int)pixelsData[ i ]
                          << std::endl;
            }

            std::cout << "PGM ok!" << std::endl;
        }

        //TODO: pixelsData

        pgmImage.close();
    }

*/

class PGMImage : public BinaryPortableImage
{
private:
    uint32_t _component; // ilosc poziomow szarosci przypadajaca na jeden piksel

public:
    PGMImage();

    uint32_t getComponent();

    virtual bool open( const char *path );
    virtual void close();
};

#endif // PGMIMAGE_H
