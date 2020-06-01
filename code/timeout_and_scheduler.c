#include"global.h"

int show_rela(){//debug
	//<for debugging******
	inode* p;
	if (running->pcb->parent!=NULL)
		printf("parent of process %s: %s \n",running->pcb->PID,running->pcb->parent->pcb->PID);
	p=running->pcb->children;
	printf("children of process %s:\n",running->pcb->PID);
	while(p->next!=NULL){
		printf("(%s)",p->next->pcb->PID);
		p=p->next;
	}
	printf("\n");
	return 0;
	//>for debugging******
}

int schedule(){
	inode* p;//记录优先级最高的进程
	inode* add;//新建ready_list节点
	int prior_p, prior_running;
	//find highest piority process p
	if (ready_list[2]->next!=NULL) p=ready_list[2]->next;//******
	else if (ready_list[1]->next!=NULL) p=ready_list[1]->next;
	else if (ready_list[0]->next!=NULL) p=ready_list[0]->next;
	else return 1;//没有就绪进程，不调度

	//判断是否满足三个条件中的任何一个,满足则抢占
	if (p==NULL) printf("p=null\n");
	else if (p->pcb==NULL) printf("p->pcb==null\n");//***
	prior_p=p->pcb->priority;
	//条件5
	if (running==NULL){
		ready_list[prior_p]->next=p->next;//从等待序列中移除p进程
		running=p;//将p放入运行进程
		running->pcb->status='0';
		running->pcb->list=-1;
		running->next=NULL;
		printf("* process %s is running\n",p->pcb->PID);
		//show_rela();//******
		return 0;
	}
	//条件3、4
	prior_running=running->pcb->priority;
	if (prior_p > prior_running || running->pcb->status!='0'){//多方案****
		ready_list[prior_p]->next=p->next;//从等待序列中移除p进程
		//将running进程移入对应等待序列尾
		running->pcb->status='1';
		running->pcb->list=0;
		list_insert(ready_list[prior_running],running);
		//将p放入运行进程
		running=p;
		running->pcb->status='0';
		running->pcb->list=-1;
		running->next=NULL;
		printf("* process %s is running\n",p->pcb->PID);
		//show_rela();//******
		return 1;
	}
	printf("process %s is running\n",running->pcb->PID);
	//show_rela();//******
	return -1;
}

int time_out(){
	inode *p;
	int pri;
	p=running;
	running=NULL;
	p->pcb->list=0;
	p->pcb->status='1';
	pri=p->pcb->priority;
	list_insert(ready_list[pri],p);//插入对应ready队列
	schedule();
	return 0;
}