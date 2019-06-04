#include "packet.h"

Packet::Packet( AVPacket *packet )
{
    this->_packet = packet;
}

Packet::~Packet()
{
    if( this->_packet != NULL )
        ::av_packet_free( &this->_packet );
}

uint32_t Packet::getSize()
{
    if( this->_packet == NULL )
        return 0;

    return this->_packet->size;
}

uint8_t *Packet::getData()
{
    if( this->_packet == NULL )
        return 0;

    return this->_packet->data;
}

bool Packet::reset()
{
    if( this->_packet == NULL )
        return false;

    ::av_packet_unref( this->_packet );

    return true;
}
