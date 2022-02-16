CXX 	= g++
AR 	= ar
SIZE 	= size
OBJCOPY = objcopy
AVRDUDE = avrdude

SIZEFLAGS = 

PRJCXXFLAGS = -Os -g \
	-fno-exceptions -std=c++14 \
	-W -Wall -pedantic
	
LDFLAGS := $(LDFLAGS) -Wl,--gc-sections -Wl,--relax

#FORMAT = ihex binary
FORMAT = elf64-x86-64
