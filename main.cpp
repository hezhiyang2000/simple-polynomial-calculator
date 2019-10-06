// ConsoleApplication2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int Status;
#define TRUE 1;
#define FALSE 0;
#define OK 1;
#define ERROR 0;
#define INFEASIBLE -1;
//#define OVERFLOW -2;

#define EPSILON 1E-8

typedef struct {
	double a;
	int index;
}Elem;
typedef struct LNode {
	Elem data;
	struct LNode *next;
}LNode, *LinkList;

Status GetOpeNum(FILE *input, int &openum, int min, int max);
Status ClearList(LinkList L);
Status SimpList(LinkList &L);
Status GetDoubleNum(FILE *input, double &doublenum);


Status InitList(LinkList &L) {
	//initialize the LinkList with a head LNode and {0,0}
	if (L != NULL) ClearList(L);
	L = (LNode *)malloc(sizeof(LNode));
	if (L == NULL) return OVERFLOW;
	L->next = (LNode *)malloc(sizeof(LNode));
	L->next->data.a = 0;
	L->next->data.index = 0;
	L->next->next = NULL;
	return OK;
}//end InitList

Status InitListList(LinkList *list, int num) {
	//init the linklistlist(assign NULL to everyone)
	int num1 = num;
	while (num--) {
		list[num1 - num - 1] = NULL;
	}
	return OK;
}

Status DestroyList(LinkList &L) {
	//destroy the LinkList
	if (L == NULL) return OK;
	ClearList(L);
	free(L->next);
	L->next = NULL;
	return OK;
}//end DestroyList

Status ClearList(LinkList L) {
	//clear the LinkList when the list has already exist
	LNode *t;
	while (L->next != NULL) {
		t = L->next;
		L->next = t->next;
		free(t);
	}
	L->next = (LNode *)malloc(sizeof(LNode));
	L->next->data.a = 0;
	L->next->data.index = 0;
	L->next->next = NULL;
	return OK;
}//end ClearList

Status ListEmpty(LinkList L) {
	//check if the list is empty?
	if (L == NULL) { return INFEASIBLE; }
	else if (L->next->data.index == 0 && fabs(L->next->data.a) < EPSILON) { return TRUE; }
	else return FALSE;
}//end ListEmpty

Status ListLenth(LinkList L, int &l) {
	//return the lenth of the list
	LNode *p = L;
	int i = 0;
	while (p->next != NULL) {
		p = p->next;
		i++;
	}
	l = i;
	return OK;
}//end ListLenth

Status GetElem(LinkList L, int i, Elem &elem) {
	//find elem[i] and assign it to elem
	LNode *p = L;
	while (p->next != NULL && --i) p->next;
	//find the Node before i
	if (p->next == NULL && i != 0) { return ERROR; }
	else {
		elem = p->next->data;
		return OK;
	}
}//end GetElem

Status GetElemWithIndex(LinkList L, int index, Elem &elem) {
	//find elem with given index and assign it to elem
	if (ListEmpty(L)) return ERROR;//the linklist is empty
	LNode *p = L;
	while (p->data.index > index) {//is the index > given index
		p = p->next;
	}
	if (p->data.index != index) {
		return ERROR;//fail to find the elem with the index
	}
	else {
		elem = p->data;
		return OK;//success
	}
}//end GetElemWithIndex

Status DeleteElemAfter(LNode *nodebefore) {
	//delete the node after nodebefore
	if (nodebefore->next == NULL) {
		return ERROR;//nodebefore doesnot have a next
	}
	else {
		LNode *thenode;
		thenode = nodebefore->next;
		nodebefore->next = thenode->next;
		free(thenode);
		return OK;
	}
}//end DeleteElemAfter

Status ListInsert(LinkList &L, Elem elem) {
	//insert a elem in ordered linklist
	LNode *before = L, *after;
	if (fabs(elem.a) < EPSILON) return OK;
	while (before->next->data.index > elem.index) {//find the right place for insert
		before = before->next;
	}
	if (before->next->data.index < elem.index) {//doesnot have a same index lnode
		after = before->next;
		before->next = (LNode *)malloc(sizeof(LNode));
		before->next->data = elem;
		before->next->next = after;
		return OK
	}
	else if (fabs(before->next->data.a - -elem.a) < EPSILON) {//has a offset lnode
		if (elem.a != 0) {
			if (before->next->data.index == 0) {
				before->next->data.a = 0;
				return OK;
			}
			else {
				DeleteElemAfter(before);
				return OK;
			}
		}
	}
	else {//has a unoffset lnode
		before->next->data.a += elem.a;
		return OK;
	}
}//end ListInsert

