#include "stdafx.h"
#include "ClientType.h"


ClientType::ClientType()
{
	this->iResult = InitWinSock::GetResult();
	addrinfor.result = NULL;
	addrinfor.ptr = NULL;

	ZeroMemory(&addrinfor.hints, sizeof(addrinfor.hints));
	this->addrinfor.hints.ai_family = AF_UNSPEC;
	this->addrinfor.hints.ai_socktype = SOCK_STREAM;
	this->addrinfor.hints.ai_protocol = IPPROTO_TCP;

	this->iResult = getaddrinfo(static_cast<PCSTR>(IP_ADDRESS), DEFAULT_PORT, &this->addrinfor.hints, &this->addrinfor.result);

	AttempToConnect(this->addrinfor.result, this->addrinfor.ptr, this->_client, this->iResult);
}

void ClientType::CloseConnect()
{
	closesocket(this->_client);
	WSACleanup();
}

void ClientType::SendMsg()
{
	string msg = "";
	getline(cin, msg);

	char* sendbuf = new char[DEFAULT_BUFFER];

	this->iResult = send(this->_client, sendbuf, (int)strlen(sendbuf), 0);

	if (this->iResult == SOCKET_ERROR) {
		cout << "Send failed with error: "<<WSAGetLastError()<<endl;
		WSACleanup();
		return;
	}
	cout << msg << endl;
	
	this->iResult = shutdown(this->_client, SD_SEND);
	if (this->iResult == SOCKET_ERROR) {
		cout << "Shutdown failed with error" << WSAGetLastError();
		CloseConnect();
		return;
	}
}

void ClientType::ReceiveMsg()
{
	char *recvbuf = new char[DEFAULT_BUFFER];
	int recvlen = DEFAULT_BUFFER;
	while (true) {
		this->iResult = recv(_client, recvbuf, recvlen, 0);
		if (this->iResult > 0) {
			string msg = recvbuf;
			cout << msg << endl;
		}
		else if (this->iResult == 0) {
			cout << "Connection closed\n";
			break;
		}
		else {
			cout << "recv failed with error: " << WSAGetLastError() << endl;
			WSACleanup();
			break;
		}
	}

	CloseConnect();
}


ClientType::~ClientType()
{
}

bool CheckResult(int result)
{
	if (result!=0) {
		cout << "getaddrinfo() failed with error: " << result << endl;
		return false;
	}
	return true;
}

bool ClientType::AttempToConnect(addrinfo * &result, addrinfo * &ptr, SOCKET& client, int& Result)
{
	if (CheckResult(this->iResult)) {
		for (this->addrinfor.ptr = this->addrinfor.result; this->addrinfor.ptr != NULL; this->addrinfor.ptr = this->addrinfor.ptr->ai_next) {
			client = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (client == INVALID_SOCKET) {
				cout << "Socket failed with error: " << WSAGetLastError() << endl;
				WSACleanup();
				return 0;
			}

			//Connect to server
			Result = connect(client, ptr->ai_addr, (int)ptr->ai_addrlen);

			if (Result == SOCKET_ERROR) {
				closesocket(client);
				client = INVALID_SOCKET;
				continue;
			}
			break;
		}
	}
	freeaddrinfo(result);

	if (client == INVALID_SOCKET) {
		cout << "Unable to connect to server!\n";
		WSACleanup();
		return 0;
	}
	else {
		thread _thrReceive(&ClientType::ReceiveMsg);
		_thrReceive.join();
	}
	return 1;
}


