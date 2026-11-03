#all files (tasks) to be executed 
all: task1 task1_2 task2_1 task2_2 task2_3

#the basics of a make file syntax - 
#target: ingredients (dependencies)
#   recipe command to run

#run the library/helper files first 
#to creates asm_io.o i need asm.io.asm to run 
#use shortcuts at the top to run these files 
asm_io.o: asm_io.asm
	nasm -f elf asm_io.asm -o asm_io.o

driver.o: driver.c
	gcc -m32 -c driver.c -o driver.o

#run the files using the same rules as before to complete task 1 
task1.o: task1.asm
	nasm -f elf task1.asm -o task1.o

task1: task1.o driver.o asm_io.o
	gcc -m32 task1.o driver.o asm_io.o -o task1

task1_2.o: task1_2.asm
	nasm -f elf task1_2.asm -o task1_2.o

task1_2: task1_2.o driver.o asm_io.o
	gcc -m32 task1_2.o driver.o asm_io.o -o task1_2

#run the files using the same rules as before to complete task 2 
task2_1.o: task2_1.asm
	nasm -f elf task2_1.asm -o task2_1.o

task2_1: task2_1.o driver.o asm_io.o
	gcc -m32 task2_1.o driver.o asm_io.o -o task2_1

task2_2.o: task2_2.asm
	nasm -f elf task2_2.asm -o task2_2.o

task2_2: task2_2.o driver.o asm_io.o
	gcc -m32 task2_2.o driver.o asm_io.o -o task2_2

task2_3.o: task2_3.asm
	nasm -f elf task2_3.asm -o task2_3.o

task2_3: task2_3.o driver.o asm_io.o
	gcc -m32 task2_3.o driver.o asm_io.o -o task2_3

#runs rm -f to remove all .o files (compiled files) to rebuild if needs be
clean:
	rm -f *.o task1 task1_2 task2_1 task2_2 task2_3