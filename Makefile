CC = gcc

CFLAGS = \
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

LDFLAGS = \
	-nostdlib \
	-static \
	-no-pie \
	-T linker.ld

BUILD = build

OBJS = \
	$(BUILD)/main.o \
	$(BUILD)/graphics.o

KERNEL = main.elf

.PHONY: all clean run iso

all: $(KERNEL)

$(BUILD):
	mkdir -p $(BUILD)


$(BUILD)/main.o: src/main.c src/graphics.h src/limine.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/main.c -o $(BUILD)/main.o


$(BUILD)/graphics.o: src/graphics.c src/graphics.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/graphics.c -o $(BUILD)/graphics.o


$(KERNEL): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(KERNEL)


clean:
	rm -rf $(BUILD) $(KERNEL)

run: iso
	./scripts/run.sh


iso: $(KERNEL)
	./scripts/iso.sh