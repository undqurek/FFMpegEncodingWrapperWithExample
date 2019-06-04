#ifndef PACKET_H
#define PACKET_H

#include "preprocesor.h"

extern "C"
{
    #include <libavutil/opt.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/common.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/samplefmt.h>
}


class FFMpegEncoder;

class Packet
{
private:
    AVPacket *_packet;

    friend class FFMpegEncoder;

public:
    Packet( AVPacket *packet );
    ~Packet();

    uint32_t getSize();
    uint8_t *getData();

    bool reset();
};

#endif // PACKET_H