Status ListMinus(LinkList &L, Elem elem) {
	//minus a elem in ordered linklist
	LNode *before = L, *after;
	while (before->next->data.index > elem.index) {//find the right place for insert
		before = before->next;
	}
	if (before->next->data.index < elem.index) {//doesnot have a same index lnode
		after = before->next;
		before->next = (LNode *)malloc(sizeof(LNode));
		before->next->data = elem;
		before->next->next = after;
		return OK
	}
	else if (fabs(before->next->data.a - elem.a) < EPSILON) {//has a offset lnode
		if (fabs(elem.a) > EPSILON) {
			if (before->next->data.index == 0) {
				before->next->data.a = 0;
				return OK;//if the next is the last
			}
			else {
				DeleteElemAfter(before);
				return OK;
			}
		}
	}
	else {//has a unoffset lnode
		before->next->data.a -= elem.a;
		return OK;
	}
}//end ListInsert

Status WriteList(LinkList &L, FILE *input) {
	//if L is empty, create a new one
	//if not, clear and recreate
	if (!ListEmpty(L)) {
		ClearList(L);
	}
	Elem newdata;
	do {
		GetDoubleNum(input, newdata.a);
		GetOpeNum(input, newdata.index, 0, 2147483647);
		ListInsert(L, newdata);
	} while (!(newdata.index == 0 && fabs(newdata.a) <= EPSILON));
	return OK;
}

Status PrintList(LinkList L, FILE *output) {
	//print the list with good form
	if (ListEmpty(L) == -1) {
		fprintf(output, "NULL\n");
		return OK;
	}//the list is empty
	else if (ListEmpty(L)) {
		fprintf(output, "0.0000\n");
		return OK;
	}
	do {
		if (L->next->data.index == 0) {
			if (fabs(L->next->data.a) > EPSILON) {
				fprintf(output, "%.4lf", L->next->data.a);
			}
		}
		else if (L->next->data.index == 1) {
			fprintf(output, "%.4lfx", L->next->data.a);
			if (L->next->next->data.a > EPSILON) {
				fprintf(output, "+");
			}
		}
		else {
			fprintf(output, "%.4lfx^%d", L->next->data.a, L->next->data.index);
			if (L->next->next->data.a > EPSILON) {
				fprintf(output, "+");
			}
		}
		L = L->next;
	} while (L->next != NULL);
	fprintf(output, "\n");
	return OK;
}

Status CopyList(LinkList originlist, LinkList &aimlist) {
	//copy originlist to aimlist, if aimlist doesnot exist, init it
	if (aimlist == NULL) InitList(aimlist);
	if (!ListEmpty(aimlist)) ClearList(aimlist);
	LNode *aimplacebefore = aimlist;
	Elem newdata;
	while (!(originlist->data.index == 0)) {
		newdata = originlist->next->data;
		ListInsert(aimlist, newdata);
		originlist = originlist->next;
	}
	return OK;
}

Status PlusList(LinkList L1, LinkList L2, LinkList &L3, int mode) {
	// L1 plus(mode1)/minus(mode2) L2, save the answer in L3
	LinkList Lt = NULL;
	InitList(Lt);
	CopyList(L1, Lt);
	if (mode == 1) {
		while (L2->next != NULL) {
			ListInsert(Lt, L2->next->data);
			L2 = L2->next;
		}
	}
	else if (mode == 2) {
		while (L2->next != NULL) {
			ListMinus(Lt, L2->next->data);
			L2 = L2->next;
		}
	}
	if (L3 != NULL) ClearList(L3);
	CopyList(Lt, L3);
	DestroyList(Lt);
	return OK;
}

