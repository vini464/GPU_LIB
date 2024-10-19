build:
	as -g -o gpu_functions.o gpu_functions.s
	as -g -o memory.o memory.s
	as -g -o test_sp.o test_sp.s
	as -g -o wbr_sp.o wbr_sp.s
link:
	ld -o run *.o 
	rm *.o

run: build link 
	sudo ./run
	
