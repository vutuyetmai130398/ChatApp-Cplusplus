#pragma once

#include"stdafx.h"
#include<winsock2.h>
#include<WS2tcpip.h>
#include<stdlib.h>
#include<iphlpapi.h>
#include<iostream>
using namespace std;

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "Ws2_32.lib")

class InitState
{
private:
	WSAData wsaData;
	int iResult;
public:
	InitState();
	bool CheckResult();
	int GetResult();
	~InitState();
};

