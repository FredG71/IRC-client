#define _CRT_SECURE_NO_WARNINGS
#include "IRC.h"
#include "Parse.h"

SYSTEMTIME Time;
char ParseRetVal[512];

char* GetChannel(char* szRaw)
{
	assert(szRaw);
	return SubStringRange(szRaw, "#", " ", 0);
}

void SetTime(void)
{
	ZeroMemory(&Time, sizeof(SYSTEMTIME));
	GetLocalTime(&Time);
	sprintf_s(ParseRetVal, strlen("%02d:%02d ") + 1, "%02d:%02d ", Time.wHour, Time.wMinute);
}

/* Modified version of the Boyer Moore implemention on the wikipedia page, see http://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_string_search_algorithm */
char* szBoyer_Moore(
	char* szString,
	int32_t nStringlen,
	char* szPat,
	int32_t nPatlen
	)
{
	assert(szString && szPat && nStringlen > 0 && nPatlen > 0);
	int32_t i;
	int32_t delta1[256];
	int32_t* delta2 = malloc(nPatlen * sizeof(int32_t));
	make_delta1(delta1, szPat, nPatlen);
	make_delta2(delta2, szPat, nPatlen);

	if (nPatlen == 0)
		return szString;
	/*END IF*/

	i = nPatlen - 1;
	while (i < nStringlen)
	{
		int j = nPatlen - 1;
		while (j >= 0 && (szString[i] == szPat[j]))
		{
			--i;
			--j;
		}
		if (j < 0)
		{
			free(delta2);
			delta2 = NULL;
			return(szString + i + 1);
		}
		i += max(delta1[szString[i]], delta2[j]);
	}
	free(delta2);
	delta2 = NULL;
	return NULL;
}

void make_delta1(int32_t* delta1, char* szPat, int nPatlen)
{
	register int32_t nIndex = 0;
	for (nIndex = 0; nIndex < 256; nIndex++)
		delta1[nIndex] = nPatlen;

	for (nIndex = 0; nIndex < nPatlen - 1; nIndex++)
		delta1[szPat[nIndex]] = nPatlen - 1 - nIndex;
}

int32_t is_prefix(char* szWord, int32_t nWordlen, int32_t nPos)
{
	int32_t nSuffixlen = nWordlen - nPos;
	register int32_t nIndex = 0;
	for (nIndex = 0; nIndex < nSuffixlen; nIndex++)
	{
		if (szWord[nIndex] != szWord[nPos + nIndex])
			return 0;
	}
	return 1;
}

int32_t suffix_length(char* szWord, int32_t nWordlen, int32_t nPos)
{
	register int nIndex = 0;
	for (nIndex = 0; (szWord[nPos - nIndex] == szWord[nWordlen - 1 - nIndex])
		&& (nIndex < nPos); nIndex++);
		return nIndex;
}

void make_delta2(PINT delta2, char* szPat, int32_t nPatlen)
{
	int32_t p;
	int32_t nLast_prefix_index = nPatlen - 1;

	for (p = nPatlen - 1; p >= 0; p--)
	{
		if (is_prefix(szPat, nPatlen, p + 1))
			nLast_prefix_index = p + 1;

		delta2[p] = nLast_prefix_index + (nPatlen - 1 - p);
	}

	for (p = 0; p < nPatlen - 1; p++)
	{
		INT slen = suffix_length(szPat, nPatlen, p);
		if (szPat[p - slen] != szPat[nPatlen - 1 - slen])
			delta2[nPatlen - 1 - slen] = nPatlen - 1 - p + slen;
	}
}

char* IRCParse_ParseAuthNotice(char* szString)
{
	assert(szString);
	char* szTemp = Substring(szString, ":", 1);
	strcat_s(ParseRetVal, 512, szTemp);
	return ParseRetVal;
}

char* IRCParse_OnWelcome(char* szString)
{
	assert(szString);
	char* szTemp = strpbrk(szString, "  ");
	assert(szTemp);
	szTemp = strpbrk(szTemp, ":");
	assert(szTemp);
	szTemp = Substring(szTemp, ":", 1);
	strcat_s(ParseRetVal, 511, szTemp);
	return ParseRetVal;
}

