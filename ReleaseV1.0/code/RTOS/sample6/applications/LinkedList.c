/**
  ******************************************************************************
  * @file    C-test/LinkedList.c 
  * @author  Sean Wang
  * @version V1.0.0
  * @date    2020-10-15
  * @brief   Main program body
  ******************************************************************************
  */ 
	
#include <stdio.h>
#include "test.h"

//Ϊ�˷��������Ԥ�����һ��ȫ�ֵ��ַ�����
char *gpNameTable[]=
{
	"Colin",
	"Geogrge",
	"Remon",
	"Eric",
	"Quentin",
	"White",
	"Sean",
	"Daniel"
};

static studentNode *pHeader=NULL;

/*
//--------------------------------------------------------------------------------
//����һ���ṹ�����飬����ģ�⶯̬�ڴ����
static studentNode memeryPool[STUDUENTS_NUM]={0};  

//ģ�⶯̬�ڴ���亯��

static studentNode *getMemery(void)
{
	uint16_t i;
	for(i=0;i<STUDUENTS_NUM;i++)
	{
		if(memeryPool[i].freeOrNot == 0)
		{
			memeryPool[i].freeOrNot = 0xff; //��Ǹýڵ��Ѿ����������
			return &memeryPool[i];
		}
	}
	return NULL;
}

//ģ�⶯̬�ڴ��ͷź���
static void freeMemery(studentNode *handler)
{
	handler->freeOrNot = 0x0; //��Ǹýڵ��Ѿ����ͷŵ���
}
//--------------------------------------------------------------------------------
*/

//�������������µ���Ϣ
static uint16_t appendInfo(uint16_t no, char *name, uint8_t gender, uint32_t birthday)
{
	studentNode *p = pHeader, *pNewNode=NULL;
	
	if(pHeader == NULL) //��Ҫ���һ���ǲ��������ǿյ�
	{
		//pHeader = getMemery();
		pHeader = (studentNode *)rt_malloc(sizeof(studentNode));
		if(pHeader == NULL)
		{
			rt_kprintf("something wrong, cannot get memory~!\n");
			return 0;
		}
		//���������һ���ڵ㸳ֵ
		pHeader ->info.No=no;
		pHeader ->info.pName = name;
		pHeader ->info.Gender = gender; //1�����У�0����Ů
		pHeader ->info.Birthday = birthday;
		//���õ�һ���ڵ��ָ��
		pHeader ->pPrev = NULL; //��һ���ڵ��ǰһ���ڵ��ǿյ�
		pHeader ->pNext = NULL;  //��һ���ڵ���滹û�����������ǿ�
	}
	else //�������Ϊ�գ���Ҫ���ҵ������β��
	{
		while(p ->pNext != NULL)
		{
			p=p->pNext;
		}
		
		//�ҵ��������ڴ�
		//pNewNode=getMemery();
		pNewNode = (studentNode *)rt_malloc(sizeof(studentNode));
		if(pNewNode == NULL)
		{
			rt_kprintf("something wrong, cannot get memory~!\n");
			return 0;
		}
		p->pNext=pNewNode;
		
		//���µĽڵ㸳ֵ
		pNewNode->info.No = no;
		pNewNode->info.pName = name;
		pNewNode->info.Gender = gender; //1�����У�0����Ů
		pNewNode->info.Birthday = birthday;
		pNewNode ->pNext = NULL;
		pNewNode ->pPrev = NULL;
		
		//�����½ڵ��ָ��
		pNewNode->pPrev = p; 
		pNewNode->pNext = NULL; 
	}
	return 1;
}

//���ݸ�����ѧ�����������ҵ���Ӧ�Ľڵ㲢�������ַ
static studentNode *getHandlerFromNo(uint16_t searchNo)
{
	studentNode *p = pHeader;
	
	while(p != NULL)
	{
		if(p->info.No==searchNo)
		{
			return p;
		}
		p=p->pNext;
	}
	return NULL;
}

