#include<string.h>
#include"global.h"

int create(char* PID, char priority){
	int i;
	PCB* p;//指向新创建的PCB
	inode* q;//用于遍历各inode类型的链表
	inode* add;//指向新建的inode节点

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
	q= comp_list;//用q遍历running进程的子进程
	while(q->next!=NULL){
		q=q->next;
	}
    q->next=add;

	//将此进程加入父进程的子进程列表
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;
	add->next=NULL;
	q= running->pcb->children;//用q遍历running进程的子进程
	while(q->next!=NULL){
		q=q->next;
	}
    q->next=add;//插入children对列末尾
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
	if (priority=='0'){
		q=ready_list[0];
		while(q->next!=NULL){
            q=q->next;
        }
        q->next=add;
	}
	else if (priority=='1'){
		q=ready_list[1];
		while(q->next!=NULL){
            q=q->next;
        }
        q->next=add;
	}
	else if (priority=='2'){
		q=ready_list[2];
		while(q->next!=NULL){
            q=q->next;
        }
        q->next=add;
	}
	else return 0;
	schedule();
}

int destroy(char* PID){//将所有子进程都杀掉之后才schedule
	//从com_list中找到对应inode（保存该inode，以便最后free pcb），进而找到进程pcb
	//destroy所有子进程
	//从父进程的子进程列表中删除此进程
	//删除所有列表中此进程的索引节点
	//回收被该进程占用的资源
	//free（index），回收该pcb的内存
	//schedule()
	int ls;//记录pcb所在队列
	char pri;//进程优先级
	inode *p,*q,*index;//index用于记录comp_list中对应inode节点
	//寻找对应inode
	index=comp_list->next;
	while (index!=NULL && strcomp(index->pcb->PID,PID)!=0)
	{
		index=index->next;
	}
	if (index->next==NULL) return -1;//未找到该PID对应pcb
	//destroy所有子进程
	p=index->pcb->children;
	while (p->next!=NULL)
	{
		destroy(p->next->pcb->PID);
		p=p->next;
	}
	//删除父进程的记录
	p=index->pcb->parent->pcb->children;
	list_delete(p,PID);
	//删除所有列表中此进程的索引节点,回收占有的资源
	ls=index->pcb->list;
	switch (ls)
	{
	case -1:{
		running=NULL;
		break;}
	case 0:{
		pri=index->pcb->priority;
		if (pri=='0')
			p=ready_list[0];
		else if (pri=='1')
			p=ready_list[1];
		else 
			p=ready_list[2];
		list_delete(p,PID);
		break;}
	//阻塞队列，涉及资源回收*******
	case 1:{
		p=resources[0].p;
		//调用资源释放函数，回收资源
		list_delete(p,PID);
		break;}
	case 2:{
		p=resources[1].p;
		//调用资源释放函数，回收资源
		list_delete(p,PID);
		break;}
	case 3:{
		p=resources[2].p;
		//调用资源释放函数，回收资源
		list_delete(p,PID);
		break;}
	case 4:{
		p=resources[3].p;
		//调用资源释放函数，回收资源
		list_delete(p,PID);
		break;}
	default:{
		return -1;
		break;}
	}
	//回收内存
	free(index);
	return 0;
}