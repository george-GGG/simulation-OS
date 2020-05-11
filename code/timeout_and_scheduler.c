#include"struct.h"
int schedule(){
	inode* p;//记录优先级最高的进程
	inode* q;//用于遍历wlist
	inode* add;//新建ready_list节点
	int prior_p, prior_running;
	//find highest piority process p
	if (ready_list[2]!=NULL) p=ready_list[2]->next;//******
	else if (ready_list[1]->next!=NULL) p=ready_list[1]->next;
	else p=ready_list[0]->next;

	//判断是否满足三个条件中的任何一个,满足则抢占
	prior_p=(int)p->p->priority;
	//条件5
	if (running==NULL){
		ready_list[prior_p]->next=p->next;//从等待序列中移除p进程
		running=p->p;//将p放入运行进程
		printf("* process %s is running\n",p->p->PID);
	}
	//条件3、4
	prior_running=(int)running->priority;
	if (prior_p > prior_running || running->status!='0'){
		ready_list[prior_p]->next=p->next;//从等待序列中移除p进程
		//将running进程移入对应等待序列尾
		add=(struct wlist*)malloc(sizeof(struct wlist));
		add->next=NULL;
		add->p=running;
		q=ready_list[prior_running].next;
		if (q==NULL) ready_list[prior_running].next=add;
		while(q!=NULL){
			if (q->next==NULL) {q->next=add;break;}
			else q=q->next;
		}
		//将p放入运行进程
		running=p->p;
		printf("* process %s is running\n",p->p->PID);
	}
}

int time_out(){

}