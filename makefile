all: sh13 clean

sh13: bin/main.o bin/network.o  bin/guilib.o bin/gamelib.o
	g++ -o sh13 bin/main.o bin/network.o bin/guilib.o bin/gamelib.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

bin/main.o: main.cpp src/network.hpp
	g++ -c main.cpp -o bin/main.o

bin/network.o: src/network.cpp src/network.hpp
	g++ -c src/network.cpp -o bin/network.o
	
bin/guilib.o: src/guilib.cpp src/guilib.hpp
	g++ -c src/guilib.cpp -o bin/guilib.o
	
bin/gamelib.o: src/gamelib.cpp src/gamelib.hpp 
	g++ -c src/gamelib.cpp -o bin/gamelib.o
	
clean:
	rm -f bin/*.o
	

