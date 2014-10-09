#include "List.h"

/*@note: Initializes the list structure
@param pList: List structure to initialize
@return: Dynamically allocated doubly linked list structure; list is empty*/
PDoublyList List_Init(PDoublyList pList)
{
	//assert(!pList);
	pList = (PDoublyList)malloc(sizeof(DoublyList));
	assert(pList);
	pList->pBack = NULL;
	pList->pFront = NULL;
	pList->pNext = NULL;
	pList->pPrev = NULL;
	return pList;
}

/*
@note: Inserts a new element after pList->pFront (or pList->pCurrent if called a second time)
List_Insert_Front is not implemented as it isn't needed at the moment
@param pList: List structure
@param pData: Pointer to the data to add to the list
*/
void List_Insert_Back(PDoublyList pList, HWND Data)
{
	assert(pList);
	/* First time called. List is empty */
	if (!pList->pFront)
	{
		/*Allocate a new node, front is the first node*/
		pList->pFront = (PDoublyList)malloc(sizeof(DoublyList));
		assert(pList->pFront);
		pList->pFront->pData = (HWND*)malloc(sizeof(HWND));
		assert(pList->pFront->pData);
		*pList->pFront->pData = Data;
		pList->pFront->pNext = NULL;
		pList->pFront->pPrev = NULL;
		pList->pBack = pList->pFront;
		pList->pCurrent = pList->pFront;
	}
	else
	{
		pList->pCurrent->pNext = (PDoublyList)malloc(sizeof(DoublyList));
		assert(pList->pCurrent->pNext);
		pList->pCurrent->pNext->pData = (HWND*)malloc(sizeof(HWND));
		assert(pList->pCurrent->pNext->pData);
		*pList->pCurrent->pNext->pData = Data;
		pList->pCurrent->pNext->pNext = NULL;
		pList->pCurrent->pNext->pPrev = pList->pCurrent;
		pList->pBack = pList->pCurrent->pNext;
		pList->pCurrent = pList->pCurrent->pNext;
	}
}

/*@note: Deallocates any used memory, first deallocated PDoublyList::pData, then the nodes, then finally the structure itself
@param ppList: Pointer to the doubly linked list structure*/
void List_Destroy(PDoublyList ppList)
{
	PDoublyList pList;
	assert(ppList);
	while (ppList->pFront)
	{
		pList = ppList->pFront;
		ppList->pFront = ppList->pFront->pNext;
		free(pList->pData);
		pList->pData = NULL;
		free(pList);
		pList = NULL;
	}
	free(ppList);
	ppList = NULL;
}

/*@note: Returns the first element in the linked list
@param pList: Pointer to the list structure
@return First element in the list*/
HWND List_GetFront(PDoublyList pList)
{
	assert(pList);
	assert(pList->pFront);
	assert(pList->pFront->pData);
	return *pList->pFront->pData;
}

/*@note: Returns the last element in the linked list
@param pList: Pointer to the list structure
@return: Last element in the list*/
HWND List_GetBack(PDoublyList pList)
{
	assert(pList && pList->pBack && pList->pBack->pData);
	return *pList->pBack->pData;
}

/*@note: Sequentially searches the linked list for pData, starts from the front
@param pList: Pointer to the list structure
@param pData: Data to search for
@return 1 if pData is found, otherwise 0*/
uint8_t List_SearchFront(PDoublyList pList, HWND pData)
{
	assert(pList && pList->pFront);
	PDoublyList pTemp = pList->pFront;
	while (pTemp)
	{
		if (pTemp->pData)
		{
			if (*pTemp->pData == pData)
				return 1;
		}
		pTemp = pTemp->pNext;
	}
	return 0;
}
/*@note: Sequentially searches the linked list for pData, starts from the back
@param pList: Pointer to the list structure
@param pData: Data to search for
@return 1 if pData is found, otherwise 0*/
uint8_t List_SearchBack(PDoublyList pList, HWND pData)
{
	assert(pList && pList->pBack);
	PDoublyList pTemp = pList->pBack;
	while (pTemp)
	{
		if (pTemp->pData)
		{
			if (*pTemp->pData == pData)
				return 1;
		}
		pTemp = pTemp->pPrev;
	}
	return 0;
}
/*@note: Sequentially searches the linked list for pData, starts from the front
@param pList: Pointer to the list structure
@param pData: Data to search for
@return The found element, otherwise NULL*/
HWND List_GetSearchFront(PDoublyList pList, HWND pData)
{
	//assert(pList && pList->pFront);
	PDoublyList pTemp = pList->pFront;
	while (pTemp)
	{
		if (pTemp->pData)
		{
			if (*pTemp->pData == pData)
				return *pTemp->pData;
		}
		pTemp = pTemp->pNext;
	}
	return 0;
}
/*@note: Sequentially searches the linked list for pData, starts from the back
@param pList: Pointer to the list structure
@param pData: Data to search for
@return The found element, otherwise NULL*/
HWND List_GetSearchBack(PDoublyList pList, HWND pData)
{
	assert(pList && pList->pBack);
	PDoublyList pTemp = pList->pBack;
	while (pTemp)
	{
		if (pTemp->pData)
		{
			if (*pTemp->pData == pData)
				return *pTemp->pData;
		}
		pTemp = pTemp->pPrev;
	}
	return 0;
}
