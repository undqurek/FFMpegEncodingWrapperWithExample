#include "ppmimage.h"

#include <stdio.h>
#include <string.h>


PPMImage::PPMImage()
    : BinaryPortableImage( "P6" )
{
    this->_component = 0;
}

uint32_t PPMImage::getComponent()
{
    return this->_component;
}

bool PPMImage::open( const char *path )
{
    if( this->BinaryPortableImage::open( path ) )
    {
        this->_file >> this->_component;

        if( this->_file.get() == EOF )
        {
            this->_file.close();

            return false;
        }

        return true;
    }

    return false;
}
