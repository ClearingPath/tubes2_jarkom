#ifndef loginreg_H
#define loginreg_H

#include <iostream>
#include <fstream>
#include "boolean.h"

using namespace std;


void start();
void registering();
void login();
bool checkUserName(string un);
bool checkPass(string p);
void mainChat(string user);
void logout();
void sendMessageTo(string str);
void sendMessage(string message);
void createGroup(string str);
void joinGroup(string str);
void leaveGroup(string str);
void showChat(string str);

#endif