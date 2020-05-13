#include"global.h"
int schedule(){
	inode* p;//记录优先级最高的进程
	inode* q;//用于遍历wlist
	inode* add;//新建ready_list节点
	int prior_p, prior_running;
	//find highest piority process p
	if (ready_list[2]->next!=NULL) p=ready_list[2]->next;//******
	else if (ready_list[1]->next!=NULL) p=ready_list[1]->next;
	else p=ready_list[0]->next;

	//判断是否满足三个条件中的任何一个,满足则抢占
	prior_p=p->pcb->priority;
	//条件5
	if (running==NULL){
		ready_list[prior_p]->next=p->next;//从等待序列中移除p进程
		running->next=p;//将p放入运行进程
		printf("* process %s is running\n",p->pcb->PID);
	}
	//条件3、4
	prior_running=running->next->pcb->priority;
	if (prior_p > prior_running || running->next->pcb->status!='0'){//多方案****
		ready_list[prior_p]->next=p->next;//从等待序列中移除p进程
		//将running进程移入对应等待序列尾
		add=(inode*)malloc(sizeof(inode));
		add->next=NULL;
		add->pcb=running->next->pcb;
		list_insert(ready_list[prior_running],add);
		//将p放入运行进程
		running->next=p;
		printf("* process %s is running\n",p->pcb->PID);
	}
	return -1;
}

int time_out(){
	inode *p;
	int pri;
	p=running->next;
	running->next=NULL;
	p->pcb->list=0;
	p->pcb->status='1';
	pri=p->pcb->priority;
	list_insert(ready_list[pri],p);//插入对应ready队列
	schedule();
	return 0;
}