#include "Array.h"
/*
@note: Returns the size of the array, useful for iterating
@param pArray: Pointer to the array structure
@return: The size of the array minus one (zero-based)*/
uint32_t Array_Num(PArray pArray)
{
	assert(pArray);
	return (pArray->nSize > 0) ? pArray->nSize - 1 : pArray->nSize;
}
/*@note: Removes the elements from nStart to nEnd - shifts as needed
@param pArray: Pointer to the array structure
@param nStart: Start point of where to remove, note that &pData[nStart] is removed as well!
@param nEnd: End point*/
void Array_RemoveRange(PArray pArray, uint32_t nStart, uint32_t nEnd)
{
	assert(pArray && nEnd <= pArray->nSize);
	memmove(&pArray->pData[nStart], &pArray->pData[nEnd + 1], Array_Num(pArray) * sizeof(*pArray->pData));
	pArray->nSize -= (nEnd + 1) - nStart;
}

/*@note: removes an element from the array, namely &pData[nIndex] and shifts as needed, note that nothing is deleted!
@param pArray: Pointer to array structure
@param nIndex: Index of element to remove
@return: Removed element*/
void* Array_Remove(PArray pArray, uint32_t nIndex)
{
	assert(pArray && nIndex <= pArray->nSize);
	HWND nTemp = pArray->pData[nIndex];
	memmove(&pArray->pData[nIndex], &pArray->pData[nIndex + 1], Array_Num(pArray) * sizeof(*pArray->pData));
	pArray->nSize--;
	return nTemp;
}
/*@note: Returns index of nElement if found (first occurance), not properly tested
@param pArray: Pointer to array structure
@param nElement: Element to look for
@return: The index or -1 if not found*/
int32_t Array_IndexOf(PArray pArray, void* nElement)
{
	register uint32_t nIndex = 0;
	assert(pArray);
	for (nIndex = 0; nIndex <= Array_Num(pArray); nIndex++)
	{
		if (nElement == pArray->pData[nIndex])
			return nIndex;
	}
	return -1;
}
/*@note: Returns index of nElement if found (last occurance), not properly tested
@param pArray: Pointer to array structure
@param nElement: Element to look for
@return: The index or -1 if not found*/
int32_t Array_LastIndexOf(PArray pArray, void* nElement)
{
	register uint32_t nTemp = 0;
	register uint32_t nIndex = 0;
	assert(pArray);
	for (nIndex = 0; nIndex <= Array_Num(pArray); nIndex++)
	{
		if (nElement == pArray->pData[nIndex])
			nTemp = nIndex;
		/*END IF*/
	}
	return -1;
}
/*@note: Looks for an element in the array and returns 1 or 0
@param pArray: Pointer to array structure
@param nElement: Element to look for
@return: 1 if found, otherwise 0*/
int8_t Array_Contains(PArray pArray, void* nElement)
{
	assert(pArray);
	register uint32_t nIndex = 0;
	for (nIndex = 0; nIndex <= Array_Num(pArray); nIndex++)
	{
		if (pArray->pData[nIndex] == nElement)
			return 1;
	}
	return 0;
}
/*@note: Clears the entire array but not does clean up or delete anything
@param pArray: Pointer to array structure*/
void Array_Clear(PArray pArray)
{
	register uint32_t nIndex = 0;
	register uint32_t nSizeTemp = 0;
	assert(pArray);
	nSizeTemp = pArray->nSize - 1;
	for (nIndex = 0; nIndex <= nSizeTemp; nIndex++)
		pArray->nSize--;
	/*END FOR*/
}
/*@note: Initializes the array
@param pArray: Pointer to array structure
@return: The allocated array structure*/
PArray Array_Init(PArray pArray)
{
	assert(!pArray);
	pArray = (PArray)malloc(sizeof(struct __Array));
	assert(pArray);
	pArray->pData = (HWND*)malloc(sizeof(struct __Array) * 12);
	pArray->nBytesCount = sizeof(struct __Array) * 12;
	pArray->nSize = 0;
	return pArray;
}
/*@note: Destroys the array, and frees up used resoureces
@param pArray: Pointer to array structure*/
void Array_Destroy(PArray* pArray)
{
	assert(pArray && *pArray && (*pArray)->pData);
	free((*pArray)->pData);
	free((*pArray));
}
/*@note: Inserts nElement in pArray->pData + nIndex, not used or tested
@param pArray: Pointer to array structure
@param nIndex: Index where to insert the element
@param nElement: Element to insert*/
void Array_Insert(PArray pArray, uint32_t nIndex, void* nElement)
{
	assert(pArray && nIndex <= pArray->nSize);
	memmove(&pArray->pData[nIndex], &pArray->pData[nIndex - 1], Array_Num(pArray) * sizeof(*pArray->pData));
	*(pArray->pData + nIndex) = nElement;
	pArray->nSize++;
}

