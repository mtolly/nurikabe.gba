PATH := $(DEVKITARM)/bin:$(PATH)

# --- Project details -------------------------------------------------

PROJ    := nurikabe
TARGET  := $(PROJ)

OBJS    := $(PROJ).o tiles16.o tiles8.o

# --- Build defines ---------------------------------------------------

PREFIX  := arm-none-eabi-
CC      := $(PREFIX)gcc
LD      := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy

ARCH    := -mthumb-interwork -mthumb
SPECS   := -specs=gba.specs

CFLAGS  := $(ARCH) -O2 -Wall -fno-strict-aliasing -std=c99 -I tonclib/include/
LDFLAGS := $(ARCH) $(SPECS) -L tonclib/lib/ -l tonc


.PHONY : build clean

# --- Build -----------------------------------------------------------
# Build process starts here!
build: $(TARGET).gba

# Strip and fix header (step 3,4)
$(TARGET).gba : $(TARGET).elf
	$(OBJCOPY) -v -O binary $< $@
	-@gbafix $@

# Link (step 2)
$(TARGET).elf : $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

# Compile (step 1)
nurikabe.o: nurikabe.c tiles16.h tiles8.h
	$(CC) -c $< $(CFLAGS) -o $@
tiles16.o: tiles16.s
	$(CC) -c $< $(CFLAGS) -o $@
tiles8.o: tiles8.s
	$(CC) -c $< $(CFLAGS) -o $@
%.c: %.c.erb
	erb $< > $@
tiles16.s tiles16.h : tiles16.png
	grit tiles16.png -gT ff00ff -gB 4
tiles8.s tiles8.h : tiles8.png
	grit tiles8.png -gT ff00ff -gB 4

# --- Clean -----------------------------------------------------------

clean :
	@rm -fv *.gba
	@rm -fv *.elf
	@rm -fv *.o

#EOF
