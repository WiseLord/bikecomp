# Output file name
TARG     = bikecomp

DISPLAY = ILI9341
DISPVAR = SPI

# MCU name and frequency
MCU      = atmega328p
F_CPU    = 16000000

# Source files
SRCS     = $(wildcard *.c)

SRCS += display/glcd.c
SRCS += display/dispdrv.c
SRCS += display/gc320x240/ili9341.c

SRCS += $(wildcard display/fonts/font*.c)

DEFINES += -D_$(DISPLAY)
DEFINES += -D_DISP_$(DISPVAR)

DEFINES += -D_DISP_RST_ENABLED
DEFINES += -D_DISP_BCKL_ENABLED

# Build directory
BUILDDIR = build

# Compiler options
OPTIMIZE = -Os -mcall-prologues -fshort-enums -ffunction-sections -fdata-sections
DEBUG    = -g -Wall -Werror
DEPS     = -MMD -MP -MT $(BUILDDIR)/$(*F).o -MF $(BUILDDIR)/$(*D)/$(*F).d
CFLAGS   = $(DEBUG) -lm $(OPTIMIZE) $(DEPS) -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS  = $(DEBUG) -mmcu=$(MCU) -Wl,-gc-sections -mrelax

CFLAGS += -Idisplay/fonts
CFLAGS += -I.

# AVR toolchain and flasher
CC       = avr-gcc
OBJCOPY  = avr-objcopy
OBJDUMP  = avr-objdump

# AVRDude parameters
AVRDUDE  = avrdude
AD_MCU   = -p $(MCU)
#AD_PROG = -c stk500v2
#AD_PORT = -P avrdoper

AD_CMD   = $(AD_MCU) $(AD_PROG) $(AD_PORT) -V

# Build objects
OBJS     = $(addprefix $(BUILDDIR)/, $(SRCS:.c=.o))
ELF      = $(BUILDDIR)/$(TARG).elf
HEX      = flash/$(TARG).hex

all: $(HEX) size

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom -R .nwram $(ELF) $(HEX)

$(ELF): $(OBJS)
	@mkdir -p $(addprefix $(BUILDDIR)/, $(SUBDIRS)) flash
	$(CC) $(LDFLAGS) -o $(ELF) $(OBJS)
	$(OBJDUMP) -h -S $(ELF) > $(BUILDDIR)/$(TARG).lss

size: $(ELF)
	@sh ./size.sh $(ELF)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEFINES) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

.PHONY: flash
flash: $(ELF)
	$(AVRDUDE) $(AD_CMD) -U flash:w:flash/$(TARG).hex:i

.PHONY: fuse
fuse:
	$(AVRDUDE) $(AD_CMD) -U lfuse:w:0xff:m -U hfuse:w:0xd1:m -U efuse:w:0xFF:m

# Dependencies
-include $(OBJS:.o=.d)
