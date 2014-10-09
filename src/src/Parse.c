#define _CRT_SECURE_NO_WARNINGS
#include "Parse.h"
#include "IRC.h"
#include "Window.h"

struct __Memblock Memblock;

/*@note Returns a block of memory to be used for parsing, note that this is always the same block, and thus can contain unpredictable data.
@param bZero: Should we zero initialize the array?
@param nSize: Size of the array
@return: A pointer to the allocated block, or the block itself if it's already been allocated*/
void* Parse_StringBlock(uint8_t bZero, uint32_t nSize)
{
	if (nSize > Memblock.nBlockSize && Memblock.__Block)
	{
		Memblock.__Block = realloc(Memblock.__Block, Memblock.nBlockSize * nSize);
		Memblock.nBlockSize = (Memblock.nBlockSize * nSize) - 1;
		if (bZero)
			ZeroMemory(Memblock.__Block, Memblock.nBlockSize);
		else
			return Memblock.__Block;
		return Memblock.__Block;
	}
	if (Memblock.__Block)
		return Memblock.__Block;
	else
	{
		if (nSize == 0)
		{
			Memblock.__Block = malloc(sizeof(void*)* 1024);
			Memblock.nBlockSize = sizeof(void*)* 1024 - 1;
		}
		else
		{
			Memblock.__Block = malloc(sizeof(void*)*nSize);
			Memblock.nBlockSize = sizeof(void*)*nSize - 1;
		}
		if (bZero && nSize >= 0)
			ZeroMemory(Memblock.__Block, sizeof(void*)*nSize);
		else if (bZero && nSize == 0)
			ZeroMemory(Memblock.__Block, sizeof(void*)* 1024);
		return Memblock.__Block;
	}
	assert(Memblock.__Block);
	return (void*)0x0;
}

/*@note: Finds the substring of szString by starting at szBegin and going to the end, nBeginOffset is used to shift
the string - see memmove_s
@param szString: Target string
@param szBegin: Where to start looking for
@param nBeginOffset: Offset to shift the string - useful if you want to remove the first character
@return: the substring*/
char* Substring(char* szString, char* szBegin, uint32_t nBeginOffset)
{
	assert(szString && szBegin);
	char* szMalloc = (char*)Parse_StringBlock(TRUE, strlen(szString));
	char* __Begin = strpbrk(szString, szBegin);
	assert(__Begin);
	memmove_s(szMalloc, Memblock.nBlockSize, __Begin, strlen(__Begin));
	if (nBeginOffset >= 0)
		memmove_s(szMalloc, Memblock.nBlockSize, &szMalloc[nBeginOffset], strlen(__Begin));
	strcpy_s(Memblock.szRetBuffer, 1023, szMalloc);
	ZeroMemory(Memblock.__Block, Memblock.nBlockSize + 1);
	return Memblock.szRetBuffer;
}
/*@note: Finds the substring of szString by starting at szBegin and ending at szEnd, nBeginOffset is used to shift
the string - see memmove_s
@param szString: Target string
@param szBegin: Where to start looking for
@param szEnd: Substring end
@param nBeginOffset: Offset to shift the string - useful if you want to remove the first character
@return: the substring*/
char* SubStringRange(char* szString, char* szBegin, char* szEnd, uint32_t nBeginOffset)
{
	assert(szString && szBegin && szEnd);
	char* __Begin = strpbrk(szString, szBegin);
	assert(__Begin);
	char* __End = strpbrk(__Begin, szEnd);
	char* szMalloc = (char*)Parse_StringBlock(TRUE, strlen(szString));
	assert(szMalloc && __End);
	memmove_s(szMalloc, Memblock.nBlockSize, __Begin, __End - __Begin);
	if (nBeginOffset >= 0)
		memmove_s(szMalloc, Memblock.nBlockSize, &szMalloc[nBeginOffset], strlen(szMalloc));
	strcpy_s(Memblock.szRetBuffer, 1023, szMalloc);
	ZeroMemory(Memblock.__Block, Memblock.nBlockSize + 1);
	return Memblock.szRetBuffer;
}

