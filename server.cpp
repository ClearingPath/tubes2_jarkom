/**
	server: menerima bilangan n dari klien, mengembalikan n + 1 ke klien
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

int main(){
	int sock, client_sock;
	char buffer[10];
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int port = 9000, len, n;
	
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
	serv_addr.sin_port = htons(9000); // port 9000
	
	// bind server ke address
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		close(sock);
		printf("Cannot bind socket\n");
		exit(1);
	}
	
	listen(sock,5); // listen ke maksimal 5 koneksi klien
	
	while(1){
		// terima koneksi dari klien
		clilen = sizeof(cli_addr);
		client_sock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
		
		// kosongkan buffer
		bzero(buffer, 10);
		
		// baca dari klien
		len = read(client_sock, buffer, 10);
		
		if (len >= 0){ // jika pembacaan berhasil
			n = atoi(buffer); // convert ke integer
			printf("Recv : %d\n",n);
			
			sprintf(buffer, "%d", n + 1); // convert n + 1 ke char*
			write(client_sock,buffer,10); // tulis ke klien
		}
		
		// tutup koneksi klien
		close(client_sock);
	}
	close(sock);
	return 0;
}