/*@note: Adds an element to the array, and reallocated if necessary
@param pArray: Pointer structure etc
@param nElement: Element to add*/
void Array_Add(PArray pArray, void* nElement)
{
	if (pArray->nBytesCount <= ((pArray->nSize + 1) * sizeof(nElement)))
	{
		realloc(pArray->pData, pArray->nBytesCount * sizeof(nElement)* 12);
		pArray->nBytesCount = pArray->nBytesCount * sizeof(nElement)* 12;
	}
	*(pArray->pData + pArray->nSize) = nElement;
	pArray->nSize++;
}

/*@note: Returns the element found at pArray->pData[nIndex], mimics subscript operator - sort of
@param pArray: Pointer structure
@param nIndex: Index position in the array*/
void* Array_Index(PArray pArray, uint32_t nIndex)
{
	assert(pArray && nIndex <= Array_Num(pArray));
	return *(pArray->pData + nIndex);
}





/*
@note: Returns the size of the strArray, useful for iterating
@param pstrArray: Pointer to the strArray structure
@return: The size of the strArray minus one (zero-based)*/
uint32_t strArray_Num(PFArraystr pstrArray)
{
	assert(pstrArray);
	return (pstrArray->nSize > 0) ? pstrArray->nSize - 1 : pstrArray->nSize;
}
/*@note: Removes the elements from nStart to nEnd - shifts as needed
@param pstrArray: Pointer to the strArray structure
@param nStart: Start point of where to remove, note that &pData[nStart] is removed as well!
@param nEnd: End point*/
void strArray_RemoveRange(PFArraystr pstrArray, uint32_t nStart, uint32_t nEnd)
{
	assert(pstrArray && nEnd <= pstrArray->nSize);
	memmove(&pstrArray->pData[nStart], &pstrArray->pData[nEnd + 1], strArray_Num(pstrArray) * sizeof(*pstrArray->pData));
	pstrArray->nSize -= (nEnd + 1) - nStart;
}

