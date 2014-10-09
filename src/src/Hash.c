#include "Hash.h"
#include <stdio.h>

/*
@param pHashStruct: Hash structure, this ought to be null as a new one is allocated in this procedure
 and returned to the caller.
 @param pHashFunction: Pointer to a function that hashes a key
 @return: A dynamically allocated pointer to a PHash structure*/
PHash Hash_Create(PHash pHashStruct, HashFunction pHashFunction)
{
	/*We don't want the hash structure to already be allocated*/
	assert(!pHashStruct && pHashFunction);
	pHashStruct = (PHash)malloc(sizeof(Hash));
	if (pHashStruct)
	{
		pHashStruct->pHashFunction = pHashFunction;
		pHashStruct->Table = (HWND*)malloc(0x400);
		if (pHashStruct->Table)
		{
			/*Set its block size, to be used if reallocation is needed but that probably
			won't be implementeded for this kind of application as the number of windows will probably 
			not be large*/
			memset(pHashStruct->Table, 0, 0x400);
			pHashStruct->nBlockSize = 0x400;
			pHashStruct->nTableSize = pHashStruct->nBlockSize / 4;
			/*Same as above*/
			pHashStruct->TableBase = pHashStruct->Table;
			return pHashStruct;
		}
	}
	return NULL;
}

/*@param ppHashStruct: A pointer to the pointer of the hash structure, frees the table and the hash structure*/
void Hash_Destroy(PHash* ppHashStruct)
{
	assert(ppHashStruct && *ppHashStruct);
	if ((*ppHashStruct)->Table)
	{
		free((*ppHashStruct)->Table);
		(*ppHashStruct)->Table = NULL;
		free(*ppHashStruct);
		(*ppHashStruct) = NULL;
	}
}

/*@param HashStruct: Pointer to the active hash structure
  @param Data: Data to the inserted to the table
  @param szKey: Key to be used*/
void Hash_Insert(PHash HashStruct, HWND Data, TCHAR* szKey)
{
	assert(HashStruct && szKey);
	uint32_t nIndex = HashStruct->pHashFunction(HashStruct, szKey);
	HashStruct->Table[nIndex] = Data;
}

/*@param HashStruct: Pointer to active hash structure
  @param szKey: Key used to retrieve the data in the table*/
HWND Hash_Get(PHash HashStruct, TCHAR* szKey)
{
	assert(HashStruct && szKey);
	/*Transform key*/
	uint32_t nIndex = HashStruct->pHashFunction(HashStruct, szKey);
	/*...Is it a valid pointer?*/
	assert(HashStruct->Table + nIndex);
	/*If so, return it..*/
	return HashStruct->Table[nIndex];
}

/*@param pHashStruct: Hash structure
  @param szKey: Key to transform
  @return: Transformed key
 */
uint32_t HashTransform(PHash pHashStruct, TCHAR* szKey)
{
	assert(pHashStruct && szKey);
	uint32_t nLen = strlen(szKey);
	uint32_t nHash = nLen, nTemp;
	int32_t nRem = 0;
	/*Cannot transform to integer index if key is empty!*/
	if (nLen <= 0)
		return 0;
	nRem = nLen & 3;
	nLen >>= 2;

	for (; nLen > 0; --nLen)
	{
		nHash += *(const int16_t*)(szKey);
		nTemp = (*(const int16_t*)(szKey + 2) << 11) ^ nHash;
		nHash = (nHash << 16) ^ nTemp;
		szKey += 2 * sizeof(int16_t);
		nHash += nHash >> 11;
	}

	switch (nRem)
	{
	case 3:
		nHash += *(const int16_t*)(szKey);
		nHash ^= nHash << 16;
		nHash ^= ((int8_t)szKey[sizeof(uint16_t)]) << 18;
		nHash += nHash >> 11;
		break;
	case 2:
		nHash += *(const int16_t*)(szKey);
		nHash ^= nHash << 11;
		nHash += nHash >> 17;
		break;
	case 1:
		nHash += (int8_t)*szKey;
		nHash ^= nHash << 10;
		nHash += nHash >> 1;
	}
	__asm
	{
			mov eax, nHash
			mov ecx, eax
			shl ecx, 0x3
			xor eax, ecx
			mov nHash, eax

			mov eax, nHash
			mov ecx, eax
			shr ecx, 0x5
			add eax, ecx
			mov nHash, eax

			mov eax, nHash
			mov ecx, eax
			shl ecx, 0x4
			xor eax, ecx
			mov nHash, eax

			mov eax, nHash
			mov ecx, eax
			shr ecx, 0x11
			add eax, ecx
			mov nHash, eax

			mov eax, nHash
			mov ecx, eax
			shl ecx, 0x19
			xor eax, ecx
			mov nHash, eax

			mov eax, nHash
			mov ecx, eax
			shr ecx, 0x6
			add eax, ecx
			mov nHash, eax
	}
	return nHash % pHashStruct->nTableSize;
}