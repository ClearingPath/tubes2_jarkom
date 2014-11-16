#include "loginreg.hpp"

void start(){
	string str; 
	bool correct;
	correct = false;
	cout << "*************************************" <<endl;
	cout << "     Welcome to LINKER MESSENGER  " << endl;
	cout << "*************************************" << endl;	
	cout << "1. Type signup to register" << endl;
	cout << "2. Type login if you already registered" << endl;
	cout << "3. Type exit to exit the application" << endl;
	cout << "------------------------------------" << endl;
	while (!correct){
		cout << endl << ">  ";
		cin >> str;
		if(str.compare("signup")==0){
			signup();
			correct = true;
			start();
		}
		else if(str.compare("login")==0){
			login();
			correct = true;
		}
		else if(str.compare("exit")==0){
			correct=true;
			exit();
		}
		else {
			cout << "Wrong input!" <<endl<<endl;
		}
	}
}

void signup(){
	string username;
	string pass;
	string ret;
	bool correct;
	correct=false;
	cout << "enter new username : ";
	getline(cin >> ws,username);
	cout << "enter password     : "; 
	getline(cin >> ws,pass);
	while(!correct){

		if(checkUserName(username)){
			correct=true;
		}else{
			cout << "username has already been used" << endl<<endl;
			cout << "enter new username : ";
			getline(cin >> ws,username);
			cout << "enter password     : "; 
			getline(cin >> ws,username);
		}
	}
	ret.append("REG||");
	ret.append(username);
	ret.append("||");
	ret.append(pass);
	ret.append("||");
	sendToPort(ret);
	//masukkin ke file eksternal 
	writeExt(username,pass);
	cout << endl << "You've already registered!" << endl <<endl;
}

void writeExt(string un, string pass){
	ofstream myfile;
	myfile.open("user.txt", ios_base::app);
	myfile << un;
	myfile << "||";
	myfile << pass;
	myfile << endl;
	myfile.close();
}


vector< vector <string> > readExt(string path){
	vector< vector <string> > userVector;
	vector<string> namepasswordVector;
	int found, nline;
	string line;
	string name,pass;
	ifstream myfile(path.c_str());
	if (myfile.is_open()){
		while (getline(myfile,line)){
			cout << line << endl;
			found=line.find("||");
			name=line.substr(0,found);
			pass=line.substr(found+2);
			cout << "nama : " << name << endl;
			cout << "pass : " << pass << endl;
			//masukkin ke vector
			namepasswordVector.clear();
			namepasswordVector.push_back(name);
			namepasswordVector.push_back(pass);
			userVector.push_back(namepasswordVector);

		}		
		myfile.close();
	}
	else cout << "unable to open file";
	return userVector;
}


void login(){
	string username;
	string pass;
	string ret;
	bool correct;
	correct=false;

	while(!correct){
		cout << "username  : ";
		getline(cin >> ws,username);
		cout << "password  : ";	
		getline(cin >> ws,pass);
		if(checkPass(pass)){
			ret.append("LOU||");
			ret.append(username);
			ret.append("||");
			cout << "return : " << ret << endl;
			cout << endl << "---------------WELCOME " <<  username << "!----------------" << endl;
			mainChat(username);
			correct=true;
		}
	}
}

void mainChat(string user){
	string str;
	string username = user;
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
		cout << "6. type logout to sign out from the application" << endl;
		cout << "7. type exit to exit the application" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << endl;
		cout << user << "> ";
		getline(cin >> ws,str);

		//logout
		if(str.compare("logout")==0){
			status=0;
			logout(username);
		}
		//message
		else if(str.substr(0,8).compare("message ")==0){
			sendMessage(str);
		}
		//create group
		else if(str.substr(0,7).compare("create ")==0){
			createGroup(str);
		}
		//join group
		else if(str.substr(0,5).compare("join ")==0){
			joinGroup(str);
		}
		//leave group
		else if(str.substr(0,6).compare("leave ")==0){
			leaveGroup(str);
		}
		//show chat history
		else if(str.substr(0,5).compare("show ")==0){
			showChat(str);
		}
		//exit
		else if(str.compare("exit")==0){
			status=0;
			exit();
		}
		//empty input
		else if(str.compare("")==0){
			//do nothing
		}
		//wrong input
		else{
			cout <<"Wrong input!" <<endl;
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

void logout(string username){
	string ret;
	ret.append("LOU||");
	ret.append(username);
	ret.append("||");
	cout << "Logout success!" << endl;
	sendToPort(ret);
	start();
} 	

void sendMessage(string str){
	string target;
	string msg;
	string ret;
	string clientTime;
	time_t currentTime;
	target=str.substr(8);	
	cout << "kirim ke : " << target <<  endl; //nanti dihapus
	cout << "Message : " << endl;
	getline(cin,msg);
	time(&currentTime);
	clientTime=ctime(&currentTime);
	ret.append("MSG||");
	ret.append(target);
	ret.append("||");
	ret.append(clientTime);//client time
	ret.append("||");
	ret.append(msg);
	ret.append("||");
	sendToPort(ret);
}

void createGroup(string str){
	string groupname;
	string ret;
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
	ret.append("CGR||");
	ret.append(groupname);
	ret.append("||");
	sendToPort(ret);
}

void joinGroup(string str){
	string groupname;
	string ret;
	groupname=str.substr(5);
	cout << " join grup : " << groupname << endl;
	ret.append("JGR||");
	ret.append(groupname);
	ret.append("||");
	sendToPort(ret);
}

void leaveGroup(string str){
	string groupname;
	string ret;
	groupname=str.substr(6);
	cout << " keluar grup : " << groupname << endl;
	ret.append("LGR||");
	ret.append(groupname);
	ret.append("||");
	sendToPort(ret);
}

void showChat(string str){
	string destination;
	destination=str.substr(5);
	cout << " tampilin chat : " << destination << endl;
}

void exit(){
	cout << "THANK YOU!" <<endl;
}

string sendToPort(string str){
	//kirim ke port
	cout << "return : " << str << endl;
	return str;
}

int main(){
	string user;
	string path = "user.txt";
	vector< vector<string> > hasil;
	// start();
	hasil = readExt(path);
	cout << hasil[1][1];
	return 0;
}