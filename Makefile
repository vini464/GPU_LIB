build:
	as -g -o ./obj/gpu.o ./src/lib/gpu.s

run: build 
	gcc ./src/jarvis.c -o run ./obj/*.o


