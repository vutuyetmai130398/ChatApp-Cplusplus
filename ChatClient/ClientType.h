#pragma once
#include"InitWinSock.h"
#include<string>
#include<thread>
using namespace std;

#define IP_ADDRESS "192.168.56.1"
#define DEFAULT_PORT "9999"
#define DEFAULT_BUFFER 512

struct Addr_Info {
	addrinfo* result;
	addrinfo* ptr;
	addrinfo hints;
};

class ClientType:InitWinSock
{
private:
	SOCKET _client;
	Addr_Info addrinfor;
	int iResult;
public:
	ClientType();
	void CloseConnect();
	void SendMsg();
	void ReceiveMsg();
	friend bool CheckResult(int);
	bool AttempToConnect(addrinfo*&, addrinfo*&, SOCKET&, int&);
	~ClientType();
};

