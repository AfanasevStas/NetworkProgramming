//Server
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iphlpapi.h>
#include <thread>
#include <vector>

#include<FormatLastError.h>
using namespace std;

#pragma comment(lib,"WS2_32.lib")
#pragma comment(lib, "FormatLastError.lib")
#define MTU 1500

void Client_dialog(SOCKET client_socket)
{
	CHAR send_buffer[MTU] = "Hello client!";
	INT iReseivedBytes = 0;
	INT iSentBytes = 0;
	DWORD dwError = 0;
	CHAR szError[256] = {};
	do
	{
		CHAR recv_buffer[MTU] = {};
		iReseivedBytes = recv(client_socket, recv_buffer, MTU, 0);
		if (iReseivedBytes > 0)
		{
			cout << "Received " << iReseivedBytes << " " << recv_buffer << endl;
			iSentBytes = send(client_socket, recv_buffer, strlen(send_buffer), 0);
			if (iSentBytes == SOCKET_ERROR)
			{
				dwError = WSAGetLastError();
				cout << FormatLastError(dwError, szError) << endl;
				//cout << "Send failed with error:\t" << WSAGetLastError() << endl;
			}
			else cout << iSentBytes << " Bytes sent" << endl;
		}
		else if (iReseivedBytes == 0) cout << "Connection closing..." << endl;
		else
		{
			dwError = WSAGetLastError();
			cout << FormatLastError(dwError, szError) << endl;
		}
		//cout << "Receive failed with error: " << WSAGetLastError() << endl;
	} while (iReseivedBytes > 0);
}

void main()
{
	setlocale(LC_ALL, "");
	cout << "Im - SERVER" << endl;
	DWORD dwError = 0;
	CHAR szError[256] = {};
	WSADATA wsaData;
	INT iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "WSAStartup failed with error: " << iResult << endl;
		return;
	}

	addrinfo hints;
	addrinfo* target;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, "27015", &hints, &target);
	if (iResult != 0)
	{
		cout << "getaddresinfo() failed with error: " << iResult << endl;
		freeaddrinfo(target);
		WSACleanup();
		return;
	}

	SOCKET listen_socket = socket(target->ai_family, target->ai_socktype, target->ai_protocol);
	dwError = WSAGetLastError();
	if (listen_socket == INVALID_SOCKET)
	{
		cout << FormatLastError(dwError, szError) << endl;
		//cout << "SOCKET creation failed with error: " << WSAGetLastError() << endl;
		freeaddrinfo(target);
		WSACleanup();
		return;
	}

	iResult = bind(listen_socket, target->ai_addr, target->ai_addrlen);
	dwError = WSAGetLastError();
	if (iResult != 0)
	{
		cout << FormatLastError(dwError, szError) << endl;
		//cout << "bind failed with error: " << WSAGetLastError() << endl;
		closesocket(listen_socket);
		freeaddrinfo(target);
		WSACleanup();
		return;
	}

	if (listen(listen_socket, 1) == SOCKET_ERROR)
	{
		dwError = WSAGetLastError();
		cout << FormatLastError(dwError, szError) << endl;
		//cout << "Listen failed with error: " << WSAGetLastError() << endl;
		closesocket(listen_socket);
		freeaddrinfo(target);
		WSACleanup();
		return;
	}


	SOCKADDR_IN client_addres;
	INT client_adres_len = sizeof(client_addres);
	SOCKET client_socket;
	vector<thread> clients;
	while (true)
	{
		client_socket = accept(listen_socket, (SOCKADDR*)&client_addres, &client_adres_len);
		if (client_socket == INVALID_SOCKET)
		{
			dwError = WSAGetLastError();
			cout << FormatLastError(dwError, szError) << endl;
			//cout << "Accept failed with error: " << WSAGetLastError() << endl;
			closesocket(listen_socket);
			freeaddrinfo(target);
			WSACleanup();
			return;
		}
		cout << inet_ntoa(client_addres.sin_addr) << ":" << ntohs(client_addres.sin_port) << endl;
		clients.emplace_back(Client_dialog, client_socket);
		clients.back().detach();
	}
	iResult = shutdown(client_socket, SD_BOTH);
	if (iResult == SOCKET_ERROR)
	{
		dwError = WSAGetLastError();
		cout << FormatLastError(dwError, szError) << endl;
		//cout << "Shutdown failed with error:\t" << WSAGetLastError() << endl;
	}
	system("PAUSE");
	closesocket(listen_socket);
	freeaddrinfo(target);
	WSACleanup();
}