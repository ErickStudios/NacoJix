
CC = gcc

CFLAGS = -m32 \
         -ffreestanding \
         -fno-pie \
         -fno-stack-protector \
         -O2 \
         -Wall \
         -Wextra

LDFLAGS = -m32 \
          -nostdlib \
          -no-pie \
          -T linker.ld \
          -Wl,--build-id=none

BUILD = build

all: $(BUILD)/jix.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: boot/boot.S | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/main.o: kernel/main.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/memheap.o: kernel/memory/heap.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/jix.o: kernel/jix.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/jix.elf: $(BUILD)/boot.o $(BUILD)/main.o $(BUILD)/memheap.o $(BUILD)/jix.o
	$(CC) $(LDFLAGS) $^ -lgcc -o $@

$(BUILD)/jix.iso: $(BUILD)/jix.elf
	mkdir -p $(BUILD)/isodir/boot/grub
	cp $< $(BUILD)/isodir/boot/jix.elf
	cp grub/grub.cfg $(BUILD)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(BUILD)/isodir

run: all
	qemu-system-i386 -machine pc -m 128M \
	    -cdrom $(BUILD)/jix.iso -boot d

clean:
	rm -rf $(BUILD)

.PHONY: all run clean
