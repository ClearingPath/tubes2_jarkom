#include "includes.hpp"

//deklarasi fungsi dan prosedur
void openSocket(int port);
void recieveConnection();
void sendMessage();
void producer();
void clientHandle(int client_sock,int token);
void guess();
bool active(int token);
void eraseVector(int number);

//deklarasi global variable
vector<int> cid;
vector<int> vtoken;
int token = 0;
bool all,producing;
int sock, client_sock;
char buffer[1000];
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;
int len, n;
mutex lock;
vector<thread> pool;

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

	pool.push_back(thread(recieveConnection));
	pool.back().join();
	p.join();

	//close(sock);
	close(sock);
	return 0;
}

void producer(){
	while(all){
		lock.lock(); // ambil lock
		// Jika token masih kurang dari 5
		if (vtoken.size() < 5)
		{
			producing = true;
			vtoken.push_back(token);
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
		if ((!producing) && (vtoken.size() > 0)){
			// terima koneksi dari klien
			clilen = sizeof(cli_addr);
			client_sock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
			printf ("Client %d Connected!\n",client_sock);
			cid.push_back(vtoken.front());
			pool.push_back(thread(clientHandle,client_sock,vtoken.front()));
			vtoken.erase(vtoken.begin());
		}
		usleep (50 * 1000);
	}
}

void clientHandle(int client_sock,int token){
	bool act=active(token);
	while (act){
		bzero(buffer,1000);
		len = read(client_sock,buffer,1000);
		if(len >= 0){
			printf("Message client %d = %s",client_sock,buffer);
			if (strcmp(buffer,"close") == 0){
				eraseVector(token);
			}
			else if (strcmp(buffer,"") == 0){
				printf ("SERVER DETECT : CLIENT %d DIED",client_sock);
				eraseVector(token);
			}
			printf("\n");
		}
		act = active(token);
		usleep(50 * 1000);
	}
	close(client_sock);
}


bool active(int token){
	bool temp = false;
	int i = 0;
	while (i < cid.size()){
		temp = temp || (cid[i] == token);
		i++;
	}
	return temp;
}

void eraseVector(int number){
	bool found = false;
	int i = 0;
	while (!found && (i < cid.size())){
		if (cid[i] == number){
			found = true;
		}
		else{
			i++;
		}
	}
	if (i != cid.size()){
		cid.erase(cid.begin()+i);
	}
}