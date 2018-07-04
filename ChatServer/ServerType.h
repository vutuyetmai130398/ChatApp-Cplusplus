#pragma once

#include"InitState.h"
#include<thread>
#include<vector>
#include<string>
#include<iostream>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "9999"
#define MAX_CLIENT 10

using namespace std;

struct add_info
{
	addrinfo *result;
	addrinfo *ptr;
	addrinfo hints;
};

class ServerType:InitState
{
private:
	add_info Add_Info;
	SOCKET _server;
	vector<SOCKET> _clientList;
	int iResult;
	void CreateSocket();
	thread ListenClient;
public:
	ServerType();
	void SetupServer();
	bool CheckResult();
	friend bool CheckServer(addrinfo*&, SOCKET);
	friend void CheckError(addrinfo*);
	friend void Checkerrorsocket(SOCKET&);
	void SetupTCPListening();
	void HandleClientOrReciveMsg(SOCKET);
	void SendMsgToClient(string, SOCKET);
	void CloseConnection(SOCKET);
	~ServerType();
};

