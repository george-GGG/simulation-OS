#ifndef STRUCT_H
#define STRUCT_H

//定义结构体
struct inode//索引节点，目的是给所有进程（可能处于不同状态）建立一个索引列表，方便检索。
{
	struct PCB* pcb;
	struct inode* next;
};
typedef struct inode inode;

struct PCB{
	char PID[5];	
	int occupied_resource[4];//已分配的资源，对应R1,R2,R3,R4
	int waiting_resource[4];//等待分配的资源
	char status;//0:run, 1:ready, 2:block,3:none
	int list;//-1:running; 0:ready list; 1-4: waiting list
	struct inode* parent;//父进程只有一个
	struct inode* children;//子进程不止一个，所以要建一个inode列表，inode每个节点指向对应子进程PCB
	char priority;//0,1,2
};
typedef struct PCB PCB;

struct RCB{
	//char RID[5];
	int initial_number;
	int available_number;
	struct inode* p;//waiting list
};
typedef struct RCB RCB;
#endif