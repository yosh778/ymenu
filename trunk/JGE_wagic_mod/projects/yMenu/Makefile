TARGET = yMenu
OBJS = src/GameApp.o src/GameLauncher.o src/DSystm.o src/YEntry.o src/lib.o src/Eboot.o src/YDir.o src/Display.o src/Sfo.o src/YLaunch.o src/YISO.o src/YCSO.o

BUILD_PRX = 1
PSP_FW_VERSION=660

CFLAGS = -O2 -G0 -Wall -DDEVHOOK -DPSPFW3XX
CXXFLAGS = $(CFLAGS) -fno-exceptions
ASFLAGS = $(CXXFLAGS)

INCDIR = ../../JGE/include ../../JGE/src/zipFS ../../JGE/include/psp ../../JGE/include/psp/freetype2 ../../JGE/src ../../Boost ../../../libs/include
LIBDIR = ../../JGE/lib/psp ../../../libs/lib
LDFLAGS =
LIBS = -lunziprar -ljge300 -lhgetools -lfreetype -lc -ljpeg -lgif -lpng -lz -lm -lmikmod -lpsppower -lpspmpeg -lpspaudiocodec -lpspaudiolib -lpspaudio -lpspmp3 -lpsprtc -lstdc++ -lpspfpu -lintrafont -lpspgum -lpspgu

# Either pro / ark / tn / vhbl
ifndef MODE
	MODE=pro
endif

ifeq ($(MODE), tn)
	CXXFLAGS+= -DTN_CFW
else
ifeq ($(MODE), vhbl)
	CXXFLAGS+= -DVHBL
else
ifeq ($(MODE), pro)
	CXXFLAGS+= -DPROCFW
endif
endif
endif

ifdef DEBUG
	CXXFLAGS+= -DYDEBUG
endif


ifneq ($(MODE), vhbl)
	LIBS+= -lpspsystemctrl_user_${MODE}
else
	CXXFLAGS+= -DVHBL
endif


EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = yMenu
PSP_EBOOT_ICON = icon_full.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
