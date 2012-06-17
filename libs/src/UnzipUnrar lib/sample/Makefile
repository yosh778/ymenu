TARGET = UnzipUnrar
OBJS = main.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fexceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS = -lunziprar -lpspdebug -lpspsdk -lc -lstdc++ -lpspgum -lpspgu -lpng -lz -lm -lfreetype
BUILD_PRX = 0
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Unzip/Unrar sample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