/*@note: Finds the substring of szString by starting nBegin and ending to the end of the string, unlike the one above, this uses integers to find
the position, nBeginOffset is used to shift the string - see memmove_s
@param szString: Target string
@param nBegin: Start position of the substring
@param nBeginOffset: Offset to shift the string - useful if you want to remove the first character
@return: the substring*/
char* NumSubstring(char* szString, uint32_t nBegin, uint32_t nBeginOffset)
{
	assert(szString && nBegin >= 0);
	if (nBegin > 0)
	{
		char* szMalloc = (char*)Parse_StringBlock(TRUE, strlen(szString));
		memmove_s(szMalloc, Memblock.nBlockSize, &szString[nBegin], strlen(szString) - nBegin);
		if (nBeginOffset >= 0)
			memmove_s(szMalloc, Memblock.nBlockSize, &szMalloc[nBeginOffset], strlen(szMalloc));
		strcpy_s(Memblock.szRetBuffer, 1013, szMalloc);
		ZeroMemory(Memblock.__Block, Memblock.nBlockSize + 1);
	}
	else
		strcpy_s(Memblock.szRetBuffer, 1023, szString);
	/*END ELSE*/
	return Memblock.szRetBuffer;
}
/*@note: Finds the substring of szString by starting nBegin and ending at nEnd, unlike the one above, this uses integers to find
the position, nBeginOffset is used to shift the string - see memmove_s
@param szString: Target string
@param nBegin: Start position of the substring
@param nBegin: End position of the substring
@param nBeginOffset: Offset to shift the string - useful if you want to remove the first character
@return: the substring*/
char* NumRangeSubstring(char* szString, uint32_t nBegin, uint32_t nEnd, uint32_t nBeginOffset)
{

	assert(nBegin < nEnd && szString && nBegin >= 0);
	if (nBegin == 0 && nEnd == 0)
		strcpy_s(Memblock.szRetBuffer, 1023, szString);
	/*END IF*/
	else
	{
		char* szMalloc = (char*)Parse_StringBlock(TRUE, strlen(szString));
		memmove_s(szMalloc, Memblock.nBlockSize, &szString[nBegin], nEnd - nBegin);
		if (nBeginOffset > 0)
			memmove_s(szMalloc, Memblock.nBlockSize, &szMalloc[nBeginOffset], strlen(szMalloc));
		strcpy_s(Memblock.szRetBuffer, 1023, szMalloc);
		ZeroMemory(Memblock.__Block, Memblock.nBlockSize + 1);
	}
	return Memblock.szRetBuffer;
}

/*@note: Trims the string from szEnd to the end
@param szString: Start string
@param szEnd: Where to start trimmming
@return: Trimmed string*/
char* Strtrim(char* szString, char* szEnd)
{
	assert(szString && szEnd);
	char* szMalloc = (char*)Parse_StringBlock(TRUE, strlen(szString));
	assert(szMalloc);
	char* __End = strpbrk(szString, szEnd);
	memmove_s(szMalloc, Memblock.nBlockSize, szString, __End - &szString[0]);
	strcpy_s(Memblock.szRetBuffer, 1023, szMalloc);
	ZeroMemory(Memblock.__Block, Memblock.nBlockSize + 1);
	return Memblock.szRetBuffer;
}

/*@note: Trims the string from nBegin to the end
@param szString: Start string
@param nBegin: Where to start trimmming
@return: Trimmed string*/
char* StrtrimNum(char* szString, uint32_t nBegin)
{
	assert(szString && nBegin < strlen(szString));
	char* szMalloc = (char*)Parse_StringBlock(TRUE, strlen(szString));
	assert(szMalloc);
	memmove_s(szMalloc, Memblock.nBlockSize, szString, nBegin);
	strcpy_s(Memblock.szRetBuffer, 1023, szMalloc);
	ZeroMemory(Memblock.__Block, Memblock.nBlockSize + 1);
	return Memblock.szRetBuffer;
}

/*@note: Concatenates two strings, this isn't actually used a lot
@param szString: Target string (needs to be large enough to hold szString + szCat)
@param nDstSize: Size of the destination string
@param szCat: String to be concatenated
@return: szString - the concatenated string*/
char* Strcat(char* szString, uint32_t nDstSize, char* szCat)
{
	assert(strlen(szCat) + 1 <= nDstSize && szString && szCat);
	memmove_s(&szString[strlen(szString)], strlen(szString) + strlen(szCat), szCat, strlen(szCat));
	return szString;
}

/*@note: Removes a character from the string, could use SubString(x, y, 1) for the first character though.
@param szString: Target string
@param szInChar: Target character to be removed (sent as a string) note tht multiple characters hasn't been tested" i.e. "RemChar(x, "ab") may return
undefined results
@return: Modified string*/
char* RemChar(char* szString, char* szInChar)
{
	assert(szString && szInChar);
	char* szTemp = strpbrk(szString, szInChar);
	assert(szTemp);
	char* szMalloc = malloc(sizeof(char*)*strlen(szString));
	ZeroMemory(szMalloc, sizeof(char*)*strlen(szString));
	strcpy_s(szMalloc, strlen(szString) + 1, szString);
	memmove_s(&szMalloc[szTemp - szString], sizeof(char*)*strlen(szString), &szMalloc[(szTemp - szString) + 1], strlen(szMalloc));
	strcpy_s(Memblock.szRetBuffer, 1023, szMalloc);
	free(szMalloc);
	szMalloc = (char*)0x0;
	return Memblock.szRetBuffer;
}

