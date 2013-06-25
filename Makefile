#
SOURCES=boot.o main.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link 

clean:
	-rm *.o kernel

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
