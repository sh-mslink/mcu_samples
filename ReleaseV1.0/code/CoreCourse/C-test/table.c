/**
  ******************************************************************************
  * @file    C-test/table.c 
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

//声明一个结构体数组，数组中每个元素都是一个结构体
student students[STUDUENTS_NUM]={0};  


//定义一个函数来写学生信息
void tableRecoder(uint16_t index, uint16_t no, char *name, uint8_t gender, uint32_t birthday)
{
	students[index].No=no;
	students[index].pName = name;
	students[index].Gender = gender; //1代表男，0代表女
	students[index].Birthday = birthday;
}

//定义一个函数根据输入的学号在表中查找，如果找到就把学生的所有信息都显示出来
void showStudentInfo(uint16_t no)
{
	uint16_t i,len;
	len = STUDUENTS_NUM;
	
	printf("students table size is %d\n", len);
	
	for(i=0; i < len; i++ )
	{
		if(students[i].No == no)
		{
			printf("Found [%d], \tName=%s, \tgender=%d,	\tbirthday=%d\n", 
												students[i].No, students[i].pName,students[i].Gender,students[i].Birthday);
			break;
		}
	}
	if(i==len)
		printf("Failed~! Not found the No.\n");
}


//定义一个测试用函数
void tableTest(void)
{
	//初始化数据表
	tableRecoder(0,1,gpNameTable[0],0,19950101);
	tableRecoder(1,2,gpNameTable[1],1,19950202);
	tableRecoder(2,3,gpNameTable[2],1,19950303);
	tableRecoder(3,4,gpNameTable[3],0,19950404);
	tableRecoder(4,5,gpNameTable[4],1,19950505);
	tableRecoder(5,6,gpNameTable[5],0,19950606);
	tableRecoder(6,7,gpNameTable[6],1,19950707);
	tableRecoder(7,8,gpNameTable[7],0,19950808);
	//显示数据表中某个学号的学生信息
	showStudentInfo(1);
	//显示数据表中某个学号的学生信息
	showStudentInfo(5);
}
