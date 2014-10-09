#pragma once
#include <stdlib.h>
#include <tchar.h>
#include <stdint.h>
#include <assert.h>
#include <Windows.h>

struct __Hash;
typedef uint32_t(*HashFunction)(struct __Hash*, TCHAR* szString);

typedef struct __Hash{
	/*Function pointer to the hash function*/
	HashFunction pHashFunction;
	/*Pointer to the hash table*/
	HWND* Table;
	/*Size of the table*/
	uint32_t nTableSize;
	/*Allocated size of the table, used to reallocate if needed*/
	uint32_t nBlockSize;
	/*Base of table, used to deallocate memory - won't be needed if we know the type of data
	we need to insert i.e. not a pointer to void*/
	HWND* TableBase;
} Hash, *PHash;

PHash Hash_Create(PHash HashStruct, HashFunction pHashFunction);
void Hash_Destroy(PHash* ppHashStruct);
void Hash_Insert(PHash HashStruct, HWND pData, TCHAR* szKey);
HWND Hash_Get(PHash HashStruct, TCHAR* szKey);
uint32_t HashTransform(PHash pHashStruct, TCHAR* szKey);
