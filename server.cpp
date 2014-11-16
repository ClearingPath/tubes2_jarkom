#include "includes.hpp"

//deklarasi fungsi dan prosedur
void recieveConnection();
void sendMessage();
void producer();
void clientHandle(int client_sock,int cid);
void clientMessageHandle(int client_sock,string message);
void userLogin(int client_sock,vector<string> & message);
void userLogout(int client_sock,vector<string> & message);
void userRegister(int client_sock,vector<string> & message);
void sendMessage(int client_sock,vector<string> & message);
void createGroup(int client_sock,vector<string> & message);
void joinGroup(int client_sock,vector<string> & message);
void leaveGroup(int client_sock,vector<string> & message);
void errorReply(int client_sock,string code,string message);
void successReply(int client_sock);
bool active(int cid);
void changeOfflineClient(int cid);
void stringChopper(string message,vector<string> & result);

//deklarasi global variable
vector<clientID> clientOnline;
vector<int> vtoken;
int token = 0;
bool all,producing;
int sock, client_sock;
char buffer[1000];
string stringBuffer;
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;
int len, n;
mutex kunci;
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
		kunci.lock(); // ambil lock
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

		kunci.unlock(); // lepas lock
		
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
			pool.push_back(thread(clientHandle,client_sock,1));
			vtoken.erase(vtoken.begin());
		}
		usleep (50 * 1000);
	}
}

void clientHandle(int client_sock,int cid){
	bool act=active(token);
	while (act){
		bzero(buffer,1000);
		len = read(client_sock,buffer,1000);
		if(len >= 0){
			printf("Message client %d = %s",client_sock,buffer);
			if (strcmp(buffer,"") == 0){
				changeOfflineClient(cid);
			}
			else{
				string temp = buffer;
				clientMessageHandle(client_sock,temp);
			}
			printf("\n");
		}
		act = active(token);
		usleep(50 * 1000);
	}
	close(client_sock);
}

void clientMessageHandle(int client_sock,string message){
	vector<string> choppedString;
	stringChopper(message,choppedString);
	if (choppedString[0].compare("LIN") == 0){
		userLogin(client_sock,choppedString);
	}
	else if (choppedString[0].compare("LOU") == 0){
		userLogout(client_sock,choppedString);
	}
	else if (choppedString[0].compare("REG") == 0){
		userRegister(client_sock,choppedString);
	} 
	else if (choppedString[0].compare("MSG") == 0){
		sendMessage(client_sock,choppedString);
	}
	else if (choppedString[0].compare("CGR") == 0){
		createGroup(client_sock,choppedString);
	} 
	else if (choppedString[0].compare("JGR") == 0){
		joinGroup(client_sock,choppedString);
	}
	else if (choppedString[0].compare("LGR") == 0){
		leaveGroup(client_sock,choppedString);
	}
	else{
		errorReply(client_sock,"000","Protocol error!");
	}
}

void userLogin(int client_sock,vector<string> &  message){
	//still stub
	clientID temp;
	temp.id = 1;
	temp.user = "sudibya";
	temp.password = "none";
	clientOnline.push_back(temp);
	successReply(client_sock);
}

void userLogout(int client_sock,vector<string> & message){
	//still stub
	successReply(client_sock);
}
void userRegister(int client_sock,vector<string> &  message){
	//still stub
	successReply(client_sock);
}
void sendMessage(int client_sock,vector<string> &  message){
	//still stub
	successReply(client_sock);
}
void createGroup(int client_sock,vector<string> &  message){
	//still stub
	successReply(client_sock);
}
void joinGroup(int client_sock,vector<string> &  message){
	//still stub
	successReply(client_sock);
}
void leaveGroup(int client_sock,vector<string> &  message){
	//still stub
	successReply(client_sock);
}
void errorReply(int client_sock,string code,string message){
	stringBuffer = "SVR||";
	stringBuffer += code;
	stringBuffer += "||";
	stringBuffer += message;
	stringBuffer += "||";
	bzero(buffer,1000);
	strcpy(buffer,stringBuffer.c_str());
	len = write(client_sock,buffer,1000);
}
void successReply(int client_sock){
	stringBuffer = "SVR||200||OK||";
	bzero(buffer,1000);
	strcpy(buffer,stringBuffer.c_str());
	len = write(client_sock,buffer,1000);
}
bool active(int cid){
	bool temp = false;
	int i = 0;
	while (i < clientOnline.size()){
		temp = temp || (clientOnline[i].id == cid);
		i++;
	}
	return temp;
}

void changeOfflineClient(int cid){
	bool found = false;
	int i = 0;
	while ((!found) && (i < clientOnline.size())){
		if (clientOnline[i].id == cid)
			found = true;
		else
			i++;
	}
	if (i != clientOnline.size()){
		clientOnline.erase(clientOnline.begin()+i);
	}
}

void stringChopper(string message,vector<string> & result){
	char temp[1000];
	char * cstr;
	cstr = new char [100];
	string stringTemp;
	strcpy(temp,message.c_str());
	cstr = strtok(temp,"||");
	while (cstr != 0){
		stringTemp = cstr;
		result.push_back(stringTemp);
		cstr = strtok(NULL,"||");
	}
}