build:
	as -g -o ./obj/gpu.o ./src/lib/gpu.s

run: build 
	gcc ./src/test.c -o run ./obj/*.o


