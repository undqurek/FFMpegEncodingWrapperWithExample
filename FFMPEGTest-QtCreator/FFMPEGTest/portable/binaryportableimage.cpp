#include "binaryportableimage.h"

#include <stdio.h>
#include <string.h>


BinaryPortableImage::BinaryPortableImage( const char *sign )
{
    this->_sign = sign;

    this->_width = 0;
    this->_height = 0;
}

uint16_t BinaryPortableImage::getWidth()
{
    return this->_width;
}

uint16_t BinaryPortableImage::getHeight()
{
    return this->_height;
}

bool BinaryPortableImage::isOpened()
{
    return this->_file.is_open();
}

bool BinaryPortableImage::open( const char *path )
{
    if( this->_file.is_open() )
        return false;

    this->_file.open( path, std::ios::in | std::ios::binary );

    if( this->_file.is_open() )
    {
        std::filebuf *buffer = this->_file.rdbuf();

        if( ! this->checkSign( buffer ) )
        {
            buffer->close(); // buffer->close() zamyka również this->_file

            return false;
        }

        if( ! this->skipComments( buffer ) )
        {
            buffer->close(); // buffer->close() zamyka również this->_file

            return false;
        }

        this->_file >> this->_width;
        this->_file >> this->_height;

        return true;
    }

    return false;
}

void BinaryPortableImage::close()
{
    if( this->_file.is_open() )
        this->_file.close();
}

bool BinaryPortableImage::readPixels( uint8_t *data, uint32_t length )
{
//    if( this->_file.is_open() )
//    {
//        int left = length;

//        for( int offset = 0; offset < length; )
//        {
//            int tmp = this->_file.readsome( (char *)(data + offset), left );

//            if( tmp == -1 )
//                return false;

//            offset += tmp;
//            left -= tmp;
//        }

//        return true;
//    }

//    return false;

    if( this->_file.is_open() )
    {
        this->_file.read( (char *)data, length );

        if( this->_file )
           return true;
    }

    return false;
}

bool BinaryPortableImage::skipPart( std::filebuf *file, char delimiter )
{
    while( true )
    {
        int tmp = file->sbumpc();

        if( tmp == EOF )
            return false;

        if( (char)tmp == delimiter )
            return true;
    }

    return false;
}

bool BinaryPortableImage::checkPart( std::filebuf *file, const char *data, uint32_t length )
{
    for( uint32_t i = 0; i < length; ++i )
    {
        int tmp = file->sbumpc();

        if( tmp == EOF )
            return false;

        if( (char)tmp != data[ i ] )
            return false;
    }

    return true;
}

bool BinaryPortableImage::checkSign( std::filebuf *file )
{
    if( ! this->checkPart( file, this->_sign, 2 ) )
        return false;

    if( ! this->skipPart( file, '\n' ) )
        return false;

    return true;
}

bool BinaryPortableImage::skipComments( std::filebuf *file )
{
    while( true )
    {
        int tmp = file->sgetc();

        if( tmp == EOF )
            return false;

        if( (char)tmp == '#' )
        {
            file->sbumpc();

            if( ! this->skipPart( file, '\n' ) )
                return false;
        }
        else
            return true;
    }
}
