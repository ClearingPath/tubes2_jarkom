#ifndef includes_hpp
#define includes_hpp

#include <sys/types.h>   // tipe data penting untuk sys/socket.h dan netinet/in.h
#include <netinet/in.h>  // fungsi dan struct internet address
#include <sys/socket.h>  // fungsi dan struct socket API
#include <netdb.h>       // lookup domain/DNS hostname
#include <errno.h>
#include <cstring>
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

using std::queue;
using std::thread;
using std::mutex;
using std::vector;

#define PORT 25000

#endif