.PHONY : all debug clean

VM=qemu
VM_OPT=-boot order=a -vga std
VM_DEBUG_OPT=-s -S $(VM_OPT)

TARGET=x86

KERNEL_ASM_SRC=	$(wildcard src/arch/$(TARGET)/*.S) 		\
		$(wildcard src/arch/$(TARGET)/graphic/*.S)	\
		$(wildcard src/arch/$(TARGET)/io/*.S)		\
		$(wildcard src/arch/$(TARGET)/system/*.S)
KERNEL_ASM_OUT=$(KERNEL_ASM_SRC:.S=.o)

BOOTSRC=$(wildcard src/arch/$(TARGET)/boot/*.asm)
BOOTBIN=$(BOOTSRC:.asm=.bin)

KERNEL_SRC=	$(wildcard src/*.c)		\
		$(wildcard src/display/*.c) 	\
		$(wildcard src/system/*.c)	\
		$(wildcard src/string/*.c)	\
		$(wildcard src/io/*.c)		\
		$(wildcard src/command/*.c)
KERNEL_OUT=$(KERNEL_SRC:.c=.o)

KERNEL_INC=-I src/include/ -I src/arch/$(TARGET)/include/

ENTRYPOINT=start
OFFSET=0x500

BIN_FOLDER=bin/
OUT=$(BIN_FOLDER)/llr.img
TEMPIMG=$(BIN_FOLDER)/temp.bin

ASM=yasm
CC=gcc

ASMFLAGS=-f bin
KERNELASMF=-f elf32

CFLAGS=-m32 -ffreestanding -std=c99 -Wall $(KERNEL_INC)
LDFLAGS=-m elf_i386 -Ttext $(OFFSET) --oformat binary

QUIET=2>/dev/null
FORMAT_PRINT=printf "%-45s"

all: $(BOOTBIN) $(KERNEL_OUT) $(KERNEL_ASM_OUT)
	@$(FORMAT_PRINT) "Linking kernel ... "
	@ld $(LDFLAGS) -e $(ENTRYPOINT) -o $(TEMPIMG) $(KERNEL_ASM_OUT) $(KERNEL_OUT)
	@echo "Done!"

	@mkdir -p $(BIN_FOLDER)
	@$(FORMAT_PRINT) "Preparing bootable image ... "
	@dd if=/dev/zero of=$(OUT) count=1 bs=1440K $(QUIET)
	@dd if=$(BOOTBIN) of=$(OUT) bs=512 count=1 conv=notrunc $(QUIET)
	@dd if=$(TEMPIMG) of=$(OUT) bs=512 count=17 conv=notrunc seek=1 $(QUIET)
	@echo "Done!"

$(KERNEL_ASM_OUT): $(KERNEL_ASM_SRC)
	@$(FORMAT_PRINT) $@
	@$(CC) $(CFLAGS) -c $(patsubst %.o, %.S, $@) -o $@
	@echo "OK"

$(KERNEL_OUT): $(KERNEL_SRC)
	@$(FORMAT_PRINT) $@
	@$(CC) $(CFLAGS) -c $(patsubst %.o, %.c, $@) -o $@
	@echo "OK"

$(BOOTBIN): $(BOOTSRC)
	@$(FORMAT_PRINT) $@
	@$(ASM) $(ASMFLAGS) -o $@ $(patsubst %.bin, %.asm, $@)
	@echo "OK"

run: all
	@echo "Starting image ..."
	@$(VM) $(VM_OPT) -fda $(OUT) &

debug: all
	@echo "Starting virtual machine for debugging ..."
	@$(VM) $(VM_DEBUG_OPT) -fda $(OUT) &
	@echo "To connect to virtual machine from GDB type :"
	@echo " (gdb) set architecture i8086"
	@echo " (gdb) target remote localhost:1234"

clean:
	@$(FORMAT_PRINT) "Removing binary files ... "
	@rm -f $(TEMPIMG)
	@rm -f $(BOOTBIN)
	@rm -f $(KERNEL_OUT)
	@rm -f $(KERNEL_ASM_OUT)
	@echo "Done!"
	@$(FORMAT_PRINT) "Removing task disk image ... "
	@rm -f $(OUT)
	@echo "Done!"

