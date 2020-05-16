#include<stdio.h>
#include<string.h>
#include"global.h"
#include"struct.h"
int init(){
	//队列、资源初始化
	int i;
	comp_list=(inode*)malloc(sizeof(inode));//给comp_list建立头节点
	comp_list->next=NULL;
	running=NULL;//只有一个running进程，所以不需要头节点
	for (i=0;i<3;i++){//给每个优先级队列建立头节点
		ready_list[i]=(inode*)malloc(sizeof(inode));//记得最后要free这几块内存******
		ready_list[i]->next=NULL;
	}
	for (i=0;i<4;i++){
		resources[i].p=(inode*)malloc(sizeof(inode));//头节点
		resources[i].p->next=NULL;
		resources[i].initial_number=i+1;
		resources[i].available_number=i+1;
	}
	printf("varies inited\n");//debug
	//建立init进程
	PCB* p;//指向新建的init PCB
	inode *add;//inode节点
	p=(PCB*)malloc(sizeof(PCB));//create PCB of init

	//p初始化
	p->parent=NULL;
	p->children=NULL;
	strcpy(p->PID,"init");//PID
	p->status='0';
	p->list=-1;//NULL表示正在运行
	p->priority='0';
	for (i=0;i<4;i++){
		p->occupied_resource[i]=0;
		p->waiting_resource[i]=0;
	}
	//加入comp_list队列
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;
	add->next=NULL;
	list_insert(comp_list,add);
	//加入running序列
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;
	add->next=NULL;
	running=add;
	
	printf("* process init is running");
	return 0;
}