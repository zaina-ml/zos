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
	-Isrc \
	-mcmodel=kernel

LDFLAGS := \
	-nostdlib \
	-static \
	-no-pie \
	-T linker.ld

BUILD_DIR := build
KERNEL := main.elf
SRCS := \
	src/main.c \
	src/limine/request.c \
	src/graphics/graphics.c \
	src/graphics/font.c
OBJS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all build clean iso run

all: clean iso run

build: $(KERNEL)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: src/%.c
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