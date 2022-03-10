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
	
	
#define STUDUENTS_NUM  40 /*����ѧ������*/
	

//����һ���ṹ��
typedef struct  //C�����﷨������ṹ���ָ��
{
	uint16_t No;
	char *pName;
	uint8_t Gender;
	uint32_t Birthday;
} student;  //�¶���Ľṹ������и��������͵����֣��Ժ�������������ȥ�����ṹ�������  

//����һ���ṹ�������
typedef struct node //C�����﷨������ṹ���ָ��
{
	struct node *pNext;  //����һ��ָ�����ṹ���ָ��
	struct node *pPrev;  //����һ��ָ��ǰ��ṹ���ָ��
	student info;    //���ڵ��ʵ�����ݽṹ��
	uint8_t freeOrNot; //����һ���ر�ı�־��������ģ���ڴ���䣬���������ϵ����
} studentNode;  //�¶���Ľṹ������и��������͵����֣��Ժ�������������ȥ�����ṹ�������  


extern char *gpNameTable[];

extern void tableTest(void);
extern void LinkedListTest(void);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_H */