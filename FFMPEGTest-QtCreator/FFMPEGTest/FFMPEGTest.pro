LIBS += -L"$${PWD}/lib/" \
    -l"avcodec.dll" \
    -l"avdevice.dll" \
    -l"avfilter.dll" \
    -l"avformat.dll" \
    -l"avutil.dll" \
    -l"postproc.dll" \
    -l"swresample.dll" \
    -l"swscale.dll"

INCLUDEPATH += "include/"

SOURCES += \
    main.cpp \
    portable/binaryportableimage.cpp \
    portable/ppmimage.cpp \
    portable/pgmimage.cpp \
    ffmpeg/ffmpegencoder.cpp \
    ffmpeg/frame.cpp \
    ffmpeg/packet.cpp

HEADERS += \
    portable/binaryportableimage.h \
    portable/ppmimage.h \
    portable/pgmimage.h \
    ffmpeg/ffmpegencoder.h \
    ffmpeg/frame.h \
    ffmpeg/preprocesor.h \
    ffmpeg/packet.h \
    utils.h
