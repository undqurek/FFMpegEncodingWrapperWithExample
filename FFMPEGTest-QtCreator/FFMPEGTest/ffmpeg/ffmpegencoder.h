#ifndef FFMPEGENCODER_H
#define FFMPEGENCODER_H

#include "preprocesor.h"

#include "frame.h"
#include "packet.h"

#include <stdint.h>

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


class FFMpegEncoder
{
private:
    static bool _registered;
    bool _opened;

    AVCodec *_codec;
    AVCodecContext *_context;

    AVCodecID _codecId;
    int _width;
    int _height;
    int64_t _bitRate;
    int _framesRate;
    int _gopSize;
    int _bFramesLimit;
    AVPixelFormat _pixelFormat; // AV_PIX_FMT_YUV420P - 420 - https://en.wikipedia.org/wiki/Chroma_subsampling

public:
    FFMpegEncoder();
    ~FFMpegEncoder();

    bool isOpened();

    bool open();
    void close();

    Frame *createFrame( AVPictureType type = AV_PICTURE_TYPE_NONE );
    Packet *createPacket();

    bool encodeFrame( Frame *frame, Packet *packet, bool &outReady );
    bool encodeFrame( Packet *packet, bool &outReady );

    AVCodecID getCodecId();
    void setCodecId( AVCodecID value );

    int getWidth();
    void setWidth( int value );

    int getHeight();
    void setHeight( int value );

    int64_t getBitRate();
    void setBitRate( int64_t value );

    // Zwraca ilosc klatek na sekunde.
    int getFramesRate();

    // Ustawia ilosc klatek na sekunde.
    void setFramesRate( int value );

    int getGopSize();

    // 0 - tylko ramki intra (I)
    void setGopSize( int value );

    int getBFramesLimit();
    void setBFramesLimit( int value );

    AVPixelFormat getPixelFormat();
    void setPixelFormat( AVPixelFormat value );
};

#endif // FFMPEGENCODER_H
