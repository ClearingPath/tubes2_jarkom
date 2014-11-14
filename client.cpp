/**
	klien: mengirimkan bilangan n ke server, menerima n + 1 dari server
	disarankan untuk membungkus address dan socket ke kelas tersendiri
*/

#include <cstdio>
#include <sys/types.h>   // tipe data penting untuk sys/socket.h dan netinet/in.h
#include <netinet/in.h>  // fungsi dan struct internet address
#include <sys/socket.h>  // fungsi dan struct socket API
#include <netdb.h>       // lookup domain/DNS hostname
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <string>
#include <iostream>
#define port 25000

using namespace std;
int sock, len; char buffer[10];
void login();

int main(int argc, char** argv){
	// penggunaan: ./client <server ip> <nilai n>
	if (argc != 3){
		printf("Pemakaian: ./client <server ip> <nilai n>\n");
	}
	
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	// buka socket TCP (SOCK_STREAM) dengan alamat IPv4 dan protocol IP
	if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0){
		close(sock);
		printf("Cannot open socket\n");
		exit(1);
	}
	
	// buat address server
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"Host not found\n");
		exit(1);
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);
	
	// connect ke server, jika error keluar
	if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) exit(1);
	
	// copy n ke buffer
	bcopy(argv[2], buffer, 10);
	
	login();
	
	close(sock);
	return 0;
}

void login(){
	string user;
	cout << endl << "Masukan id :" << endl;
	cin >> user;
	
	strcpy (buffer, user.c_str());
	len = write(sock,buffer,16);
	if (len >= 0){
		len = read(sock,buffer,16);
		if (len >= 0){
			printf("%s\n",buffer);
		}
	}
}
