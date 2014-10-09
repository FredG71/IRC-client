#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <Windows.h>

typedef struct __DoublyList{
	struct __DoublyList* pPrev;
	struct __DoublyList* pNext;
	struct __DoublyList* pBack;
	struct __DoublyList* pFront;
	struct __DoublyList* pCurrent;
	HWND* pData;
} DoublyList, *PDoublyList;

PDoublyList List_Init(PDoublyList pList);
void List_Insert_Back(PDoublyList pList, HWND Data);
void List_Destroy(PDoublyList ppList);
HWND List_GetFront(PDoublyList pList);
HWND List_GetBack(PDoublyList pList);
uint8_t List_SearchFront(PDoublyList pList, HWND pData);
uint8_t List_SearchBack(PDoublyList pList, HWND pData);
HWND List_GetSearchFront(PDoublyList pList, HWND pData);
HWND List_GetSearchBack(PDoublyList pList, HWND pData);