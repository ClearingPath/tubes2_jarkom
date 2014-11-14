all:
	g++ -std=c++11 -o server server.cpp -pthread -Wl,--no-as-needed
	g++ -o client client.cpp
	
runserver:
	./server
	
runclient:
	./client