/*@note: Adds the famous IRC brackets i.e. <nick>.
@param szIn: Target nickname
@param szOut: Modified nickname, i.e. <szIn>
@return: Nickname with brackets added*/
char* AddBrackets(char* szIn, char* szOut)
{
	assert(szIn && szOut);
	strcpy_s(szOut, strlen(szIn) + 1, szIn);
	memmove(&szOut[1], szOut, strlen(szOut));
	szOut[0] = '<'; szOut[strlen(szOut)] = '>';
	return szOut;
}

/*@note: Returns the IRC nickname from a message, note that this has not been tested with all IRC messages
@param szIn: The raw IRC message
@return: the nickname*/
char* GetNick(char* szIn)
{
	assert(szIn);
	if (!strstr(szIn, "!"))
		return szIn;
	char* RetVal = SubStringRange(szIn, ":", "!", 1);
	char* szOut = (malloc(sizeof(char*)*strlen(RetVal)));
	memset(szOut, 0, sizeof(char*)*strlen(RetVal));
	RetVal = AddBrackets(RetVal, szOut);
	return RetVal;
}

/*@note: Returns the IRC user from a message, note that this has not been tested with all IRC messages
@param szIn: The raw IRC message
@return: the user*/
char* GetUser(char* szIn)
{
	assert(szIn);
	char* Begin = szBoyer_Moore(szIn, strlen(szIn), "!", strlen("!"));
	char* szTemp = (malloc(sizeof(char*)*strlen(szIn)));
	char* szRange = SubStringRange(Begin, "!", " ", 0);
	strcpy_s(szTemp, strlen(szRange) + 1, szRange);
	strcpy_s(Memblock.szRetBuffer, 1023, szTemp);
	free(szTemp);
	szTemp = NULL;
	return Memblock.szRetBuffer;
}

/*@note: Sends the irc message to a particular window, WM_CREATE_IRC_WINDOW message is sent with the channel's name which is the key in the hash table,
if said key exists, the message is forwarded to that window.
@param szString: The message
@return: The parsed IRC message*/
char* IRCParse_ChannelMessage(char* szString)
{
	assert(szString);
	char* szNick = GetNick(szString);
	assert(szNick);
	strcpy_s(szChannel, strlen(GetChannel(szString)) + 1, GetChannel(szString));
	SendMessage(ContextIRC.hParent, WM_CREATE_IRC_WINDOW, (WPARAM)szChannel, 0);
	char* szMessage = szBoyer_Moore(szString, strlen(szString), " :", strlen(" :"));
	assert(szMessage);
	szMessage = Substring(szMessage, ":", 1);
	assert(szMessage);
	char* szMalloc = malloc(sizeof(char*)*strlen(szString));
	memset(szMalloc, 0, sizeof(char*)*strlen(szString));
	strcat(szNick, " ");
	strcat(szNick, szMessage);
	strcpy_s(szMalloc, strlen(szNick) + 1, szNick);
	strcat(ParseRetVal, szMalloc);
	//free(szMalloc);
	//free(szNick);
	szNick = NULL;
	szMalloc = NULL;
	return ParseRetVal;
}
/*@note: Sends the irc message to a particular window, WM_CREATE_IRC_WINDOW message is sent with the user's ID name which is the key in the hash table,
if said key exists, the message is forwarded to that window. Note that considering that multiple servers aren't provided, this should be fine, however
when this is added (if it ever will be), then the server name should be concatenated to the key, to ensure that a new window is opened, even if the
same user sends a PM from different servers etc.
@param szString: The message
@return: The parsed IRC message*/
char* IRCParse_PrivateMessage(char* szString)
{
	assert(szString);
	char* szNick = GetNick(szString);
	char* szUser = GetUser(szString);
	assert(szNick && szUser);
	strcpy_s(ContextIRC.szHashKey, strlen(szUser) + 1, szUser);
	SendMessage(ContextIRC.hParent, WM_CREATE_IRC_WINDOW, (WPARAM)szUser, 0);
	char* szMessage = szBoyer_Moore(szString, strlen(szString), " :", strlen(" :"));
	szMessage = Substring(szMessage, ":", 1);
	char* szMalloc = malloc(sizeof(char*)*strlen(szString));
	memset(szMalloc, 0, sizeof(char*)*strlen(szString));
	strcat(szNick, " ");
	strcat(szNick, szMessage);
	strcpy_s(szMalloc, strlen(szNick) + 1, szNick);
	strcat(ParseRetVal, szMalloc);
	//free(szMalloc);
	//free(szNick);
	szMalloc = NULL;
	szNick = NULL;
	return ParseRetVal;
}