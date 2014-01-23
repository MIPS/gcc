CC	  = gcc
LD	  = ld
AR	  = ar
OBJCOPY   = objcopy

GCCWARN   = -Wall -Wstrict-prototypes
CFLAGS	  = -g -m32 -O2 -fno-strict-aliasing $(GCCWARN)
SFLAGS    = $(CFLAGS) -D__ASSEMBLY__
LDFLAGS   = $(CFLAGS)
M16       = -m16

SRC-DIR   = .
VPATH	  = $(SRC-DIR)
X16FLAGS  = -g $(M16) -D__SYS16__ -I $(SRC-DIR)/include16
S16FLAGS  = $(X16FLAGS) -D__ASSEMBLY__
C16OPTFLAGS = -Os
C16FLAGS  = $(X16FLAGS) $(GCCWARN) -march=i386 -mregparm=3 \
	     $(C16OPTFLAGS) -ffreestanding \
	    -fno-stack-protector -mpreferred-stack-boundary=2 \
	    -fno-pic

LD16FLAGS    = -m elf_i386 --section-start=.text=0x1000
LDWRAPFLAGS  = -m elf_i386 --section-start=.text=0x8000

LIB16S    = 
LIB16C    = conio.c crt0.c exit.c strlen.c
LIB16C   := $(addprefix lib16/,$(LIB16C))
LIB16O    = $(LIB16C:.c=.o) $(LIB16S:.S=.o)

TEST1O    = test16/test1.o
TEST1ELF  = $(TEST1O:.o=.elf)

all : run16 $(TEST1ELF)
	./run16 $(TEST1ELF)

lib16/%.o: lib16/%.S
	mkdir -p lib16
	$(CC) $(S16FLAGS) -c -o $@ $<

lib16/%.o: lib16/%.c
	mkdir -p lib16
	$(CC) $(C16FLAGS) -c -o $@ $<

lib16/lib16.a: $(LIB16O)
	mkdir -p $(@D)
	rm -f $@
	$(AR) cq $@ $^

test16/%.o: test16/%.S
	mkdir -p $(@D)
	$(CC) $(S16FLAGS) -c -o $@ $<

test16/%.o: test16/%.c
	mkdir -p $(@D)
	$(CC) $(C16FLAGS) -c -o $@ $<

test16/%.elf: test16/%.o lib16/lib16.a
	mkdir -p $(@D)
	$(LD) $(LD16FLAGS) -o $@ $^

run/wrap.elf: run/wrap.o
	mkdir -p $(@D)
	$(LD) $(LDWRAPFLAGS) -o $@ $^

run/wrap.bin: run/wrap.elf
	mkdir -p $(@D)
	$(OBJCOPY) -O binary $< $@

run/wwrap.o: run/wwrap.S run/wrap.bin

run/%.o: run/%.S
	mkdir -p $(@D)
	$(CC) $(SFLAGS) -c -o $@ $<

run/%.o: run/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

run16: run/run16.o run/wwrap.o
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f run16 *.o lib16/*.o lib16/*.a
	rm -f test16/*.o test16/*.elf test16/*.bin
	rm -f run/*.o run/*.elf run/*.bin

spotless: clean
	rm -f *~ */*~
