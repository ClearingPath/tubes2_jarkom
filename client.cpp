/**
	klien: mengirimkan bilangan n ke server, menerima n + 1 dari server
	disarankan untuk membungkus address dan socket ke kelas tersendiri
*/

#include "includes.hpp"
#include "client.hpp"

using namespace std;

int sock, len; char buffer[1000];
bool all;

int main(int argc, char** argv){
	// penggunaan: ./client <server ip> <nilai n>
	if (argc != 2){
		printf("Pemakaian: ./client <server ip>\n");
	}
	else{
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
		serv_addr.sin_port = htons(PORT);
		
		// connect ke server, jika error keluar
		if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) exit(1);
		
		start();
		
		close(sock);
	}
	return 0;
}

void start(){
	string str; 
	bool correct;
	correct = false;
	cout << "*************************************" <<endl;
	cout << "     Welcome to LINKER MESSENGER  " << endl;
	cout << "*************************************" << endl;	
	cout << "Type signup to register" << endl;
	cout << "               or" << endl;
	cout << "Type login if you already registered" << endl;
	cout << "------------------------------------" << endl;
	while (!correct){
		cout << endl << ">  ";
		cin >> str;
		if(str.compare("register")==0){
			registering();
			correct = true;
			start();
		}else{
			if(str.compare("login")==0){
				login();
				correct = true;
			}else{
				cout << endl << endl << "Wrong input" <<endl;
			}
		}
	}
}

void registering(){
	string username;
	string pass;
	bool correct;
	correct=false;
	cout << "enter new username : ";
	cin >> username;
	cout << "enter password     : "; 
	cin >> pass;
	while(!correct){
		if(checkUserName(username)){
			correct=true;
		}else{
			cout << "username has already been used" << endl<<endl;
			cout << "enter new username : ";
			cin >> username;
			cout << "enter password     : "; 
			cin >> pass;
		}
	}
}

void login(){
	string username;
	string pass;
	bool correct;
	correct=false;

	while(!correct){
		cout << "username  : ";
		cin >> username;
		cout << "password  : ";	
		cin >> pass;
		if(checkPass(pass)){
			cout << endl << "---------------WELCOME " <<  username << "!----------------" << endl;
			mainChat(username);
			correct=true;
		}
	}
}

void mainChat(string user){
	string str;
	int status;
	status=1; //0 if logout

	while(status==1){
		cout << endl;
		cout << "--------------------------------------------------" << endl;
		cout << "MENU : " << endl;
		cout << "1. type message <destination> to send message" << endl;
		cout << "2. type create <group_name> to create a group" << endl;
		cout << "3. type join <group_name> to join a group" << endl;
		cout << "4. type leave <group_name> to leave a group" << endl; 
		cout << "5. type show <friend's_name or group_name> to show chat history" <<endl;
		cout << "6. type logout to exit the application" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << endl;
		cout << user << "> ";
		getline(cin >> ws,str);

		//logout
		if(str.compare("logout")==0){
			status=0;
			logout();
		}else{
		//message
			if (str.substr(0,8).compare("message ")==0){
				sendMessageTo(str);
			}else{
		//create group
				if(str.substr(0,7).compare("create ")==0){
					createGroup(str);
				}else{
		//join group
					if(str.substr(0,5).compare("join ")==0){
						joinGroup(str);
					}else{
		//leave group
						if(str.substr(0,6).compare("leave ")==0){
							leaveGroup(str);
						}else{
		//show chat history
							if(str.substr(0,5).compare("show ")==0){
								showChat(str);
							}else{
		//empty input
								if(str.compare("")==0){
									//do nothing
									cout << "kosongg" <<endl;
								}else{
		//wrong input
									cout << "Wrong input";									
								}
							}
						}
					}
				}
			}
		}
	}
}

bool checkUserName(string un){
	if(un.compare("")==0)
		return false;
	else
		return true;
}

bool checkPass(string p){
	if(p.compare("")==0)
		return false;
	else
		return true;
}

void logout(){
	cout << "THANK YOU!" << endl;
} 	

void sendMessageTo(string str){
	string receiver;
	string msg;
	receiver=str.substr(8);	
	cout << "kirim ke : " << receiver <<  endl; //nanti dihapus
	cout << "Message : " << endl;
	getline(cin,msg);
	sendMessage(msg);
}

void sendMessage(string message){
	cout << endl<< "message sent!" << endl;
	cout << "pesan yang dikirim : " << endl;
	cout << message << endl;
}

void createGroup(string str){
	string groupname;
	groupname=str.substr(7);
	cout << " bikin grup : " << str << endl;
	//masukkin ke file eksternal 
	ofstream myfile;
	myfile.open("group.txt", ios_base::app);
	myfile << groupname;
	myfile << "&&";
	myfile << endl;
	myfile.close();
	cout << endl << "The group has been created!" << endl;
}

void joinGroup(string str){
	string groupname;
	groupname=str.substr(5);
	cout << " join grup : " << groupname << endl;

}

void leaveGroup(string str){
	string groupname;
	groupname=str.substr(6);
	cout << " keluar grup : " << groupname << endl;

}

void showChat(string str){
	string destination;
	destination=str.substr(5);
	cout << " tampilin chat : " << destination << endl;

}
