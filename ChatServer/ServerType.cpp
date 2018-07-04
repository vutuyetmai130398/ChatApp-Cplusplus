#include"stdafx.h"
#include "ServerType.h"

void CheckError(addrinfo *res)
{
	cout << WSAGetLastError() << endl;
	freeaddrinfo(res);
	WSACleanup();
}


bool CheckServer(addrinfo* &res, SOCKET ser)
{
	if (ser == INVALID_SOCKET) {
		cout << "Error at socket(): ";
		CheckError(res);
		return true;
	}
	return false;
}

void Checkerrorsocket(SOCKET & ser)
{
	if (ser == INVALID_SOCKET) {
		cout << WSAGetLastError() << endl;
		closesocket(ser);
		WSACleanup();
	}
}


void ServerType::CreateSocket()
{
	this->_server = socket(this->Add_Info.result->ai_family, this->Add_Info.result->ai_socktype, this->Add_Info.result->ai_protocol);
	this->ListenClient=std::thread((&ServerType::SetupTCPListening, this));
}

ServerType::ServerType():InitState()
{
	if (InitState::CheckResult()) {
		this->iResult = InitState::GetResult();
		this->_server = INVALID_SOCKET;

		//Setup hints
		ZeroMemory(&this->Add_Info.hints, sizeof(this->Add_Info.hints));
		this->Add_Info.hints.ai_family = AF_INET;
		this->Add_Info.hints.ai_socktype = SOCK_STREAM;
		this->Add_Info.hints.ai_protocol = IPPROTO_TCP;
		this->Add_Info.hints.ai_flags = AI_PASSIVE;
	}

	this->SetupServer();
}


void ServerType::SetupServer()
{
	cout << "Setting Server...";
	this->iResult = getaddrinfo(NULL, DEFAULT_PORT, &this->Add_Info.hints, &this->Add_Info.result);
}

bool ServerType::CheckResult()
{
	if (this->iResult) {
		cout << "getaddrinfo failed with error: " << this->iResult;
		return false;
	}
	return true;
}



void ServerType::SetupTCPListening()
{
	SOCKET _client;

	//Listening clients
	if (CheckServer(this->Add_Info.result, this->_server) == 0) {
		bind(this->_server, this->Add_Info.result->ai_addr, (int)this->Add_Info.result->ai_addrlen);
	}
	freeaddrinfo(this->Add_Info.result);

	while (true)
	{
		this->iResult = listen(this->_server, SOMAXCONN);
		if (this->iResult == SOCKET_ERROR) {
			cout << "Listening failed at: ";
			Checkerrorsocket(this->_server);
			return;
		}

		//Accept clients
		_client = accept(this->_server, NULL, NULL);
		if (_client == INVALID_SOCKET) {
			cout << "Accept failed with error: ";
			Checkerrorsocket(this->_server);
		}
		else {
			this->_clientList.push_back(_client);
		}

		thread handleClient(&ServerType::HandleClientOrReciveMsg, _client);
		handleClient.join();
	}

}

void ServerType::HandleClientOrReciveMsg(SOCKET _client)
{
	char recbuf[DEFAULT_BUFLEN];
	int recbuflen;
	string msg = "";
	while (true)
	{
		//Receive bytes
		this->iResult = recv(_client, recbuf, recbuflen, 0);
		if (this->iResult > 0) {
			msg = "Client: " + *recbuf;
			this->SendMsgToClient(msg, _client);
		}
		else if (this->iResult == 0) {
			cout << "Connection Closing ... ";
			this->CloseConnection(_client);
		}
	}

	//Displays the msg which is received
	std::cout << msg << endl;
}

void ServerType::SendMsgToClient(string msg, SOCKET _client)
{
	std::vector<SOCKET>::iterator it = this->_clientList.begin();
	char*sendBuf = new char[DEFAULT_BUFLEN];

	while (*it!=NULL) {
		if(*it!=_client)
			this->iResult = send(*it, msg.c_str(), strlen(msg.c_str()), 0);
		if (this->iResult == SOCKET_ERROR) {
			cout << "Send failed: ";
			Checkerrorsocket(*it);
			return;
		}
		it++;
	}
}

void ServerType::CloseConnection(SOCKET _client)
{
	this->iResult = shutdown(_client, SD_SEND);
	if (this->iResult == SOCKET_ERROR) {
		cout << "Shutdown failed with error: ";
		Checkerrorsocket(_client);
		return;
	}

}

ServerType::~ServerType()
{
}
