#include <stdint.h>
#include <iostream>
#include <math.h>

#define UINT64_C uint64_t

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

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

#include <stdlib.h>

#include "utils.h"

#include "portable/pgmimage.h"
#include "portable/ppmimage.h"
#include "ffmpeg/ffmpegencoder.h"


#define Y_INDEX 0
#define Cb_INDEX 1
#define Cr_INDEX 2


void copy420Image( Image *image, Frame *frame )
{
    int frameWidth = frame->getWidth();
    int frameHeight = frame->getHeight();

    Pixel *pixels = image->getPixels();

    uint32_t yLineSize = frame->getLineSize( Y_INDEX );
    uint32_t cbLineSize = frame->getLineSize( Cb_INDEX );
    uint32_t crLineSize = frame->getLineSize( Cr_INDEX );

    uint8_t *yData = frame->getData( Y_INDEX );
    uint8_t *cbData = frame->getData( Cb_INDEX );
    uint8_t *crData = frame->getData( Cr_INDEX );

//    http://www.ece.ucdavis.edu/cerl/reliablejpeg/compression/
//    Color Space:
//    uint8_t yComponent = ( 0.299  * pixel.r + 0.587  * pixel.g + 0.114  * pixel.b);
//    uint8_t uComponent = (-0.1687 * pixel.r - 0.3313 * pixel.g + 0.5    * pixel.b + 128);
//    uint8_t vComponent = ( 0.5    * pixel.r - 0.4187 * pixel.g - 0.0813 * pixel.b + 128);

    // Y
    for (int y = 0; y < frameHeight; y++)
    {
        for (int x = 0; x < frameWidth; x++)
        {
            Pixel pixel = pixels[ y * image->getWidth() + x ];
            uint8_t yComponent = ( 0.299  * pixel.r + 0.587  * pixel.g + 0.114  * pixel.b);

            yData[ y * yLineSize + x ] = yComponent;
        }
    }

    // Cb and Cr
    // "Naiwny" downsampling chrominancji
    for (int y = 0; y < frameHeight / 2; y++) // zgodnie z 4:2:0 https://en.wikipedia.org/wiki/Chroma_subsampling#Sampling_systems_and_ratios
    {
        for (int x = 0; x < frameWidth / 2; x++) // zgodnie z 4:2:0 https://en.wikipedia.org/wiki/Chroma_subsampling#Sampling_systems_and_ratios
        {
            Pixel pixel = pixels[ 2 * y * image->getWidth() + 2 * x ];

            uint8_t uComponent = (-0.1687 * pixel.r - 0.3313 * pixel.g + 0.5    * pixel.b + 128);
            uint8_t vComponent = ( 0.5    * pixel.r - 0.4187 * pixel.g - 0.0813 * pixel.b + 128);

            cbData[ y * cbLineSize + x ] = uComponent;
            crData[ y * crLineSize + x ] = vComponent;
        }
    }
}

