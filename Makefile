SRCS = $(shell find . -iname '*.c' -not -path './kernel/*')
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
OUT_BIN = badappleinit
ISO = init.iso
BADAPPLE = badapple.mp4

all: $(OUT_BIN) frames/.folder
.PHONY: all

%.o: %.c
	$(CC) -O2 -MMD -static -c $< -o $@

$(OUT_BIN): $(OBJS)
	$(CC) -static $(OBJS) -o $@ -lm

-include $(DEPS)

iso: $(ISO)
.PHONY: iso

$(ISO): $(OUT_BIN) frames/.folder
	rm -rf initfs
	mkdir -p initfs/sbin
	cp $(OUT_BIN) initfs/sbin/init
	cp -r frames initfs/frames
#	sudo chown -R root:root initfs
	xorriso -as mkisofs initfs -o $(ISO)

run: $(ISO)
	qemu-system-x86_64 -kernel kernel/arch/x86/boot/bzImage -cdrom $(ISO) -enable-kvm -append "root=/dev/sr0" -vga qxl -smp 4 -m 128M
.PHONY: run

clean:
	rm -rf $(ISO) $(OUT_BIN) $(OBJS) $(DEPS) initfs frames badapple.mp4
.PHONY: clean

$(BADAPPLE):
	yt-dlp -f mp4 'https://www.youtube.com/watch?v=FtutLA63Cp8' -o $(BADAPPLE)

frames/.folder: $(BADAPPLE)
	mkdir -p frames
	touch frames/.folder
	ffmpeg -i $(BADAPPLE) 'frames/%04d.png'
