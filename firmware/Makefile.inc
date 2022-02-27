PORT 		= -P /dev/ttyUSB0
UPLOAD_PROTOCOL = arduino
# UPLOAD_PROTOCOL = usbtiny
BUILD_MCU 	= atmega644p
BUILD_VARIANT 	= standard
UPLOAD_SPEED 	= 115200
BUILD_F_CPU 	= 20000000L


###############################################################################
#
# Tools & flags
#
################################################################################

CXX 	= avr-g++
AR 	= avr-ar
SIZE 	= avr-size
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

DUDEFLAGS = -V -p $(BUILD_MCU) $(PORT) -c $(UPLOAD_PROTOCOL) -b $(UPLOAD_SPEED)
#FUSES = -U lfuse:w:0xFF:m -U hfuse:w:0xD6:m -U efuse:w:0x05:m
FUSES  = -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xFD:m


SIZEFLAGS = 

PRJCXXFLAGS = -Os -g -mmcu=$(BUILD_MCU) -DF_CPU=$(BUILD_F_CPU) \
	-ffunction-sections -fdata-sections -fmerge-all-constants \
	-fno-inline-small-functions -fshort-enums \
	-fno-exceptions -std=c++14 \
	-W -Wall -pedantic

LDFLAGS := $(LDFLAGS) -Wl,--gc-sections -Wl,--relax

FORMAT = ihex

# binary
