#include<string.h>
#include"global.h"

int unique(char *PID){
	inode *q;
	q=comp_list->next;
	while(q!=NULL){
		if (strcmp(PID,q->pcb->PID)==0) return 1;
		else q=q->next;
	}
	if (q==NULL) return 0;
}

int create(char* PID, int priority){
	int i,judge;
	PCB* p;//指向新创建的PCB
	inode* q;//用于遍历各inode类型的链表
	inode* add;//指向新建的inode节点

	judge=unique(PID);
	if (judge==1){
		printf("process %s is already existing\n",PID);
		return -1;
	}
	p=(PCB*)malloc(sizeof(PCB));//创建新PCB
	//初始化PCB  
	strcpy(p->PID,PID);//PID
	p->priority=priority;//priority
	p->status='1';//ready
	p->list= 0; //ready list
	p->parent=running;//将正在运行的进程设为父进程
	//创建索引节点并加入com_list
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;
	add->next=NULL;
	list_insert(comp_list,add);

	//将此进程加入父进程的子进程列表
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;
	add->next=NULL;
	list_insert(running->pcb->children,add);
	//孩子节点
	p->children=(inode*)malloc(sizeof(inode));
    p->children->next=NULL;//孩子队列带头节点
	//资源初始化
	for (i=0;i<4;i++){
		p->occupied_resource[i]=0;
		p->waiting_resource[i]=0;}

	//添加进ready_list,需要重新建立一个inode节点
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;//ready_list节点指向PCB,一一对应******
	add->next=NULL;
	if (priority==0){
		list_insert(ready_list[0],add);
	}
	else if (priority==1){
		list_insert(ready_list[1],add);
	}
	else if (priority==2){
		list_insert(ready_list[2],add);
	}
	else 
		return 0;
	schedule();
}

int destroy(char* PID){//将所有子进程都杀掉之后才schedule
	//从com_list中找到对应inode（保存该inode，以便最后free pcb），进而找到进程pcb
	//从父进程的子进程列表中删除此进程
	//kill tree所有子进程
	//schedule()
	inode *p,*index;//index用于记录comp_list中对应inode节点
	//寻找对应inode
	index=comp_list->next;
	while (index!=NULL)//strcmp条件有问题******
	{
		if (strcmp(index->pcb->PID,PID)==0) break;
		else index=index->next;
	}
	if (index==NULL) return -1;//未找到该PID对应pcb
	//删除父进程的子进程记录
	p=index->pcb->parent->pcb->children;
	list_delete(p,PID);
	//kill tree所有子进程
	kill_tree(PID);
	//调度
	schedule();
	return 0;
}
int kill_tree(char *PID){
	//kill tree所有子进程
	//删除所有列表中此进程的索引节点
	//回收被该进程占用的资源
	//free（index），回收该pcb的内存
	int ls;//记录pcb所在队列
	int pri;//进程优先级
	inode *p,*q,*pre;//index用于记录comp_list中对应inode节点
	//寻找对应inode
	pre=comp_list;
	while (pre->next!=NULL)//strcmp条件有问题******
	{
		if (strcmp(pre->next->pcb->PID,PID)==0) break;
		else pre=pre->next;
	}
	if (pre->next==NULL) return -1;//未找到该PID对应pcb
	//kill tree所有子进程
	p=pre->next->pcb->children;
	while (p->next!=NULL)
	{
		kill_tree(p->next->pcb->PID);
		q=p->next;
		p->next=q->next;
		free(q);
	}
	//删除队列中此进程的索引节点,回收占有的资源
	ls=pre->next->pcb->list;
	switch (ls)
	{
	case -1:{
		q=running;
		running=NULL;
		free(q);
		//schedule();//将现有running进程终止之后有必要重新调度
		break;}
	case 0:{
		pri=pre->next->pcb->priority;
		p=ready_list[pri];
		list_delete(p,PID);
		break;}
	//阻塞队列
	case 1:{
		p=resources[0].p;
		list_delete(p,PID);
		break;}
	case 2:{
		p=resources[1].p;
		list_delete(p,PID);
		break;}
	case 3:{
		p=resources[2].p;
		list_delete(p,PID); 
		break;}
	case 4:{
		p=resources[3].p;
		list_delete(p,PID);
		break;}
	default:{
		return -1;
		break;}
	}
	q=pre->next;
	rfree(q);
	//回收内存
	pre->next=q->next;//删除comp_list中的节点
	free(q->pcb);//回收pcb
	free(q);
	printf("%s has been destroyed\n",PID);
	return 0;
}