void copy422Image( Image *image, Frame *frame )
{
    int frameWidth = frame->getWidth();
    int frameHeight = frame->getHeight();

    Pixel *pixels = image->getPixels();

    uint32_t yLineSize = frame->getLineSize( Y_INDEX );
    uint32_t cbLineSize = frame->getLineSize( Cb_INDEX );
    uint32_t crLineSize = frame->getLineSize( Cr_INDEX );

    uint8_t *yData = frame->getData( Y_INDEX );
    uint8_t *cbData = frame->getData( Cb_INDEX );
    uint8_t *crData = frame->getData( Cr_INDEX );

//    http://www.ece.ucdavis.edu/cerl/reliablejpeg/compression/
//    Color Space:
//    uint8_t yComponent = ( 0.299  * pixel.r + 0.587  * pixel.g + 0.114  * pixel.b);
//    uint8_t uComponent = (-0.1687 * pixel.r - 0.3313 * pixel.g + 0.5    * pixel.b + 128);
//    uint8_t vComponent = ( 0.5    * pixel.r - 0.4187 * pixel.g - 0.0813 * pixel.b + 128);

    // Y
    for (int y = 0; y < frameHeight; y++)
    {
        for (int x = 0; x < frameWidth; x++)
        {
            Pixel pixel = pixels[ y * image->getWidth() + x ];
            uint8_t yComponent = ( 0.299  * pixel.r + 0.587  * pixel.g + 0.114  * pixel.b);

            yData[ y * yLineSize + x ] = yComponent;
        }
    }

    // Cb and Cr
    // "Naiwny" downsampling chrominancji
    for (int y = 0; y < frameHeight; y++) // zgodnie z 4:2:2 https://en.wikipedia.org/wiki/Chroma_subsampling#Sampling_systems_and_ratios
    {
        for (int x = 0; x < frameWidth / 2; x++) // zgodnie z 4:2:2 https://en.wikipedia.org/wiki/Chroma_subsampling#Sampling_systems_and_ratios
        {
            Pixel pixel = pixels[ y * image->getWidth() + 2 * x ];

            uint8_t uComponent = (-0.1687 * pixel.r - 0.3313 * pixel.g + 0.5    * pixel.b + 128);
            uint8_t vComponent = ( 0.5    * pixel.r - 0.4187 * pixel.g - 0.0813 * pixel.b + 128);

            cbData[ y * cbLineSize + x ] = uComponent;
            crData[ y * crLineSize + x ] = vComponent;
        }
    }
}

//TODO: sprawdzic
void copy444Image( Image *image, Frame *frame )
{
    int frameWidth = frame->getWidth();
    int frameHeight = frame->getHeight();

    Pixel *pixels = image->getPixels();

    uint32_t yLineSize = frame->getLineSize( Y_INDEX );
    uint32_t cbLineSize = frame->getLineSize( Cb_INDEX );
    uint32_t crLineSize = frame->getLineSize( Cr_INDEX );

    uint8_t *yData = frame->getData( Y_INDEX );
    uint8_t *cbData = frame->getData( Cb_INDEX );
    uint8_t *crData = frame->getData( Cr_INDEX );

//    http://www.ece.ucdavis.edu/cerl/reliablejpeg/compression/
//    Color Space:
//    uint8_t yComponent = ( 0.299  * pixel.r + 0.587  * pixel.g + 0.114  * pixel.b);
//    uint8_t uComponent = (-0.1687 * pixel.r - 0.3313 * pixel.g + 0.5    * pixel.b + 128);
//    uint8_t vComponent = ( 0.5    * pixel.r - 0.4187 * pixel.g - 0.0813 * pixel.b + 128);

    // Y, Cb and Cr
    for (int y = 0; y < frameHeight; y++) // zgodnie z 4:4:4 https://en.wikipedia.org/wiki/Chroma_subsampling#Sampling_systems_and_ratios
    {
        for (int x = 0; x < frameWidth; x++) // zgodnie z 4:4:4 https://en.wikipedia.org/wiki/Chroma_subsampling#Sampling_systems_and_ratios
        {
            Pixel pixel = pixels[ y * image->getWidth() + x ];
            
			uint8_t yComponent = ( 0.299  * pixel.r + 0.587  * pixel.g + 0.114  * pixel.b);
			uint8_t uComponent = (-0.1687 * pixel.r - 0.3313 * pixel.g + 0.5    * pixel.b + 128);
            uint8_t vComponent = ( 0.5    * pixel.r - 0.4187 * pixel.g - 0.0813 * pixel.b + 128);

            yData[ y * yLineSize + x ] = yComponent;
			cbData[ y * cbLineSize + x ] = uComponent;
            crData[ y * crLineSize + x ] = vComponent;
        }
    }
}

