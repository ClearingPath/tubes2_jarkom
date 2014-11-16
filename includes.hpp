#ifndef includes_hpp
#define includes_hpp

#include <sys/types.h>   // tipe data penting untuk sys/socket.h dan netinet/in.h
#include <netinet/in.h>  // fungsi dan struct internet address
#include <sys/socket.h>  // fungsi dan struct socket API
#include <netdb.h>       // lookup domain/DNS hostname
#include <errno.h>
#include <cstring>
#include <string>
#include <iostream>
// thread & mutex
#include <thread>
#include <mutex>

// queue & vector
#include <queue>
#include <vector>

// untuk printf
#include <cstdio>

// untuk random, usleep, dan time
#include <cstdlib>
#include <unistd.h>
#include <time.h>

using namespace std;

typedef struct {
	int id;
	string user;
	string password;
}clientID;

#define PORT 25000

#endif