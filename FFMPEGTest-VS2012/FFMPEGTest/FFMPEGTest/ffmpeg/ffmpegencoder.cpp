#include "ffmpegencoder.h"

#include <iostream>


#define PROTECT_PARAMETER \
    if( this->_opened ) \
    { \
        std::cout << "Set parameter before open operation."; \
        return; \
    }

#define RETURN_ERROR( message, result ) \
    { \
        std::cout << (message) << std::endl; \
        \
        return result; \
    }


bool FFMpegEncoder::_registered = false;

FFMpegEncoder::FFMpegEncoder()
{
    if( ! _registered )
    {
        ::avcodec_register_all();
        _registered = true;
    }

    this->_opened = false;

    this->_codec = NULL;
    this->_context = NULL;

    this->_codecId = AV_CODEC_ID_H264;
    this->_bitRate = 4.0 * ( 1024 * 1024 * 8 );
    this->_width = 1024;
    this->_height = 768;
    this->_framesRate = 25; // ramek na sekunde
    this->_gopSize = 20; // ramka typu intra ma się pojawiać co 20 ramek wewnatrz GOP
    this->_bFramesLimit = 10; // maksymalna ilosc ramek typu B wewnatrz GOP
    this->_pixelFormat = AV_PIX_FMT_YUV420P; // 420 - https://en.wikipedia.org/wiki/Chroma_subsampling
}

FFMpegEncoder::~FFMpegEncoder()
{
    this->close();
}

bool FFMpegEncoder::isOpened()
{
    return this->_opened;
}

bool FFMpegEncoder::open()
{
    if( this->_opened )
        return false;

    this->_codec = ::avcodec_find_encoder( this->_codecId );

    if ( this->_codec == NULL )
        RETURN_ERROR( "Codec not found.", false )

    this->_context = ::avcodec_alloc_context3( this->_codec );

    if ( this->_context == NULL )
        RETURN_ERROR( "Could not allocate video codec context.", false )

	AVRational rational;

	rational.num = 1;
	rational.den = this->_framesRate; // ramek na sekunde

    this->_context->bit_rate = this->_bitRate; // ilosc bitow na sekunde
    this->_context->width = this->_width; // szerokosc obrzzu
    this->_context->height = this->_height; // wysokosc brazu
	this->_context->time_base = rational;
    this->_context->gop_size = this->_gopSize; // jak czesto powinna pojawiac sie ramka typu intra (jedna ramka I wewnatrz GOP)
    this->_context->max_b_frames = this->_bFramesLimit; // maksymalna ilosc ramek typu B wewnatrz GOP
    this->_context->pix_fmt = this->_pixelFormat; // downsapling chrominancji

    if ( ::avcodec_open2( this->_context, this->_codec, NULL ) < 0 ) // inicalizacja kontekstu do pracy z kodekiem
    {
        ::avcodec_free_context( &this->_context );

        RETURN_ERROR( "Could not open codec.", false )
    }

    this->_opened = true;

    return true;
}

void FFMpegEncoder::close()
{
    if( this->_opened )
    {
        ::avcodec_close( this->_context );
        ::avcodec_free_context( &this->_context );

        //TODO: zastanowic sie nad usuwaniem this->_codec
    }
}

Frame *FFMpegEncoder::createFrame( AVPictureType type )
{
    if( this->_opened )
    {
        AVFrame *frame = ::av_frame_alloc();

        if ( frame == NULL )
            RETURN_ERROR( "Could not allocate video frame.", NULL )

        frame->format = this->_pixelFormat;
        frame->width = this->_width;
        frame->height = this->_height;
        frame->pict_type = type; // brak tego parametru spowoduje automatyczne dopasowanie typu ramki

        if( ::av_image_alloc( frame->data, frame->linesize, this->_width, this->_height, this->_pixelFormat, 32 ) < 0 )
        {
            ::av_frame_free( &frame );

            RETURN_ERROR( "Could not allocate raw picture buffer.", NULL )
        }

        return new Frame( frame );
    }

    return NULL;
}

Packet *FFMpegEncoder::createPacket()
{
    if( this->_opened )
    {
        AVPacket *packet = new AVPacket();
        ::av_init_packet( packet );

        packet->data = NULL;
        packet->size = 0;

        return new Packet( packet );
    }

    return NULL;
}

bool FFMpegEncoder::encodeFrame( Frame *frame, Packet *packet, bool &outReady )
{
    if( this->_opened )
    {
        int ready;

        // kodowanie: frame -> packet oraz wydobywanie pakietow z boforu
        if( ::avcodec_encode_video2( this->_context, packet->_packet, frame->_frame, &ready ) < 0 )
            RETURN_ERROR( "Error encoding frame.", false );

        outReady = ( ready == 1 ? true : false );

        return true;
    }

    return false;
}

bool FFMpegEncoder::encodeFrame( Packet *packet, bool &outReady )
{
    if( this->_opened )
    {
        int ready;

        // wydobywanie pozostalych pakietow z buforu jesli takie istnieja
        if( ::avcodec_encode_video2( this->_context, packet->_packet, NULL, &ready ) < 0 )
            RETURN_ERROR( "Error encoding frame.", false );

        outReady = ( ready == 1 ? true : false );

        return true;
    }

    return false;
}

AVCodecID FFMpegEncoder::getCodecId()
{
    return this->_codecId;
}

void FFMpegEncoder::setCodecId( AVCodecID value )
{
    PROTECT_PARAMETER

    this->_codecId = value;
}

int FFMpegEncoder::getHeight()
{
    return this->_height;
}

void FFMpegEncoder::setHeight( int value )
{
    PROTECT_PARAMETER

    this->_height = value;
}

int FFMpegEncoder::getWidth()
{
    return this->_width;
}

void FFMpegEncoder::setWidth( int value )
{
    PROTECT_PARAMETER

    this->_width = value;
}

int64_t FFMpegEncoder::getBitRate()
{
    return this->_bitRate;
}

void FFMpegEncoder::setBitRate( int64_t value )
{
    PROTECT_PARAMETER

    this->_bitRate = value;
}

int FFMpegEncoder::getFramesRate()
{
    return this->_framesRate;
}

void FFMpegEncoder::setFramesRate( int value )
{
    PROTECT_PARAMETER

    this->_framesRate = value;
}

int FFMpegEncoder::getGopSize()
{
    return this->_gopSize;
}

void FFMpegEncoder::setGopSize( int value )
{
    PROTECT_PARAMETER

    this->_gopSize = value;
}

int FFMpegEncoder::getBFramesLimit()
{
    return this->_bFramesLimit;
}

void FFMpegEncoder::setBFramesLimit( int value )
{
    PROTECT_PARAMETER

    this->_bFramesLimit = value;
}

AVPixelFormat FFMpegEncoder::getPixelFormat()
{
    return this->_pixelFormat;
}

void FFMpegEncoder::setPixelFormat( AVPixelFormat value )
{
    PROTECT_PARAMETER

    this->_pixelFormat = value;
}