/*

Przed uruchomieniem nalezy skopiowac zawartosc katalogu bin/ do katalogu Debug/ projektu.

*/
int main( int argc, char **argv )
{
    FFMpegEncoder encoder;

    encoder.setCodecId( AV_CODEC_ID_H264 ); // AV_CODEC_ID_H263P    AV_CODEC_ID_MPEG4
    encoder.setWidth( 1024 ); // mozna dopasowac do rozmiaru grafiki wejsciowej
    encoder.setHeight( 500 ); // mozna dopasowac do rozmiaru grafiki wejsciowej
    encoder.setBitRate( 1.25 * ( 1024 * 1024 * 8 ) ); // 0.25 * ( 1024 * 1024 * 8 ) ??? // mozna wyznaczyc doswiadczalnie po przez stoponiowe zmnijszanie obserwujac jakosc
    encoder.setFramesRate( 25 );
    encoder.setGopSize( 40 ); // https://upload.wikimedia.org/wikipedia/commons/e/e2/GOP_2.svg
    encoder.setBFramesLimit( 30 ); // maksymalna ilosc ramek typu B na GOP
    encoder.setPixelFormat( AV_PIX_FMT_YUV420P ); // 4:2:0 https://en.wikipedia.org/wiki/Chroma_subsampling#Sampling_systems_and_ratios

    FILE *file = fopen( "output.h264", "wb" ); // można te pliki potem otwierac za pomoca VLC Player
    //TODO: sprawdzenie czy udalo sie otworzyc plik do zapisu

    if( encoder.open() )
    {
//        Frame *frame = encoder.createFrame( AV_PICTURE_TYPE_I );
        Frame *frame = encoder.createFrame();
        Packet *packet = encoder.createPacket();

        char path[ 50 ];

        int imageIndex = 0;
        int packetIndex = 0;

        bool packetReady;

        for( int j = 0; j < 20; ++j ) // 20s
        {
            for( int i = 0; i < encoder.getFramesRate(); ++i ) // klatki dla jednej sekundy
            {
                sprintf( path, "png_frames\\ppm_frames\\frame_%d.ppm", imageIndex ); // zapisywanie wewnatrz tablity path sciezki do pliku
                Image *image = Image::load( path );

                if( image == NULL )
                    break;

                //TODO: sprawdzenie zgodnosci rozmiarow obrazu i ramki

                frame->setTimestamp( imageIndex );

                copy420Image( image, frame ); // kopiowanie obrazu do ramki
//                copy422Image( image, frame ); // kopiowanie obrazu do ramki // AV_PIX_FMT_YUV422P
//                copy444Image( image, frame ); // AV_PIX_FMT_YUV444P

                if( ! encoder.encodeFrame( frame, packet, packetReady ) )
                    break;

                if( packetReady ) // kodowanie wymaga imformacji o sasiednich ramkach, dlatego dopiero po kilku iteracjach mozliwe jest odbieranie spakowanych ramek/paczek
                {
                    fwrite( packet->getData(), sizeof( uint8_t ), packet->getSize(), file );
                    //TODO: sprawdzenie czy powiodl sie zapis
                    //fflush( file );

                    std::cout << imageIndex << ". Write frame " << packetIndex << " (size=" << packet->getSize() << ")." << std::endl;

                    packet->reset(); // resetujemy stan pakietu na potrzeby wydobycia kolejne zakodowanej ramki
                    packetIndex += 1;
                }
                else
                    std::cout << imageIndex << "." << std::endl;

                imageIndex += 1;

                delete image;
            }
        }

        while( true ) // dla reszty ramek znajdujacych sie w buforach
        {
            if( ! encoder.encodeFrame( packet, packetReady ) ) // nalezy zapisac rozwniez te ramki, ktore zwracane sa z opoznieniem ze wzgledu na kompresje bazujaca na sasiedniuch ramkach - czyli dane w buforach
                break;

            if( packetReady )
            {
                fwrite( packet->getData(), sizeof( uint8_t ), packet->getSize(), file );
                //TODO: sprawdzenie czy powiodl sie zapis
                //fflush( file );

                std::cout << "[Rest] Write frame " << packetIndex << " (size=" << packet->getSize() << ")." << std::endl;

                packet->reset(); // resetujemy stan pakietu na potrzeby wydobycia kolejne zakodowanej ramki
                packetIndex += 1;
            }
            else
                break; // jesli nie ma juz danych w buforach to mozna przerwac ta petle
        }

        uint8_t terminator[] = { 0, 0, 1, 0xb7 };
        fwrite( terminator, 1, sizeof( terminator ), file ); // dodanie kodu koncowego sekwencji aby uzyskac strumien MPEG

        delete frame;

        encoder.close();
    }

    fclose( file );

    return 0;
}
