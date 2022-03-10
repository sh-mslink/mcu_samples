/**
 **************************************************************************
 * File Name    : test.h
 * Description  : test for structure and linked list header file
 * Date         : 2020-10-15
 * Version      : V1.0.0
 **************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEST_H
#define __TEST_H

#ifdef __cplusplus
extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "board.h"
	
	
#define STUDUENTS_NUM  40 /*定义学生人数*/
	

//定义一个结构体
typedef struct  //C语言语法，定义结构体的指令
{
	uint16_t No;
	char *pName;
	uint8_t Gender;
	uint32_t Birthday;
} student;  //新定义的结构体可以有个变量类型的名字，以后就能用这个名字去声明结构体变量。  

//定义一个结构体给链表
typedef struct node //C语言语法，定义结构体的指令
{
	struct node *pNext;  //定义一个指向后面结构体的指针
	struct node *pPrev;  //定义一个指向前面结构体的指针
	student info;    //本节点的实际数据结构体
	uint8_t freeOrNot; //定义一个特别的标志用来辅助模拟内存分配，和链表本身关系不大
} studentNode;  //新定义的结构体可以有个变量类型的名字，以后就能用这个名字去声明结构体变量。  


extern char *gpNameTable[];

extern void tableTest(void);
extern void LinkedListTest(void);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_H */