Status TimesList(LinkList L1, LinkList L2, LinkList &L3) {
	//L1 times L2 and save the answer in L3
	LinkList Lt = NULL;
	InitList(Lt);
	Elem newnode;
	LinkList L2originplace = L2;
	while (L1->next != NULL) {
		while (L2->next != NULL) {
			newnode.a = L1->next->data.a * L2->next->data.a;
			newnode.index = L1->next->data.index + L2->next->data.index;
			ListInsert(Lt, newnode);
			L2 = L2->next;
		}
		L1 = L1->next;
		L2 = L2originplace;
	}
	if (L3 != NULL) ClearList(L3);
	CopyList(Lt, L3);
	DestroyList(Lt);
	return OK;
}

Status GetValue(LinkList L, double x, FILE *output) {
	//put x in L and print the answer
	double answer = 0;
	while (L->next->next != NULL) {
		answer += L->next->data.a * pow(x, L->next->data.index);
		L = L->next;
	}
	fprintf(output, "%.4lf\n", answer);
	return OK;
}

double GetValueReturn(LinkList L, double x) {
	//put x in L and print the answer
	double answer = 0;
	while (L->next->next != NULL) {
		answer += L->next->data.a * pow(x, L->next->data.index);
		L = L->next;
	}
	return answer;
}

Status DiffList(LinkList L1, LinkList &L2) {
	//L1 times L2 and save the answer in L3
	LinkList Lt = NULL;
	InitList(Lt);
	Elem newnode;

	while (L1->next->next != NULL) {
		newnode.a = L1->next->data.a * L1->next->data.index;
		newnode.index = L1->next->data.index - 1;
		ListInsert(Lt, newnode);
		L1 = L1->next;
	}

	if (L2 != NULL) ClearList(L2);
	CopyList(Lt, L2);
	DestroyList(Lt);
	return OK;
}

Status IndefiniteIntegral(LinkList L1, LinkList &L2) {
	LinkList Lt = NULL;
	InitList(Lt);
	Elem newnode;

	while (L1->next != NULL) {
		newnode.a = L1->next->data.a / (L1->next->data.index + 1);
		newnode.index = L1->next->data.index + 1;
		ListInsert(Lt, newnode);
		L1 = L1->next;
	}

	if (L2 != NULL) ClearList(L2);
	CopyList(Lt, L2);
	DestroyList(Lt);
	return OK;
}

Status DefiniteIntegral(LinkList L1, double x1, double x2, FILE *output) {
	LinkList Lt = NULL;
	InitList(Lt);

	IndefiniteIntegral(L1, Lt);

	double dx = 0.00001, ans = 0;

	ans = GetValueReturn(Lt, x2) - GetValueReturn(Lt, x1);

	fprintf(output, "%.4lf\n", ans);

	DestroyList(Lt);
	return OK;
}

Status DivideList(LinkList L1, LinkList L2, LinkList &L3) {
	//divide L1 useing L2 and give up the rest, save in L3
	Elem newnode;//get the newest ans
	LinkList L1t = NULL, t = NULL, L3t = NULL;
	//L1t:temporary L1, will soon have minus operation
	//t:temporary place to save the list for minus
	//L3t:place for answer
	InitList(L1t);
	InitList(t);
	InitList(L3t);
	CopyList(L1, L1t);

	while (L1t->next->data.index >= L2->next->data.index && fabs(L1t->next->data.a) > EPSILON) {
		newnode.index = L1t->next->data.index - L2->next->data.index;//assign newnode
		newnode.a = L1t->next->data.a / L2->next->data.a;
		ListInsert(t, newnode); ListInsert(L3t, newnode);
		TimesList(t, L2, t);

		PlusList(L1t, t, L1t, 2);
		ClearList(t);
	}

	DestroyList(t);
	DestroyList(L1t);
	if (L3 != NULL) ClearList(L3);
	CopyList(L3t, L3);
	DestroyList(L3t);
	return OK;
}

