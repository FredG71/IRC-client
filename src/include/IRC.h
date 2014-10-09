#pragma once
#include <Windows.h>
#include <stdint.h>
char* GetChannel(char*);
void SetTime(void);
char* szBoyer_Moore(char*, int, char*, int);
void make_delta1(int32_t*, char*, int);
int is_prefix(char*, int, int);
int suffix_length(char*, int, int);
void make_delta2(int*, char*, int);

char* IRCParse_ParseAuthNotice(char*);
char* IRCParse_OnWelcome(char*);
char* IRCParse_OnBounceInfo(char*);
char* IRCParse_OnLUserClient(char*);
char* IRCParse_OnUserCommands(char*);
char* IRCParse_OnNotice(char*);
char* IRCParse_OnMOTD(char*);
char* IRCParse_OnMode(char*);
char* IRCParse_OnHostmask(char*);
char* IRCParse_OnJoin(char*);

extern SYSTEMTIME Time;
extern SOCKET Socket;
extern char ParseRetVal[512];
extern char szNick[96];
extern char szHost[96];
extern char szPort[96];
extern char szUser[96];
extern char szChannel[96];