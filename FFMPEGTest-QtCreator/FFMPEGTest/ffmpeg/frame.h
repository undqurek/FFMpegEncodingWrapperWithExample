#ifndef FRAME_H
#define FRAME_H

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

class Frame
{
private:
    AVFrame *_frame;

    friend class FFMpegEncoder;

public:
    Frame( AVFrame *frame );
    ~Frame();

    int getWidth();
    int getHeight();

    // Ustawia czas w ktorym powinna zostac wyswietlona ramka.
    void setTimestamp( int64_t value );

    // Zwraca czas w ktorym powinna zostac wyswietlona ramka.
    int64_t getTimestamp();

    // Zwraca rozmiar pojedynczej lini dla wskazanej skladowj.
    uint32_t getLineSize( uint8_t component );

    // Zwraca dane obrazu dla pojedynczej skladowej.
    uint8_t *getData( uint8_t component );
};

#endif // FRAME_H
