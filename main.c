#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"struct.h"

//全局变量,偷个懒，一开始就声明全局变量而不是在调用init（）之后，这样就不用考虑双重指针，更简单。
//struct PCB** block_list;//不需要单独的阻塞序列//用全局变量不需要二级指针
inode* comp_list;//comp_list头指针
inode* running;//running process
inode* ready_list[3]={NULL};//对应三个优先级0,1,2
//inode waiting_list;//各资源的RCB自己维护自己的阻塞队列
RCB resources[4];//四种资源

//函数声明,加声明避免函数定义顺序影响调用
int schedule();
int init();
int create(char* PID, char priority);
int destroy(char* PID);
int rfree(struct PCB* p);
int request(char* RID, int n);
int release(char* RID, int n);
int kill_tree(struct PCB* p);
int time_out();
int judgeSpace(char* str);
int test_shell();

int list_insert(inode* list,inode node){
	//插入队列操作
}

int main(){
	int i;
	comp_list=(inode*)malloc(sizeof(inode));//给comp_list建立头节点
	running=NULL;//只有一个running进程，所以不需要头节点
	for (i=0;i<3;i++){//给每个优先级队列建立头节点
		ready_list[i]=(inode*)malloc(sizeof(inode));//记得最后要free这几块内存******
		ready_list[i]->next=NULL;
	}
	for (i=0;i<4;i++)
		resources[i].p=NULL;
	test_shell();
	return 0;
}