/*@note: removes an element from the strArray, namely &pData[nIndex] and shifts as needed, note that nothing is deleted!
@param pstrArray: Pointer to strArray structure
@param nIndex: Index of element to remove
@return: Removed element*/
char* strArray_Remove(PFArraystr pstrArray, uint32_t nIndex)
{
	assert(pstrArray && nIndex <= pstrArray->nSize);
	char* nTemp = pstrArray->pData[nIndex];
	memmove(&pstrArray->pData[nIndex], &pstrArray->pData[nIndex + 1], strArray_Num(pstrArray) * sizeof(*pstrArray->pData));
	pstrArray->nSize--;
	return nTemp;
}
/*@note: Returns index of nElement if found (first occurance), not properly tested
@param pstrArray: Pointer to strArray structure
@param nElement: Element to look for
@return: The index or -1 if not found*/
int32_t strArray_IndexOf(PFArraystr pstrArray, char* nElement)
{
	register uint32_t nIndex = 0;
	assert(pstrArray);
	for (nIndex = 0; nIndex <= strArray_Num(pstrArray); nIndex++)
	{
		//if (nElement == pstrArray->pData[nIndex])
		if (!strcmp(nElement, pstrArray->pData[nIndex]))
			return nIndex;
	}
	return -1;
}
/*@note: Returns index of nElement if found (last occurance), not properly tested
@param pstrArray: Pointer to strArray structure
@param nElement: Element to look for
@return: The index or -1 if not found*/
int32_t strArray_LastIndexOf(PFArraystr pstrArray, char* nElement)
{
	register uint32_t nTemp = 0;
	register uint32_t nIndex = 0;
	assert(pstrArray);
	for (nIndex = 0; nIndex <= strArray_Num(pstrArray); nIndex++)
	{
		if (!strcmp(nElement, pstrArray->pData[nIndex]))
			nTemp = nIndex;
		/*END IF*/
	}
	return -1;
}
/*@note: Looks for an element in the strArray and returns 1 or 0
@param pstrArray: Pointer to strArray structure
@param nElement: Element to look for
@return: 1 if found, otherwise 0*/
int8_t strArray_Contains(PFArraystr pstrArray, char* nElement)
{
	assert(pstrArray);
	register uint32_t nIndex = 0;
	for (nIndex = 0; nIndex <= strArray_Num(pstrArray); nIndex++)
	{
		if (!strcmp(nElement, pstrArray->pData[nIndex]))
			return 1;
	}
	return 0;
}
/*@note: Clears the entire strArray but not does clean up or delete anything
@param pstrArray: Pointer to strArray structure*/
void strArray_Clear(PFArraystr pstrArray)
{
	register uint32_t nIndex = 0;
	register uint32_t nSizeTemp = 0;
	assert(pstrArray);
	nSizeTemp = pstrArray->nSize - 1;
	for (nIndex = 0; nIndex <= nSizeTemp; nIndex++)
		pstrArray->nSize--;
	/*END FOR*/
}
/*@note: Initializes the strArray
@param pstrArray: Pointer to strArray structure
@return: The allocated strArray structure*/
PFArraystr strArray_Init(PFArraystr pstrArray)
{
	assert(!pstrArray);
	pstrArray = (PFArraystr)malloc(sizeof(struct __strFArray));
	assert(pstrArray);
	pstrArray->pData = (char**)malloc(sizeof(struct __strFArray) * 12);
	//*pstrArray->pData = (char*)malloc(sizeof(struct __strFArray) * 1024);
	pstrArray->nBytesCount = sizeof(struct __strFArray) * 12;
	pstrArray->nSize = 0;
	return pstrArray;
}
/*@note: Destroys the strArray, and frees up used resoureces
@param pstrArray: Pointer to strArray structure*/
void strArray_Destroy(PFArraystr* pstrArray)
{
	assert(pstrArray && *pstrArray && (*pstrArray)->pData);
	register uint32_t i = 0;
	do{
		free(((*pstrArray)->pData[i]));
	} while (++i <= strArray_Num(*pstrArray));
	free((*pstrArray)->pData);
	free(*pstrArray);
}
/*@note: Inserts nElement in pstrArray->pData + nIndex, not used or tested
@param pstrArray: Pointer to strArray structure
@param nIndex: Index where to insert the element
@param nElement: Element to insert*/
void strArray_Insert(PFArraystr pstrArray, uint32_t nIndex, char* nElement)
{
	assert(pstrArray && nIndex <= pstrArray->nSize);
	memmove(&pstrArray->pData[nIndex], &pstrArray->pData[nIndex - 1], strArray_Num(pstrArray) * sizeof(*pstrArray->pData));
	//&pstrArray->pData[nIndex] = nElement;
	strcpy_s(pstrArray->pData[nIndex], strlen(nElement) + 1, nElement);
	pstrArray->nSize++;
}

/*@note: Adds an element to the strArray, and reallocated if necessary
@param pstrArray: Pointer structure etc
@param nElement: Element to add*/
void strArray_Add(PFArraystr pStrArray, char* nElement)
{
	if (pStrArray->nBytesCount <= ((pStrArray->nSize + 1) * sizeof(nElement)))
	{
		pStrArray->pData = (char**)realloc(pStrArray->pData, pStrArray->nBytesCount * sizeof(nElement)* 12);
		//*pStrArray->pData = realloc(*pStrArray->pData, pStrArray->nBytesCount * sizeof(nElement)* 12);
		pStrArray->nBytesCount = pStrArray->nBytesCount * sizeof(nElement)* 12;
	}
	//*(pstrArray->pData + pstrArray->nSize) = nElement;
	pStrArray->pData[pStrArray->nSize] = (char*)malloc(sizeof(char*)*strlen(nElement));
	memset(pStrArray->pData[pStrArray->nSize], 0, sizeof(char*)*strlen(nElement));
	strcpy_s(pStrArray->pData[pStrArray->nSize], strlen(nElement) + 1, nElement);
	pStrArray->nSize++;
}

/*@note: Returns the element found at pstrArray->pData[nIndex], mimics subscript operator - sort of
@param pstrArray: Pointer structure
@param nIndex: Index position in the strArray*/
char* strArray_Index(PFArraystr pstrArray, uint32_t nIndex)
{
	assert(pstrArray && nIndex <= strArray_Num(pstrArray) && nIndex >= 0);
	return *(pstrArray->pData + nIndex);
}
