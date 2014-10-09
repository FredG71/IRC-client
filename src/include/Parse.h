#pragma once
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <tchar.h>
#include <Windows.h>

typedef struct __Memblock{
	void* __Block;
	char szRetBuffer[1024];
	uint32_t nBlockSize;
};
extern struct __Memblock Memblock;

void* Parse_StringBlock(uint8_t bZero, uint32_t nSize);
char* Substring(char* szString, char* szBegin, uint32_t nBeginOffset);
char* SubStringRange(char* szString, char* szBegin, char* szEnd, uint32_t nBeginOffset);
char* NumSubstring(char* szString, uint32_t nBegin, uint32_t nBeginOffset);
char* NumRangeSubstring(char* szString, uint32_t nBegin, uint32_t nEnd, uint32_t nBeginOffset);
char* Strtrim(char* szString, char* szEnd);
char* StrtrimNum(char* szString, uint32_t nBegin);
char* Strcat(char* szString, uint32_t nDstSize, char* szCat);
char* RemChar(char* szString, char* szInChar);
char* AddBrackets(char* szIn, char* szOut);
char* GetNick(char* szIn);
char* GetUser(char* szIn);
char* IRCParse_ChannelMessage(char* szString);
char* IRCParse_PrivateMessage(char* szString);