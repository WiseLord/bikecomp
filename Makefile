# Output file name
TARG     = bikecomp

# MCU name and frequency
MCU      = atmega328p
F_CPU    = 16000000

# Source files
SRCS     = $(wildcard *.c)

# Build directory
BUILDDIR = build

# Compiler options
OPTIMIZE = -Os -mcall-prologues -fshort-enums -ffunction-sections -fdata-sections
DEBUG    = -g -Wall -Werror
DEPS     = -MMD -MP -MT $(BUILDDIR)/$(*F).o -MF $(BUILDDIR)/$(*D)/$(*F).d
CFLAGS   = $(DEBUG) -lm $(OPTIMIZE) $(DEPS) -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS  = $(DEBUG) -mmcu=$(MCU) -Wl,-gc-sections -mrelax

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

all: $(ELF) size

# Dependencies
-include $(OBJS:.o=.d)

$(ELF): $(OBJS)
	@mkdir -p $(BUILDDIR) flash
	$(CC) $(LDFLAGS) -o $(ELF) $(OBJS) -lm
	$(OBJCOPY) -O ihex -R .eeprom -R .nwram $(ELF) flash/$(TARG).hex
	$(OBJDUMP) -h -S $(ELF) > $(BUILDDIR)/$(TARG).lss

size:
	@sh ./size.sh $(ELF)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

.PHONY: flash
flash: $(ELF)
	$(AVRDUDE) $(AD_CMD) -U flash:w:flash/$(TARG).hex:i

.PHONY: fuse
fuse:
	$(AVRDUDE) $(AD_CMD) -U lfuse:w:0xff:m -U hfuse:w:0xd8:m -U efuse:w:0xCB:m
