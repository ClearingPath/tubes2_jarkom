/**
	server: menerima bilangan n dari klien, mengembalikan n + 1 ke klien
	disarankan untuk membungkus address dan socket ke kelas tersendiri
*/

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

//deklarasi fungsi dan prosedur
void openSocket(int port);
void recieveConnection();
void sendMessage();
void producer();
void consumer();
void cleanTrhead();

//deklarasi global variable
queue<int> cid;
int token = 0;
bool all,producing;
int sock, client_sock;
char buffer[10];
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;
int len, n;
mutex lock;
vector<thread> line

int main(){
	
	// buka socket TCP (SOCK_STREAM) dengan alamat IPv4 dan protocol IP
	if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0){
		close(sock);
		printf("Cannot open socket\n");
		exit(1);
	}
	
	// ganti opsi socket agar dapat di-reuse addressnya
	int yes = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		perror("Cannot set option\n");
		exit(1);
	}
	
	// buat address untuk server
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; // semua address pada network interface
	serv_addr.sin_port = htons(PORT); // port 9000
	
	// bind server ke address
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		close(sock);
		printf("Cannot bind socket\n");
		exit(1);
	}
	
	listen(sock,5); // listen ke maksimal 5 koneksi klien
	
	all = true;

	thread p(producer);

	thread c(recieveConnection);

	p.join();
	c.join();

	int terminate;

	scanf ("%d",&terminate);

	if (terminate == 0){
		all = false;
	}
	p.join();
	c.join();
	close(sock);
	return 0;
}

void producer(){
	while(all){

		lock.lock(); // ambil lock
		printf ("Current token = %d\n",token);
		// Jika token masih kurang dari 5
		if (cid.size() < 5)
		{
			producing = true;
			cid.push(token);
			token++;
		}
		else{
			producing = false;
		}

		lock.unlock(); // lepas lock
		
		// sleep selama 0 - 999 milisekon (0 - 999000 nanosekon)
		usleep(50 * 1000);
	}
}

void recieveConnection(){
	while(all){
		if ((!producing) && (token > 0)){
			lock.lock();
			cid.pop();
			// terima koneksi dari klien
			clilen = sizeof(cli_addr);
			client_sock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
			
			// kosongkan buffer
			bzero(buffer, 10);
			
			// baca dari klien
			len = read(client_sock, buffer, 10);
			
			if (len >= 0){ // jika pembacaan berhasil
				n = atoi(buffer); // convert ke integer
				printf("Recv : %s\n",buffer);
				
				if (strcmp("nopassword",buffer) == 0){
					write(client_sock,"1",10); // tulis ke klien
				}
				else
				{
					write(client_sock,"0",10);
				}
			}
			lock.unlock();
			// tutup koneksi klien
			close(client_sock);
		}
		usleep (50 * 1000);
	}
}

void cleanTrhead(){
	while (all){
		if (line.size() > 100){
			lock.lock();
			line.clear();
			lock.unlock();
		}
		usleep(30 * 1000 * 1000);
	}
}