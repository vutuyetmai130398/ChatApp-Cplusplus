#include "stdafx.h"
#include "InitWinSock.h"


InitWinSock::InitWinSock()
{
	//Init winsock
	//Call WSAStartUp return value as integer and check errors
	this->iResult = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
}

bool InitWinSock::Checkresult()
{
	if (this->iResult != 0) {
		cout << "WSAStartup failed with error: " << this->iResult;
		return false;
	}
	return true;
}

int InitWinSock::GetResult()
{
	return this->iResult;
}


InitWinSock::~InitWinSock()
{
}
