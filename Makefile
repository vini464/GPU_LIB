source_game:
	gcc -c ./src/lib/*.o
	mv *.o ./obj/

build:
	as -g -o ./gpu.o ./src/lib/gpu.s
	mv gpu.o ./obj/gpu.o

run: build 
	gcc ./src/jarvis.c -o run ./obj/gpu.o

run_game: source_game build
	gcc ./src/main.c -o tetriz ./obj/*.o

play:
	sudo ./tetriz

