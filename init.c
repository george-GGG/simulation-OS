#include<stdio.h>
#include"global.h"
#include"struct.h"
int init(){
	//队列初始化
	int i;
	comp_list=(inode*)malloc(sizeof(inode));//给comp_list建立头节点
	comp_list->next=NULL;
	running=NULL;//只有一个running进程，所以不需要头节点
	for (i=0;i<3;i++){//给每个优先级队列建立头节点
		ready_list[i]=(inode*)malloc(sizeof(inode));//记得最后要free这几块内存******
		ready_list[i]->next=NULL;
	}
	for (i=0;i<4;i++)
		resources[i].p=NULL;

	struct PCB* p;//指向新建的init PCB
	p=(struct PCB*)malloc(sizeof(struct PCB));//create PCB of init

	//p初始化
	p->parent=NULL;
	p->children=NULL;
	strcpy(p->PID,"init");//PID
	p->status='0';
	p->list=NULL;//NULL表示正在运行
	p->priority='0';
	for (i=0;i<4;i++)
		p->resources[i]=0;

	running=p;//加入runing序列
	printf("* process init is running");
	return 1;

}