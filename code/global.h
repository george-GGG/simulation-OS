#ifndef GLOBAL_H
#define GLOBAL_H
#include<stdio.h>
#include "struct.h"
#include "func.h"
//全局变量的声明
extern inode* comp_list;
extern inode* running;
extern inode* ready_list[3]={NULL};
extern RCB resources[4];

#endif
