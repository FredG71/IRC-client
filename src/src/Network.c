#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Window.h"
#include "Parse.h"
#include "IRC.h"
#pragma comment( lib, "Ws2_32.lib")

char szNick[96] = "sfhisfde";
char szHost[96] = "irc.quakenet.org";
char szPort[96] = "6667";
char szUser[96] = "Garner";
char szChannel[96];
char Buffer[512];
SOCKET Socket;

void Send(char* szFormat, ...)
{
	assert(szFormat);
	va_list pList;
	va_start(pList, szFormat);
	vsnprintf_s(Buffer, 512, 511, szFormat, pList);
	va_end(pList);
	assert(send(Socket, Buffer, strlen(Buffer), 0) != -1);
}

char* Parse(char* szString)
{
	assert(szString);
	SetTime();

	if (strstr(szString, "JOIN"))
		return IRCParse_OnJoin(szString);

	if (strstr(szString, "NOTICE AUTH"))
		return IRCParse_ParseAuthNotice(szString);

	if (strstr(szString, "PING"))
		return "\0";

	if (strstr(szString, "001") || strstr(szString, "002") || strstr(szString, "003"))
		return IRCParse_OnWelcome(szString);

	if (strstr(szString, "004") || strstr(szString, "005"))
		return IRCParse_OnBounceInfo(szString);

	if (strstr(szString, "251") || strstr(szString, "255"))
		return IRCParse_OnLUserClient(szString);

	if (strstr(szString, "252") || strstr(szString, "253") || strstr(szString, "254"))
	{
		if (!strstr(szString, "MODE"))
			return IRCParse_OnUserCommands(szString);
	}
	if (strstr(szString, "NOTICE"))
		return IRCParse_OnNotice(szString);

	if (strstr(szString, "372") || strstr(szString, "376") || strstr(szString, "375"))
		return IRCParse_OnMOTD(szString);

	if (strstr(szString, "221"))
		return IRCParse_OnMode(szString);
	if (strstr(szString, "MODE"))
		return IRCParse_OnHostmask(szString);

	if (strstr(szString, "PRIVMSG"))
	{
		if (strstr(szString, "#"))
			return IRCParse_ChannelMessage(szString);
		else
			return IRCParse_PrivateMessage(szString);
	}

	return szString;
}

DWORD WINAPI Connect(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);
	struct addrinfo __Hints, *pRes;
	WSADATA WsaData;
	char __Buffer[512];
	int i, sl;
	int o = -1; int l;

	ZeroMemory(&WsaData, sizeof(WSADATA));
	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0)
		assert(0);
	ZeroMemory(&__Hints, sizeof(__Hints));
	__Hints.ai_family = AF_INET;
	__Hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(szHost, szPort, &__Hints, &pRes) != 0)
		assert(0);
	Socket = socket(pRes->ai_family, pRes->ai_socktype, pRes->ai_protocol);
	if (Socket == INVALID_SOCKET)
		assert(0);
	if (connect(Socket, pRes->ai_addr, pRes->ai_addrlen) != 0)
		assert(0);

	Send("USER Garner 0 0 :Garner\r\n");
	Send("NICK %s\r\n", szNick);
	SetWindowText(ContextIRC.hCurrentWindow, "");

	while (sl = recv( Socket, __Buffer, 512, 0))
	{
		for (i = 0; i < sl; i++)
		{
			o++;
			Buffer[o] = __Buffer[i];
			if ((i > 0 && __Buffer[i] == '\n' && __Buffer[i - 1] == '\r') || o == 512)
			{
				Buffer[o + 1] = '\0';
				l = o;
				o = -1;
				SetIRCWindowText(Parse(Buffer));

				if (!strncmp(Buffer, "PING", 4))
				{
					Buffer[1] = 'O';
					Send(Buffer);
				}
			}
		}
	}
	return 0;
}