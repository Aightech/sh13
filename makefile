all: sh13 clean

sh13: bin/main.o bin/guilib.o bin/gamelib.o bin/netlib.o
	g++ -o sh13 bin/main.o bin/guilib.o bin/gamelib.o bin/netlib.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lpthread

bin/main.o: main.cpp src/netlib.hpp
	g++ -c main.cpp -o bin/main.o
	
bin/netlib.o: src/netlib.cpp src/netlib.hpp 
	g++ -c src/netlib.cpp -o bin/netlib.o
	
bin/guilib.o: src/guilib.cpp src/guilib.hpp
	g++ -c src/guilib.cpp -o bin/guilib.o
	
bin/gamelib.o: src/gamelib.cpp src/gamelib.hpp bin/netlib.o
	g++ -c src/gamelib.cpp -o bin/gamelib.o
	

	
clean:
	rm -f bin/*.o
	

