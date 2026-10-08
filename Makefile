CC := gcc

CFLAGS := \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-pie \
	-m64 \
	-mno-red-zone \
	-Wall \
	-Wextra \
	-Isrc/include \
	-mcmodel=kernel

LDFLAGS := \
	-nostdlib \
	-static \
	-no-pie \
	-T linker.ld

BUILD_DIR := build
KERNEL := main.elf
SRCS := \
	src/kernel/main.c \
	src/kernel/drivers/video/framebuffer.c \
	src/kernel/graphics/graphics.c \
	src/kernel/graphics/font.c \
	src/kernel/lib/string.c \
	src/kernel/terminal/terminal.c \
	src/kernel/terminal/printk.c \
	src/kernel/init.c \
	src/kernel/panic.c

OBJS := $(patsubst src/kernel/%.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all build clean iso run

all: clean iso run

build: $(KERNEL)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: src/kernel/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

iso: $(KERNEL)
	./scripts/iso.sh

run: iso
	./scripts/run.sh

clean:
	rm -rf $(BUILD_DIR) $(KERNEL) zos.iso