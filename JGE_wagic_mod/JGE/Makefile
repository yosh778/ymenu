ifeq ($(MAKECMDGOALS),debug)
DEBUG = true
endif

ifeq ($(MAKECMDGOALS),log)
DOLOG = true
endif


GENERIC_OBJS = src/JApp.o src/JGBKFont.o \
	src/JGE.o src/JGui.o src/JLBFont.o \
    src/JLogger.o \
	src/JGameObject.o src/JSpline.o src/JAnimator.o \
	src/JResourceManager.o src/JFileSystem.o \
	src/JNetwork.o \
	src/JParticle.o src/JParticleEmitter.o src/JParticleEffect.o \
	src/JParticleSystem.o \
	src/zipFS/zfsystem.o src/zipFS/ziphdr.o src/zipFS/zstream.o \
	src/JSprite.o src/Vector2D.o \
	src/tinyxml/tinystr.o src/tinyxml/tinyxml.o \
	src/tinyxml/tinyxmlparser.o src/tinyxml/tinyxmlerror.o \
	src/Encoding.o src/JTTFont.o \
	src/JMD2Model.o src/JOBJModel.o
PSP_OBJS = src/JSocket.o src/JGfx.o src/JSfx.o src/JAudio.o src/JMP3.o src/decoder_prx.o src/main.o src/vram.o
LINUX_OBJS = src/pc/JGfx.o src/pc/JSfx.o src/Xmain.o


HGE_OBJS = src/hge/hgecolor.o src/hge/hgeparticle.o \
       src/hge/hgerect.o src/hge/hgevector.o \
       src/hge/hgedistort.o src/hge/hgefont.o


CXXFLAGS = -W -Wall -Werror -Wno-unused

ifdef DEBUG
CXXFLAGS += -ggdb3
endif

ifdef DOLOG
CXXFLAGS += -DDOJLOG
endif

# Determination of target.
# TARGET_ARCHITECTURE variable will then be set to either linux or psp.

RESULT = $(shell psp-config --psp-prefix 2> Makefile.cache)
ifeq ($(RESULT),)
DEFAULT_RULE = linux
TARGET_ARCHITECTURE = linux
else
DEFAULT_RULE = 3xx
TARGET_ARCHITECTURE = psp
TARGET_LIB = libjge300.a
endif

ifeq ($(MAKECMDGOALS),linux)
DEFAULT_RULE = linux
TARGET_ARCHITECTURE = linux
endif
ifeq ($(MAKECMDGOALS),3xx)
DEFAULT_RULE = 3xx
endif

ifeq ($(DEFAULT_RULE),3xx)
TARGET_ARCHITECTURE = psp
TARGET_LIB = libjge300.a
CXXFLAGS += -DDEVHOOK -DPSPFW3XX
PSP_FW_VERSION=371
endif

ifeq ($(TARGET_ARCHITECTURE),psp)
PSPSDK = $(shell psp-config --pspsdk-path)
PSPDIR = $(shell psp-config --psp-prefix)
OBJS = $(GENERIC_OBJS) $(PSP_OBJS)
TARGET_HGE = libhgetools.a
INCDIR = include/psp include/psp/freetype2 ../Boost src/zipFS
CXXFLAGS += -O2 -G0 -DPSP
LIBDIR = lib/psp
endif
ifeq ($(TARGET_ARCHITECTURE),linux)
OBJS = $(GENERIC_OBJS) $(LINUX_OBJS)
TARGET_LIB = libjge.a
TARGET_HGE = libhgetools.a
INCDIR = -I/usr/X11/include -I../Boost -Isrc/zipFS -Iinclude/
CXXFLAGS += -DLINUX $(FMOD)
CXXFLAGS += $(INCDIR)
LIBDIR = lib/linux
endif

# Set definitive values for variables.
TARGET_LIB := $(LIBDIR)/$(TARGET_LIB)
TARGET_HGE := $(LIBDIR)/$(TARGET_HGE)

ifeq ($(TARGET_ARCHITECTURE),psp)
include $(PSPSDK)/lib/build.mak
endif

all: $(DEFAULT_RULE) Makefile.$(TARGET_ARCHITECTURE) hge

debug: $(DEFAULT_RULE) hge

linux: $(TARGET_LIB) hge

3xx: $(TARGET_LIB)

install: $(TARGET_LIB) hge

hge: $(TARGET_HGE)

ifeq ($(TARGET_ARCHITECTURE),linux)


$(TARGET_LIB): $(OBJS)
	ar r $(TARGET_LIB) $(OBJS)

$(TARGET_HGE): $(HGE_OBJS)
	ar r $(TARGET_HGE) $(HGE_OBJS)

clean:
	$(RM) -f $(OBJS) $(HGE_OBJS) Makefile.$(TARGET_ARCHITECTURE)
endif

Makefile.psp:
	echo #Makefile.psp > $@

Makefile.linux:
	$(CXX) -o /dev/null src/testfeatures.c -L$(LIBDIR) -lfmod-3.75 > /dev/null 2>&1 ; if [ "0" = "$$?" ]; then echo 'FMOD=-DWITH_FMOD'; else echo 'FMOD=-DWITHOUT_FMOD'; fi > $@

-include Makefile.$(TARGET_ARCHITECTURE)
