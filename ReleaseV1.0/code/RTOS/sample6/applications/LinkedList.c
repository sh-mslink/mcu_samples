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

//为了方便操作，预定义好一个全局的字符串表
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
//声明一个结构体数组，用来模拟动态内存分配
static studentNode memeryPool[STUDUENTS_NUM]={0};  

//模拟动态内存分配函数

static studentNode *getMemery(void)
{
	uint16_t i;
	for(i=0;i<STUDUENTS_NUM;i++)
	{
		if(memeryPool[i].freeOrNot == 0)
		{
			memeryPool[i].freeOrNot = 0xff; //标记该节点已经被分配掉了
			return &memeryPool[i];
		}
	}
	return NULL;
}

//模拟动态内存释放函数
static void freeMemery(studentNode *handler)
{
	handler->freeOrNot = 0x0; //标记该节点已经被释放掉了
}
//--------------------------------------------------------------------------------
*/

//在链表最后添加新的信息
static uint16_t appendInfo(uint16_t no, char *name, uint8_t gender, uint32_t birthday)
{
	studentNode *p = pHeader, *pNewNode=NULL;
	
	if(pHeader == NULL) //先要检查一下是不是链表是空的
	{
		//pHeader = getMemery();
		pHeader = (studentNode *)rt_malloc(sizeof(studentNode));
		if(pHeader == NULL)
		{
			rt_kprintf("something wrong, cannot get memory~!\n");
			return 0;
		}
		//给链表里第一个节点赋值
		pHeader ->info.No=no;
		pHeader ->info.pName = name;
		pHeader ->info.Gender = gender; //1代表男，0代表女
		pHeader ->info.Birthday = birthday;
		//设置第一个节点的指针
		pHeader ->pPrev = NULL; //第一个节点的前一个节点是空的
		pHeader ->pNext = NULL;  //第一个节点后面还没有数据所以是空
	}
	else //如果链表不为空，需要先找到链表的尾部
	{
		while(p ->pNext != NULL)
		{
			p=p->pNext;
		}
		
		//找到后申请内存
		//pNewNode=getMemery();
		pNewNode = (studentNode *)rt_malloc(sizeof(studentNode));
		if(pNewNode == NULL)
		{
			rt_kprintf("something wrong, cannot get memory~!\n");
			return 0;
		}
		p->pNext=pNewNode;
		
		//给新的节点赋值
		pNewNode->info.No = no;
		pNewNode->info.pName = name;
		pNewNode->info.Gender = gender; //1代表男，0代表女
		pNewNode->info.Birthday = birthday;
		pNewNode ->pNext = NULL;
		pNewNode ->pPrev = NULL;
		
		//设置新节点的指针
		pNewNode->pPrev = p; 
		pNewNode->pNext = NULL; 
	}
	return 1;
}

//根据给定的学号在链表里找到相应的节点并返回其地址
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

//在链表中间插入新的信息
//找到给定的学号信息后，把新的信息插到该信息前面
static uint16_t insertBefore(uint16_t searchNo, uint16_t newNo, char *newName, uint8_t newGender, uint32_t newBirthday)
{
	studentNode *p = NULL;
	studentNode *pNewNode = NULL;
	studentNode *pPrevNode=NULL, *pNextNode=NULL;	//为了逻辑上看起来更清晰，多定义两个指针来指向当前节点的前后节点
	
	//在链表里搜索给定学号的节点
	p=getHandlerFromNo(searchNo);
	if(p == NULL)
	{
		rt_kprintf("can not find the item~!\n");
		return 0;
	}
	
	pPrevNode=p->pPrev;
	pNextNode=p;
	
	//分配新的内存给新的节点
	//pNewNode = getMemery();
	pNewNode = (studentNode *)rt_malloc(sizeof(studentNode));
	if(pNewNode == NULL)
	{
		rt_kprintf("something wrong, cannot get memory~!\n");
		return 0;
	}
	
	//将新节点的信息根据传入信息初始化
	pNewNode ->info.No=newNo;
	pNewNode ->info.pName = newName;
	pNewNode ->info.Gender = newGender; //1代表男，0代表女
	pNewNode ->info.Birthday = newBirthday;
	pNewNode ->pNext = NULL;
	pNewNode ->pPrev = NULL;
	
	if(pPrevNode == NULL) //如果要在第一个节点前插入
	{
		pHeader=pNewNode;
		pNewNode->pNext=pNextNode;
		pNewNode->pPrev=NULL;
		
		pNextNode->pPrev=pNewNode;
	}
	else
	{	
		//通过修改指针将新的节点插入链表，为了避免混乱，先把新的节点链接进去
		pNewNode->pNext=pNextNode;
		pNewNode->pPrev=pPrevNode;
		//再更新新节点前节点的指针
		pPrevNode->pNext = pNewNode; 
		//最后更新新节点后节点的指针
		pNextNode->pPrev = pNewNode; 
	}
	return 1;
}

//从链表里删除某条信息
static uint16_t deleteInfo(uint16_t searchNo)
{
	studentNode *p = getHandlerFromNo(searchNo);
	studentNode *pPrevNode=NULL, *pNextNode=NULL;	//为了逻辑上看起来更清晰，多定义两个指针来指向当前节点的前后节点
	
	if(p == NULL)
	{
		rt_kprintf("can not find the item~!\n");
		return 0;
	}
	
	pPrevNode=p->pPrev;
	pNextNode=p->pNext;
	
	if((pPrevNode != NULL)&&(pNextNode==NULL)) //如果要删除的是最后一个节点
	{
		pPrevNode->pNext = NULL; 
	}
	else if((pPrevNode == NULL)&&(pNextNode != NULL)) //如果要删除的是第一个节点
	{
		pHeader = pNextNode;
		pNextNode->pPrev=NULL;
	}
	else
	{
		//将要被删除的信息从链表里脱离出来
		pPrevNode->pNext=pNextNode;
		pNextNode->pPrev=pPrevNode;
	}
	
	//释放节点占用的内存
	//freeMemery(p);
	rt_free(p);
}

//显示整个链表
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

//创建一个用于测试链表的函数
void LinkedListTest(void)
{
	//先在链表里添加3个人的信息
	appendInfo(1,gpNameTable[0],0,19950101);
	appendInfo(2,gpNameTable[1],1,19950202);
	appendInfo(3,gpNameTable[2],1,19950303);
	//显示链表中的内容
	showLinkedList();
	
	//在学号为2号的信息前插入一个新的信息
	insertBefore(2,4,gpNameTable[3],0,19950404);
	//显示链表中的内容
	showLinkedList();
	
	//删除学号为1的信息
	deleteInfo(1);
	//在学号为3号的信息前插入一个新的信息
	insertBefore(3,6,gpNameTable[5],0,19950606);
	//显示链表中的内容
	showLinkedList();
	
	//在学号为4号的信息前插入一个新信息
	insertBefore(4,5,gpNameTable[4],1,19950505);
	//显示链表中的内容
	showLinkedList();
	
	//删除学号为3的信息
	deleteInfo(3);
	//显示链表中的内容
	showLinkedList();
}