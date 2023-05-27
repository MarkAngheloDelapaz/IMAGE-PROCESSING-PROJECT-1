#-------------------------------------------------
#
# Project created by QtCreator 2012-11-28T08:57:36
#
#-------------------------------------------------

QT       += core gui opengl network

TARGET = Rtmcrb102
TEMPLATE = app

INSTALLS=target
#target.path=//192.168.127.22:/home/ipg/user/surface
#target.path=//192.168.127.22:/home/ipg/user/back
#target.path=//192.168.127.22:/home/ipg/user/LTR50L/surface
#target.path=//192.168.93.1:/home/ipg/user/back
#target.path=//10.32.27.215:/home/ipg/user/pwia3


QMAKE_CFLAGS += -v -Wall -diag-error -use-msasm -O2 -Zp4 -ip -msse3 -fasm-blocks -fbuiltin -funsigned-char -no-multibyte-chars -openmp
QMAKE_LINK = icc -v -Wall -diag-error -ip -msse3 -lpthread -static-intel -shared-libgcc -openmp -Xlinker -M

INCLUDEPATH	+= $(TSC9301_LIBRARY_PATH)/9301base_ver5.062/include
INCLUDEPATH	+= $(TSC9301_LIBRARY_PATH)/9301base_ver5.062/include/R_cs

CONFIG(debug, debug|release) {
	LIBS += -L$(TSC9301_LIBRARY_PATH)/9301base_ver5.062/debug
#        LIBS += -L$(TSC9301_LIBRARY_PATH)/R_cs_fc19_20170605_107b87_lib401v202sp/debug
} else {
	LIBS += -L$(TSC9301_LIBRARY_PATH)/9301base_ver5.062/release
#        LIBS += -L$(TSC9301_LIBRARY_PATH)/R_cs_fc19_20170605_107b87_lib401v202sp/release
}
LIBS += -lR_cs -l9301base -lqwt
LIBS += -lpng
LIBS += -lusb

LIBS += -lopencv_highgui
LIBS += -lopencv_core -lopencv_imgproc

#DEFINES += USE_MANUALMODE
DEFINES += X_WINDOW_SYSTEM
DEFINES += DEBUG
#DEFINES += FIND_HISTORY_DEBUG

#INSTALLS=target
#target.path=/home/ipg/user/twasid0

HEADERS += \
    tp_sub.h \
    tmcr00i.h \
    SpecSet.h \
    mark03x6.h \
    mark18x3x1.h \
    Setting.h \
    mcr03.h \
    esr25.h \
    mcr10.h \
    mark10x4.h \
    mark18x2.h \
    LTR50_Ma.h \
    LTR18_Ma.h \
    gpioinfo.h \
    mcr18.h \
    ltr50.h \
    ltr18.h

SOURCES += \
    tp_sub.c \
    tmcr00i00.c \
    mcr03.c \
    esr25.c \
    SpecSet.c \
    Setting.c \
    mcr10.c \
    mcr18.c \
    ltr50.c \
    ltr18.c

LIBS	+= -lqwt
#LIBS    += -lpng -lusb

INCLUDEPATH += /usr/include/qwt

#If not use USB camera, then comment out following 4 lines.
#DEFINES += BASLER_USB_CAMERA
#LIBS += -L$(PYLON_ROOT)/lib
#LIBS += -L$(PYLON_ROOT)/genicam/bin/Linux32_i86
#LIBS += -lpylonbase -lGenApi_gcc40_v2_3 -lGCBase_gcc40_v2_3 -lLog_gcc40_v2_3 -lMathParser_gcc40_v2_3 -lXerces-C_gcc40_v2_7_1 -llog4cpp_gcc40_v2_3
