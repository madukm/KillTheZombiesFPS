all: run

run:
	g++ -g server.cpp -std=c++17 -pthread -o server -fsanitize=address
	./server

debug:
	g++ -g server.cpp -pthread -o server
	gdb ./server
