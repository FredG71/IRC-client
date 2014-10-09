#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <Windows.h>

typedef struct __Array{
	/*Size of the array*/
	uint32_t nSize;
	/*Holds a pointer to the data*/
	void** pData;
	/*Maximum size of the array - not defined yet!*/
	uint32_t nArrayMax;
	/*Allocated size*/
	uint32_t nBytesCount;
}*PArray;
PArray Array_Init(PArray pArray);
uint32_t Array_Num(PArray pArray);
void Array_RemoveRange(PArray pArray, uint32_t nStart, uint32_t nEnd);
void* Array_Remove(PArray pArray, uint32_t nIndex);
int32_t Array_IndexOf(PArray pArray, void* nElement);
int32_t Array_LastIndexOf(PArray pArray, void* nElement);
int8_t Array_Contains(PArray pArray, void* nElement);
void Array_Clear(PArray pArray);
void Array_Destroy(PArray* pArray);
void Array_Insert(PArray pArray, uint32_t nIndex, void* nElement);
void Array_Add(PArray pArray, void* nElement);
void* Array_Index(PArray pArray, uint32_t nIndex);












/*Multi byte string variant*/
typedef struct __strFArray{
	char** pData;
	uint32_t nSize;
	uint32_t nBytesCount;
	/*Functions */
}*PFArraystr;


uint32_t strArray_Num(PFArraystr pstrArray);
void strArray_RemoveRange(PFArraystr pstrArray, uint32_t nStart, uint32_t nEnd);
char* strArray_Remove(PFArraystr pstrArray, uint32_t nIndex);
int32_t strArray_IndexOf(PFArraystr pstrArray, char* nElement);
int32_t strArray_LastIndexOf(PFArraystr pstrArray, char* nElement);
int8_t strArray_Contains(PFArraystr pstrArray, char* nElement);
void strArray_Clear(PFArraystr pstrArray);
PFArraystr strArray_Init(PFArraystr pstrArray);
void strArray_Destroy(PFArraystr* pstrArray);
void strArray_Insert(PFArraystr pstrArray, uint32_t nIndex, char* nElement);
void strArray_Add(PFArraystr pStrArray, char* nElement);
char* strArray_Index(PFArraystr pstrArray, uint32_t nIndex);