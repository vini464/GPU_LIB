build:
	as -g -o ./gpu.o ./src/lib/gpu.s
	mv gpu.o ./obj/gpu.o

run: build 
	gcc ./src/jarvis.c -o run ./obj/gpu.o


