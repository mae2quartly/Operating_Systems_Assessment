#build the ISO bootable image as default
all: os.iso

#assemble loader.asm as an object file 
source/loader.o: source/loader.asm
	nasm -f elf source/loader.asm -o source/loader.o

#assemble io.s into an object file for hardware ports
drivers/io.o: drivers/io.s
	nasm -f elf drivers/io.s -o drivers/io.o

#assemble hardware_interrupt_enabler.s into an object file for enabling/disabling interrupts
drivers/hardware_interrupt_enabler.o: drivers/hardware_interrupt_enabler.s
	nasm -f elf drivers/hardware_interrupt_enabler.s -o drivers/hardware_interrupt_enabler.o

#assemble interrupt_handlers.s into an object file for loading the IDT
drivers/interrupt_handlers.o: drivers/interrupt_handlers.s
	nasm -f elf drivers/interrupt_handlers.s -o drivers/interrupt_handlers.o

#assemble interrupt_asm.s into an object file for low-level interrupt wrappers
drivers/interrupt_asm.o: drivers/interrupt_asm.s
	nasm -f elf drivers/interrupt_asm.s -o drivers/interrupt_asm.o

#compile frame_buffer.c into a 32-bit object file
drivers/frame_buffer.o: drivers/frame_buffer.c drivers/frame_buffer.h drivers/io.h
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c drivers/frame_buffer.c -o drivers/frame_buffer.o

#compile pic.c into a 32-bit object file
drivers/pic.o: drivers/pic.c drivers/pic.h drivers/io.h drivers/type.h
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c drivers/pic.c -o drivers/pic.o

#compile keyboard.c into a 32-bit object file
drivers/keyboard.o: drivers/keyboard.c drivers/keyboard.h drivers/io.h drivers/frame_buffer.h drivers/type.h
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c drivers/keyboard.c -o drivers/keyboard.o

#compile interrupts.c into a 32-bit object file
drivers/interrupts.o: drivers/interrupts.c drivers/interrupts.h drivers/pic.h drivers/io.h drivers/frame_buffer.h drivers/keyboard.h drivers/type.h
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c drivers/interrupts.c -o drivers/interrupts.o

#compile kmain.c into bare-metal object file (remove all linux libraries - bare minimum and turn on compiler warnings)
source/kmain.o: source/kmain.c drivers/frame_buffer.h drivers/interrupts.h drivers/hardware_interrupt_enabler.h drivers/type.h
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c source/kmain.c -o source/kmain.o

#link all object files into kernel.elf using linker script 
kernel.elf: source/loader.o source/kmain.o drivers/io.o drivers/frame_buffer.o drivers/pic.o drivers/keyboard.o drivers/interrupts.o drivers/interrupt_handlers.o drivers/interrupt_asm.o drivers/hardware_interrupt_enabler.o source/link.ld
	ld -T ./source/link.ld -melf_i386 source/loader.o source/kmain.o drivers/io.o drivers/frame_buffer.o drivers/pic.o drivers/keyboard.o drivers/interrupts.o drivers/interrupt_handlers.o drivers/interrupt_asm.o drivers/hardware_interrupt_enabler.o -o kernel.elf

#copy kernel.elf into the GRUB folder and package os.iso
os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o os.iso iso

#boot os.iso in QEMU using curses mode with telnet monitor on port 45454
run: os.iso
	qemu-system-i386 -display curses -monitor telnet::45454,server,nowait -serial mon:stdio -boot d -cdrom os.iso -m 32 -d cpu -D logQ.txt

#boot os.iso with CPU register logging for debugging
run-log: os.iso
	qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 -d cpu -D logQ.txt

#remove all compiled files to rebuild cleanly 
clean:
	rm -rf source/*.o drivers/*.o kernel.elf os.iso iso/boot/kernel.elf logQ.txt