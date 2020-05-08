#include<stdlib.h>
#include<string.h>
#include"struct.h"
#include"func.h"
//全局变量,偷个懒，一开始就声明全局变量而不是在调用init（）之后，这样就不用考虑双重指针，更简单。
//不需要单独的阻塞序列//用全局变量不需要二级指针
inode* comp_list;//comp_list头指针，在init函数中初始化
inode* running;//running process，在init（）中初始化
inode* ready_list[3]={NULL};//对应三个优先级0,1,2，init中初始化
//inode waiting_list;//各资源的RCB自己维护自己的阻塞队列
RCB resources[4];//四种资源，init中初始化

int list_insert(inode* list,inode node){
	//插入队列操作
}

int main(){
	test_shell();
	return 0;
}