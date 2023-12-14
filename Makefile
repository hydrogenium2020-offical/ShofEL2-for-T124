CFLAGS := -Wall -Werror -I include -MMD

BIN_FILES = reset_example.bin jtag_example.bin boot_bct.bin mem_dumper_usb_server.bin

all: shofel2_t124 $(BIN_FILES)

# --------- x86 ----------

CC_x86 = gcc
CFLAGS_x86 := $(CFLAGS)
# shameless copypasta from https://stackoverflow.com/a/2908351/375416
C_FILES_x86:= $(wildcard exploit/*.c)
OBJ_FILES_x86 := $(addprefix build/obj_x86/,$(notdir $(C_FILES_x86:.c=.o)))
-include $(OBJ_FILES_x86:.o=.d)

build/obj_x86/%.o: exploit/%.c
	$(CC_x86) $(CFLAGS_x86) -c -o $@ $<

shofel2_t124: $(OBJ_FILES_x86)
	$(CC_x86) $(CFLAGS_x86) -o $@ $^

# ------------------------


# ----- ARMv4t Thumb -----

TOOLCHAIN_ARM ?= arm-none-eabi-
CC_ARM = $(TOOLCHAIN_ARM)gcc
AS_ARM = $(TOOLCHAIN_ARM)as
OBJCOPY_ARM = $(TOOLCHAIN_ARM)objcopy


ARCH := -march=armv4t -mtune=arm7tdmi -mthumb -mthumb-interwork
CFLAGS_ARM = $(ARCH) -O2 -nostdlib -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-inline -std=gnu11 -T,payloads/payload.ld# -Wall
-include $(OBJ_FILES_ARM:.o=.d)

build/obj_arm/%.o: payloads/%.c
	$(CC_ARM) $(CFLAGS_ARM) -c -o $@ $<

build/reset_example.elf: build/obj_arm/reset_example.o
	$(CC_ARM) $(CFLAGS_ARM) -o $@ $^

build/jtag_example.elf: build/obj_arm/jtag_example.o
	$(CC_ARM) $(CFLAGS_ARM) -o $@ $^

build/boot_bct.elf: build/obj_arm/boot_bct.o
	$(CC_ARM) $(CFLAGS_ARM) -o $@ $^

build/mem_dumper_usb_server.elf: build/obj_arm/mem_dumper_usb_server.o
	$(CC_ARM) $(CFLAGS_ARM) -o $@ $^


build/ipatch_rcm.elf: build/obj_arm/ipatch_rcm.o
	$(CC_ARM) $(CFLAGS_ARM) -o $@ $^

%.bin: build/%.elf
	$(OBJCOPY_ARM) -O binary $< $@

# ------------------------


clean:
	rm -f $(OBJ_FILES_ARM) $(OBJ_FILES_x86)
	rm -f shofel2_t124 build/*.elf $(BIN_FILES)

cleanall: clean
	rm -f build/obj_arm/*.d build/obj_x86/*.d

