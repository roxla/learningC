#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <stdio.h>

#define OK 0
#define ERROR -1

struct mydata{
	char name[20];
	int age;
};

void my_fun1(const char *s1);
void my_fun2(const char *s2);

extern int a_value;

#endif