#ifndef BINARYPORTABLEIMAGE_H
#define BINARYPORTABLEIMAGE_H

#include <stdint.h>
#include <fstream>


class BinaryPortableImage
{
protected:
    std::fstream _file;

    const char *_sign;

    uint16_t _width;
    uint16_t _height;

protected:
    BinaryPortableImage( const char *sign );

public:
    uint16_t getWidth();
    uint16_t getHeight();

    bool isOpened();

    virtual bool open( const char *path );
    virtual void close();

    bool readPixels( uint8_t *data, uint32_t length );

private:
    bool skipPart( std::filebuf *file, char delimiter );
    bool skipPart( std::filebuf *file, std::filebuf *buffer, char delimiter );
    bool checkPart( std::filebuf *file, const char *data, uint32_t length );

    bool checkSign( std::filebuf *file );
    bool skipComments( std::filebuf *file );
};

#endif // BINARYPORTABLEIMAGE_H