//�������м�����µ���Ϣ
//�ҵ�������ѧ����Ϣ�󣬰��µ���Ϣ�嵽����Ϣǰ��
static uint16_t insertBefore(uint16_t searchNo, uint16_t newNo, char *newName, uint8_t newGender, uint32_t newBirthday)
{
	studentNode *p = NULL;
	studentNode *pNewNode = NULL;
	studentNode *pPrevNode=NULL, *pNextNode=NULL;	//Ϊ���߼��Ͽ��������������ඨ������ָ����ָ��ǰ�ڵ��ǰ��ڵ�
	
	//����������������ѧ�ŵĽڵ�
	p=getHandlerFromNo(searchNo);
	if(p == NULL)
	{
		rt_kprintf("can not find the item~!\n");
		return 0;
	}
	
	pPrevNode=p->pPrev;
	pNextNode=p;
	
	//�����µ��ڴ���µĽڵ�
	//pNewNode = getMemery();
	pNewNode = (studentNode *)rt_malloc(sizeof(studentNode));
	if(pNewNode == NULL)
	{
		rt_kprintf("something wrong, cannot get memory~!\n");
		return 0;
	}
	
	//���½ڵ����Ϣ���ݴ�����Ϣ��ʼ��
	pNewNode ->info.No=newNo;
	pNewNode ->info.pName = newName;
	pNewNode ->info.Gender = newGender; //1�����У�0����Ů
	pNewNode ->info.Birthday = newBirthday;
	pNewNode ->pNext = NULL;
	pNewNode ->pPrev = NULL;
	
	if(pPrevNode == NULL) //���Ҫ�ڵ�һ���ڵ�ǰ����
	{
		pHeader=pNewNode;
		pNewNode->pNext=pNextNode;
		pNewNode->pPrev=NULL;
		
		pNextNode->pPrev=pNewNode;
	}
	else
	{	
		//ͨ���޸�ָ�뽫�µĽڵ��������Ϊ�˱�����ң��Ȱ��µĽڵ����ӽ�ȥ
		pNewNode->pNext=pNextNode;
		pNewNode->pPrev=pPrevNode;
		//�ٸ����½ڵ�ǰ�ڵ��ָ��
		pPrevNode->pNext = pNewNode; 
		//�������½ڵ��ڵ��ָ��
		pNextNode->pPrev = pNewNode; 
	}
	return 1;
}

//��������ɾ��ĳ����Ϣ
static uint16_t deleteInfo(uint16_t searchNo)
{
	studentNode *p = getHandlerFromNo(searchNo);
	studentNode *pPrevNode=NULL, *pNextNode=NULL;	//Ϊ���߼��Ͽ��������������ඨ������ָ����ָ��ǰ�ڵ��ǰ��ڵ�
	
	if(p == NULL)
	{
		rt_kprintf("can not find the item~!\n");
		return 0;
	}
	
	pPrevNode=p->pPrev;
	pNextNode=p->pNext;
	
	if((pPrevNode != NULL)&&(pNextNode==NULL)) //���Ҫɾ���������һ���ڵ�
	{
		pPrevNode->pNext = NULL; 
	}
	else if((pPrevNode == NULL)&&(pNextNode != NULL)) //���Ҫɾ�����ǵ�һ���ڵ�
	{
		pHeader = pNextNode;
		pNextNode->pPrev=NULL;
	}
	else
	{
		//��Ҫ��ɾ������Ϣ���������������
		pPrevNode->pNext=pNextNode;
		pNextNode->pPrev=pPrevNode;
	}
	
	//�ͷŽڵ�ռ�õ��ڴ�
	//freeMemery(p);
	rt_free(p);
}

//��ʾ��������
static void showLinkedList(void)
{
	uint16_t i=0;
	studentNode *p = pHeader;
	rt_kprintf("=========list start=========\n");
	while(p != NULL)
	{
		i++;
		rt_kprintf("[%d]: No=%d, \tName=%s, \tgender=%d,	\tbirthday=%d\n", 
											i, p->info.No, p->info.pName, p->info.Gender, p->info.Birthday);
		p=p->pNext;
	}
	rt_kprintf("=========list end=========\n\n");	
}

//����һ�����ڲ�������ĺ���
void LinkedListTest(void)
{
	//�������������3���˵���Ϣ
	appendInfo(1,gpNameTable[0],0,19950101);
	appendInfo(2,gpNameTable[1],1,19950202);
	appendInfo(3,gpNameTable[2],1,19950303);
	//��ʾ�����е�����
	showLinkedList();
	
	//��ѧ��Ϊ2�ŵ���Ϣǰ����һ���µ���Ϣ
	insertBefore(2,4,gpNameTable[3],0,19950404);
	//��ʾ�����е�����
	showLinkedList();
	
	//ɾ��ѧ��Ϊ1����Ϣ
	deleteInfo(1);
	//��ѧ��Ϊ3�ŵ���Ϣǰ����һ���µ���Ϣ
	insertBefore(3,6,gpNameTable[5],0,19950606);
	//��ʾ�����е�����
	showLinkedList();
	
	//��ѧ��Ϊ4�ŵ���Ϣǰ����һ������Ϣ
	insertBefore(4,5,gpNameTable[4],1,19950505);
	//��ʾ�����е�����
	showLinkedList();
	
	//ɾ��ѧ��Ϊ3����Ϣ
	deleteInfo(3);
	//��ʾ�����е�����
	showLinkedList();
}