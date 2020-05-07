#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//定义结构体
typedef struct inode//索引节点，目的是给所有进程（可能处于不同状态）建立一个索引列表，方便检索。
{
	PCB* pcb;
	inode* next;
}inode;


typedef struct PCB{
	char PID[5];	
	int occupied_resource[4];//已分配的资源，对应R1,R2,R3,R4
	int waiting_resource[4];//等待分配的资源
	char status;//0:run, 1:ready, 2:block,3:none
	int list;//-1:running; 0:ready list; 1-4: waiting list
	inode* parent;//父进程只有一个
	inode* children;//子进程不止一个，所以要建一个inode列表，inode每个节点指向对应子进程PCB
	char priority;//0,1,2
}PCB;


/*
struct rlist{//ready_list
	char priority;//记录此节点对应的优先级
	struct PCB* p;//指向对应PCB
	struct rlist* next;
};
*/

typedef struct RCB{
	char RID[5];
	int initial_number;
	int available_number;
	inode* p;//waiting list
}RCB;

//全局变量,偷个懒，一开始就声明全局变量而不是在调用init（）之后，这样就不用考虑双重指针，更简单。
//struct PCB** block_list;//不需要单独的阻塞序列
//struct PCB** running_process;//用全局变量不需要二级指针
inode* index_list;//comp_list头指针
inode* running;
inode* ready_list[3]={NULL};//对应三个优先级0,1,2
//inode waiting_list;//各资源的RCB自己维护自己的阻塞队列
RCB resources[4];//四种资源
//初始化
index_list=NULL;
running=NULL;
int i;
for(i=0;i<3;i++){
	ready_list[i]=(inode*)malloc(sizeof(inode));//记得最后要free这几块内存******
}
//ready_list=NULL;



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

int main(){
	int i;
//	running_process=&running;//用二级指针才能在其他函数里改变值
	//指针初始化为NULL
	running=NULL;
	for (i=0;i<3;i++)
		ready_list[i].next=NULL;
	for (i=0;i<4;i++)
		resource_list[i].p=NULL;
	test_shell();
	return 0;
}