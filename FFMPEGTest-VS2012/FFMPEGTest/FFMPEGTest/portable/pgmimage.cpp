#include "pgmimage.h"

PGMImage::PGMImage()
    : BinaryPortableImage( "P5" )
{
    this->_component = 0;
}

uint32_t PGMImage::getComponent()
{
    return this->_component;
}

bool PGMImage::open( const char *path )
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

void PGMImage::close()
{
    if( this->isOpened() )
    {
        this->_component = 0;

        this->BinaryPortableImage::close();
    }
}
