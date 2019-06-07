# Makefile simulador MIPS
CC=gcc
CFLAGS=-c -Wall

all: core aux
	$(CC) Core/Object/cpu.o Core/Object/instr_mem.o Core/Object/io.o Core/Object/ram.o Core/Object/register.o Auxiliar/Object/binary_ops.o Auxiliar/Object/custom_string_ops.o Auxiliar/Object/linked_list.o main.c -o app -lm

core: Core/cpu.c Core/instr_mem.c Core/io.c Core/ram.c Core/register.c
	mkdir Core/Object	
	$(CC) $(CFLAGS) Core/cpu.c -o Core/Object/cpu.o 
	$(CC) $(CFLAGS) Core/instr_mem.c -o Core/Object/instr_mem.o
	$(CC) $(CFLAGS) Core/io.c -o Core/Object/io.o
	$(CC) $(CFLAGS) Core/ram.c -o Core/Object/ram.o
	$(CC) $(CFLAGS) Core/register.c -o Core/Object/register.o

aux: Auxiliar/binary_ops.c Auxiliar/custom_string_ops.c Auxiliar/linked_list.c
	mkdir Auxiliar/Object	
	$(CC) $(CFLAGS) Auxiliar/binary_ops.c -o Auxiliar/Object/binary_ops.o
	$(CC) $(CFLAGS) Auxiliar/custom_string_ops.c -o Auxiliar/Object/custom_string_ops.o
	$(CC) $(CFLAGS) Auxiliar/linked_list.c -o Auxiliar/Object/linked_list.o

clear:
	rm -r Core/Object
	rm -r Auxiliar/Object
	rm app
