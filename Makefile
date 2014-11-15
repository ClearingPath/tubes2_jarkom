all:
	g++ -std=c++11 -o server server.cpp -pthread -Wl,--no-as-needed
	g++ -std=c++11 -o client client.cpp -pthread -Wl,--no-as-needed
	
runserver:
	./server
	
runclient:
	./client