Status DivideListRest(LinkList L1, LinkList L2, LinkList &L3) {
	//divide L1 useing L2 and get the rest, save in L3
	Elem newnode;//get the newest ans
	LinkList L1t = NULL, t = NULL, L3t = NULL;
	//L1t:temporary L1, will soon have minus operation
	//t:temporary place to save the list for minus
	//L3t:place for answer
	InitList(L1t);
	InitList(t);
	InitList(L3t);
	CopyList(L1, L1t);

	while (L1t->next->data.index >= L2->next->data.index && L1t->next->data.index != 0) {
		newnode.index = L1t->next->data.index - L2->next->data.index;
		newnode.a = L1t->next->data.a / L2->next->data.a;
		ListInsert(t, newnode); ListInsert(L3t, newnode);
		TimesList(t, L2, t);
		PlusList(L1t, t, L1t, 2);
		ClearList(t);
	}

	DestroyList(t);
	DestroyList(L3t);
	if (L3 != NULL) ClearList(L3);
	CopyList(L1t, L3);
	DestroyList(L1t);
	return OK;
}

Status GreatestCommonFactor(LinkList L1, LinkList L2, LinkList &L3) {
	//get the greatest common factor with L1 and L2, save in L3
	LinkList L1t = NULL, L2t = NULL;
	//L1t:temporary L1
	//L2t:temporary L2
	InitList(L1t);
	InitList(L2t);
	CopyList(L1, L1t);
	CopyList(L2, L2t);
	//Init the L1t and L2t

	while (1) {
		DivideListRest(L1t, L2t, L1t);
		if (L1t->next->data.index == 0) {
			if (L3 != NULL) ClearList(L3);
			CopyList(L2t, L3);
			break;
		}
		DivideListRest(L2t, L1t, L2t);
		if (L2t->next->data.index == 0) {
			if (L3 != NULL) ClearList(L3);
			CopyList(L1t, L3);
			break;
		}
	}
	//destroy the temporary list
	DestroyList(L1t);
	DestroyList(L2t);
	//simplify the answer
	SimpList(L3);
	return OK;
}

Status MiniCommonMultiple(LinkList L1, LinkList L2, LinkList &L3) {
	LinkList L3t = NULL, greatestcommonfactorlist = NULL;
	InitList(L3t); InitList(greatestcommonfactorlist);
	TimesList(L1, L2, L3t);
	GreatestCommonFactor(L1, L2, greatestcommonfactorlist);
	DivideList(L3t, greatestcommonfactorlist, L3t);
	if (L3 != NULL) ClearList(L3);
	CopyList(L3t, L3);
	DestroyList(L3t);
	return OK;
}

Status SimpList(LinkList &L) {
	//simplify the list by divede L with its max x a
	LinkList NumList;
	NumList = (LNode *)malloc(sizeof(LNode));
	if (NumList == NULL) return OVERFLOW;
	NumList->next = (LNode *)malloc(sizeof(LNode));
	NumList->next->data.a = L->next->data.a;
	NumList->next->data.index = 0;
	NumList->next->next = NULL;
	DivideList(L, NumList, L);
	DestroyList(NumList);
	return OK;
}

Status PowList(LinkList L1, LinkList &L2, int i) {
	//calculate L1^i and return in L2
	if (i == 0) {
		ClearList(L2);
		return OK;
	}
	LinkList L2t, L1t = NULL;
	L2t = (LNode *)malloc(sizeof(LNode));
	if (L2t == NULL) return OVERFLOW;
	L2t->next = (LNode *)malloc(sizeof(LNode));
	L2t->next->data.a = 1;
	L2t->next->data.index = 0;
	L2t->next->next = NULL;
	InitList(L1t);
	CopyList(L1, L1t);
	//temporary place for answer(L2)
	while (i > 0) {
		if (i % 2 == 1)
			TimesList(L2t, L1t, L2t);
		TimesList(L1t, L1t, L1t);
		i = i >> 1;
	}
	if (L2 != NULL) ClearList(L2);
	CopyList(L2t, L2);
	DestroyList(L2t);
	DestroyList(L1t);
	return OK;
}

Status GetOpeNum(FILE *input, int &openum, int min, int max) {
	//get the legal operate number
	double openumbefore;
	do {
		if (fscanf(input, "%lf", &openumbefore)) {
			if (openumbefore - (int)openumbefore > EPSILON) {
				printf("please enter a int!/n");
				exit(0);
			}
			else {
				openum = (int)openumbefore;
				if (openum >= min && openum <= max) {
					break;
				}
				else {
					printf("you entered a float, please enter a int between %d and %d.\n", min, max);
					exit(0);
				}
			}
		}
		else {
			printf("you have not entered a number.");
			exit(0);
		}
	} while (!(openum >= min && openum <= max));
	return OK;
}

