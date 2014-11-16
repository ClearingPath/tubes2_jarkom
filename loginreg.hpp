#ifndef loginreg_H
#define loginreg_H

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include "boolean.h"

using namespace std;


void start();
void signup();
void login();
bool checkUserName(string un);
bool checkPass(string p);
void writeExt(string un, string pass);
vector< vector <string> > readExt(string path);
void mainChat(string user);
void logout(string username);
void sendMessage(string str);
void createGroup(string str);
void joinGroup(string str);
void leaveGroup(string str);
void showChat(string str);
void exit();
string sendToPort(string str);
#endif