char* IRCParse_OnBounceInfo(char* szString)
{
	assert(szString);
	char* szBoyerTemp = szBoyer_Moore(szString, strlen(szString), szNick, strlen(szNick));
	assert(szBoyerTemp);
	szBoyerTemp = Substring(szBoyerTemp, " ", 1);
	if (szBoyer_Moore(szBoyerTemp, strlen(szBoyerTemp), ":", strlen(":")))
		szBoyerTemp = RemChar(szBoyerTemp, ":");
	strcat_s(ParseRetVal, 511, szBoyerTemp);
	return ParseRetVal;
}

char* IRCParse_OnLUserClient(char* szString)
{
	assert(szString);
	char* szTemp = szBoyer_Moore(szString, strlen(szString), " :", strlen(" :"));
	assert(szTemp);
	szTemp = Substring(szTemp, ":", 1);
	strcat_s(ParseRetVal, 511, szTemp);
	return ParseRetVal;
}

char* IRCParse_OnUserCommands(char* szString)
{
	assert(szString);
	char* szTemp = szBoyer_Moore(szString, strlen(szString), szNick, strlen(szNick));
	assert(szTemp);
	szTemp = Substring(szTemp, " ", 1);
	if (strstr(szTemp, ":"))
		szTemp = RemChar(szTemp, ":");
	strcat_s(ParseRetVal, 511, szTemp);
	return ParseRetVal;
}

char* IRCParse_OnNotice(char* szString)
{
	assert(szString);
	char* szTemp = szBoyer_Moore(szString, strlen(szString), " :", strlen(" :"));
	szTemp = Substring(szTemp, ":", 1);
	assert(szTemp);
	strcat_s(ParseRetVal, 511, szTemp);
	return ParseRetVal;
}

char* IRCParse_OnMOTD(char* szString)
{
	assert(szString);
	char* szTemp = szBoyer_Moore(szString, strlen(szString), " :", strlen(" :"));
	szTemp = Substring(szTemp, ":", 1);
	assert(szTemp);
	strcat_s(ParseRetVal, 511, szTemp);
	return ParseRetVal;
}

char* IRCParse_OnMode(char* szString)
{
	assert(szString);
	char* szTemp = szBoyer_Moore(szString, strlen(szString), "+", strlen("+"));
	assert(szTemp);
	strcat_s(ParseRetVal, 511, "Your user mode: ");
	strcat_s(ParseRetVal, 511 - strlen("Your user mode: "), szTemp);
	return ParseRetVal;
}

char* IRCParse_OnHostmask(char* szString)
{
	assert(szString);
	char* szTemp = szBoyer_Moore(szString, strlen(szString), "!", strlen("!"));
	assert(szTemp);
	szTemp = SubStringRange(szTemp, "!", " ", 1);
	strcat_s(ParseRetVal, 511, "Your hostmask: ");
	strcat_s(ParseRetVal, 511 - strlen("Your hostmask: "), szTemp);
	strcat_s(ParseRetVal, 511 - (strlen("Your hostmask: ") + strlen(szTemp)), "\n");
	return ParseRetVal;
}

char* IRCParse_OnJoin(char* szString)
{
	assert(szString);
	char* szMalloc = malloc(sizeof(char*)*strlen(szString));
	memset(szMalloc, 0, sizeof(char*)*strlen(szString));
	strcpy_s(szMalloc, strlen(szString) + 1, szString);
	szMalloc = Substring(szMalloc, ":", 1);
	szMalloc = Strtrim(szMalloc, " ");
	strcat(szMalloc, " has joined ");
	strcat(szMalloc, szChannel);
	strcat_s(szMalloc, strlen(szMalloc) + strlen("\r\n") + 1, "\r\n");
	strcat_s(ParseRetVal, 511, szMalloc);
	return ParseRetVal;
}