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
	string user_ip = "", user_port = "";
	char base_ip[] = "127.0.0.1";
	char base_port[] = "27015";
	string choise = "";
	cout << "Если хотите ввести IP нажмите 1: "; cin >> choise;
	if (choise[0] == '1') { cout << "Введите Порт: "; cin >> user_ip; }
	else {};

	choise[0] = 0;
	cout << "Если хотите ввести Порт нажмите 1: "; cin >> choise;
	if (choise[0] == '1') { cout << "Введите Порт: "; cin >> user_port; }
	else {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if(user_ip == "" && user_port == "")iResult = getaddrinfo(base_ip, base_port, &hints, &target);
	else if (user_ip != "" && user_port == "")
	{
		char user_ip_2[256];
		strcpy(user_ip_2, user_ip.c_str());
		iResult = getaddrinfo(user_ip_2, base_port, &hints, &target);
	}
	else if (user_ip == "" && user_port != "")
	{
		char user_port_2[256];
		strcpy(user_port_2, user_port.c_str());
		iResult = getaddrinfo(base_ip, user_port_2, &hints, &target);
	}
	else if (user_ip != "" && user_port != "")
	{
		char user_ip_2[256];
		char user_port_2[256];
		strcpy(user_ip_2, user_ip.c_str());
		strcpy(user_port_2, user_port.c_str());
		iResult = getaddrinfo(user_ip_2, user_port_2, &hints, &target);
	}
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

