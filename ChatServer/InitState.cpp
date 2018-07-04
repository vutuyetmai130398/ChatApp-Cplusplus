#include "stdafx.h"
#include "InitState.h"


InitState::InitState()
{
	//Init winsock
	//Call WSAStartUp return value as integer and check errors
	this->iResult = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
}

bool InitState::CheckResult()
{
	if (this->iResult != 0) {
		cout << "WSAStartup failed: " << this->iResult;
		return false;
	}

	return true;

}

int InitState::GetResult()
{
	return this->iResult;
}

InitState::~InitState()
{
}