Status GetDoubleNum(FILE *input, double &doublenum) {
	//get the legal double number
	if (!fscanf(input, "%lf", &doublenum)) {
		printf("you have not entered a number.");
		exit(0);
	}
	return OK;
}

Status CheckListLegal(LinkList L) {
	if (L == NULL) {
		printf("the list you entered is illegal.");
		exit(0);
	}
}

int main()
{
	//init the listlist
	FILE *input, *output;
	input = fopen("polyn.in", "r");
	output = fopen("polyn.out", "w");
	LinkList list[10];
	InitListList(list, 10);

	int state = 0, m1 = 0, m2 = 0, m3 = 0;
	double x1 = 0, x2 = 0;
	do {
		GetOpeNum(input, state, 0, 17);
		switch (state) {
		case 1: {
			GetOpeNum(input, m1, 0, 9);
			InitList(list[m1]);
			WriteList(list[m1], input);
			break;
		}
		case 2: {
			GetOpeNum(input, m1, 0, 9);
			PrintList(list[m1], output);
			break;
		}
		case 3: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			CheckListLegal(list[m1]);
			CopyList(list[m1], list[m2]);
			break;
		}
		case 4: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			GetOpeNum(input, m3, 0, 9);
			CheckListLegal(list[m1]);
			CheckListLegal(list[m2]);
			PlusList(list[m1], list[m2], list[m3], 1);
			break;
		}
		case 5: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			GetOpeNum(input, m3, 0, 9);
			CheckListLegal(list[m1]);
			CheckListLegal(list[m2]);
			PlusList(list[m1], list[m2], list[m3], 2);
			break;
		}
		case 6: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			GetOpeNum(input, m3, 0, 9);
			CheckListLegal(list[m1]);
			CheckListLegal(list[m2]);
			TimesList(list[m1], list[m2], list[m3]);
			break;
		}
		case 7: {
			GetOpeNum(input, m1, 0, 9);
			GetDoubleNum(input, x1);
			CheckListLegal(list[m1]);
			GetValue(list[m1], x1, output);
			break;
		}
		case 8: {
			GetOpeNum(input, m1, 0, 9);
			DestroyList(list[m1]);
			break;
		}
		case 9: {
			GetOpeNum(input, m1, 0, 9);
			CheckListLegal(list[m1]);
			ClearList(list[m1]);
			break;
		}
		case 10: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			CheckListLegal(list[m1]);
			DiffList(list[m1], list[m2]);
			break;
		}
		case 11: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			CheckListLegal(list[m1]);
			IndefiniteIntegral(list[m1], list[m2]);			
			break;
		}
		case 12: {
			GetOpeNum(input, m1, 0, 9);
			GetDoubleNum(input, x1);
			GetDoubleNum(input, x2);
			CheckListLegal(list[m1]);
			DefiniteIntegral(list[m1], x1, x2, output);
			break;
		}
		case 13: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			GetOpeNum(input, m3, 0, 9);
			CheckListLegal(list[m1]);
			CheckListLegal(list[m2]);
			GreatestCommonFactor(list[m1], list[m2], list[m3]);
			break;
		}
		case 14: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			GetOpeNum(input, m3, 0, 9);
			CheckListLegal(list[m1]);
			CheckListLegal(list[m2]);
			MiniCommonMultiple(list[m1], list[m2], list[m3]);
			break;
		}
		case 15: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			GetOpeNum(input, m3, 0, 9);
			CheckListLegal(list[m1]);
			CheckListLegal(list[m2]);
			DivideList(list[m1], list[m2], list[m3]);
			break;
		}
		case 16: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			GetOpeNum(input, m3, 0, 9);
			CheckListLegal(list[m1]);
			CheckListLegal(list[m2]);
			DivideListRest(list[m1], list[m2], list[m3]);
			break;
		}
		case 17: {
			GetOpeNum(input, m1, 0, 9);
			GetOpeNum(input, m2, 0, 9);
			GetOpeNum(input, m3, 0, 20);
			CheckListLegal(list[m1]);
			PowList(list[m1], list[m2], m3);
			break;
		}
		default: break;
		}
	} while (state != 0);
	fclose(input);
	fclose(output);
}