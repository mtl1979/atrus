CXX = g++
CFLAGS = -I../muscle -I../ -W -Wall -Wno-multichar -Wno-ctor-dtor-privacy -g -O3 -fno-pic -fomit-frame-pointer -DMUSCLE_SINGLE_THREAD_ONLY
CXXFLAGS = $(CFLAGS)
LFLAGS = -g -O3 -fno-pic -fomit-frame-pointer
LIBS =
EXECUTABLES = bin/Atrus
REGEXOBJS = 

ifeq ($(OSTYPE),beos)
  ifeq ($(BE_HOST_CPU),ppc)
    CXX = mwcc
    REGEXOBJS = regex.o
  else
    LIBS = -lbe -lnet -lroot
    ifeq ($(shell ls 2>/dev/null -1 /boot/develop/headers/be/bone/bone_api.h), /boot/develop/headers/be/bone/bone_api.h)
      CFLAGS += -I/boot/develop/headers/be/bone -DBONE
      LIBS = -nodefaultlibs -lbind -lsocket -lbe -lroot -L/boot/beos/system/lib
    endif   
  endif
else
  ifeq ($(OSTYPE),freebsd4.0)
     CXXFLAGS += -I/usr/include/machine
  endif
endif

VPATH = ../muscle/message ../muscle/besupport ../muscle/iogateway ../muscle/reflector ../muscle/regex ../muscle/util ../muscle/syslog ../muscle/system

# Makes all the programs that can be made using just cross-platform code
all : $(EXECUTABLES)

bin/Atrus : main.o VoidBot.o Bot.o BotData.o UserList.o User.o Message.o AbstractMessageIOGateway.o MessageIOGateway.o String.o NetworkUtilityFunctions.o SysLog.o PathMatcher.o StringMatcher.o PulseNode.o SetupSystem.o FilePathInfo.o SocketMultiplexer.o ByteBuffer.o Directory.o QueryFilter.o MiscUtilityFunctions.o
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBS)

clean :
	rm -f *.o *.xSYM $(EXECUTABLES)
