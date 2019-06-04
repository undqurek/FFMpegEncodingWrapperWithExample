#include "frame.h"

Frame::Frame( AVFrame *frame )
{
    this->_frame = frame;
}

Frame::~Frame()
{
    if( this->_frame != NULL )
        ::av_frame_free( &this->_frame );
}

int Frame::getWidth()
{
    if( this->_frame == NULL )
        return 0;

    return this->_frame->width;
}

int Frame::getHeight()
{
    if( this->_frame == NULL )
        return 0;

    return this->_frame->height;
}

void Frame::setTimestamp( int64_t value )
{
    if( this->_frame == NULL )
        return;

    this->_frame->pts = value;
}

int64_t Frame::getTimestamp()
{
    if( this->_frame == NULL )
        return 0;

    return this->_frame->pts;
}

uint32_t Frame::getLineSize( uint8_t component )
{
    if( this->_frame == NULL )
        return 0;

    return this->_frame->linesize[ component ];
}

uint8_t *Frame::getData( uint8_t component )
{
    if( this->_frame == NULL )
        return NULL;

    return this->_frame->data[ component ];
}
