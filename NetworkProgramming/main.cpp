//Client
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iphlpapi.h>

#include<FormatLastError.h>
using namespace std;

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "FormatLastError.lib")
#define MTU 1500



void main()
{
	setlocale(LC_ALL, "");
	cout << "Im - CLIENT" << endl;
	DWORD dwError = 0;
	CHAR szError[256] = {};
	WSAData wsaData;
	int iResult = 0;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "WinSOCK init failed with code: " << iResult;
		return;
	}
	addrinfo hints;
	addrinfo* target;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	iResult = getaddrinfo("127.0.0.1", "27015", &hints, &target);
	if (iResult != 0)
	{
		cout << "getaddresinfo() failed with code" << iResult << endl;
		freeaddrinfo(target);
		WSACleanup();
		return;
	}

	SOCKET connect_socket = socket(target->ai_family, target->ai_socktype, target->ai_protocol);
	dwError = WSAGetLastError();
	if (connect_socket == INVALID_SOCKET)
	{
		//cout << "SOCKET creation failed with error:\t" << WSAGetLastError() << endl;
		cout << FormatLastError(dwError, szError) << endl;
		freeaddrinfo(target);
		WSACleanup();
		return;
	}

	iResult = connect(connect_socket, target->ai_addr, target->ai_addrlen);
	dwError = WSAGetLastError();
	freeaddrinfo(target);
	if (iResult == SOCKET_ERROR)
	{
		//cout << "Eror: " << dwError << ":\t";
		cout << FormatLastError(dwError, szError) << endl;
		//cout << lpError << endl;
		cout << "Unable to connect to server" << endl;
		closesocket(connect_socket);
		WSACleanup();
		return;
	}
	cout << "Для отправки сообщения ";
	system("PAUSE");
	CHAR send_buffer[MTU] = "Hello Server";

	iResult = send(connect_socket, send_buffer, strlen(send_buffer), 0);
	dwError = WSAGetLastError();
	if (iResult == SOCKET_ERROR)
	{
		//cout << "Send failed with error: " << WSAGetLastError() << endl;
		cout << FormatLastError(dwError, szError) << endl;
		closesocket(connect_socket);
		WSACleanup();
		return;
	}

	CHAR recv_buffer[MTU] = {};
	do
	{
		iResult = recv(connect_socket, recv_buffer, MTU, 0);
		dwError = WSAGetLastError();
		if (iResult > 0) cout << "Bytes received: " << iResult << "Message: " << recv_buffer << endl;
		else if (iResult == 0)cout << "Connection closed" << endl;
		else cout << FormatLastError(dwError, szError) << endl;
			//cout << "Recive failed with error " << WSAGetLastError() << endl;
	} while (iResult > 0);

	iResult = shutdown(connect_socket, SD_BOTH);
	if (iResult == SOCKET_ERROR)cout << FormatLastError(WSAGetLastError(), szError) << endl;
		//"Shotdown failed with error " << WSAGetLastError() << endl;
	closesocket(connect_socket);

	WSACleanup();
}

