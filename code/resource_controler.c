#include"global.h"
#include<string.h>
int request(char* RID, int n){
	int r;
	if (strcmp(RID,"R1")==0) r=0;
	else if (strcmp(RID,"R2")==0) r=1;
	else if (strcmp(RID,"R3")==0) r=2;
	else if (strcmp(RID,"R4")==0) r=3;
	else { 
		printf("parameter error,there is no this resource\n");
		return -1;
	}
	if (n>(resources[r].initial_number)) {
		printf ("parameter error, there is no so many resources\n");
		return -1;
	}
	else if (n>(resources[r].available_number)){
		//将running进程阻塞
		running->pcb->waiting_resource[r]+=n;
		running->pcb->status=2;//status调为阻塞
		running->pcb->list=r;//队列改为阻塞队列
		list_insert(resources[r].p,running);
		//running进程变为init进程
		ready_list[0]->next->pcb->status=0;
		ready_list[0]->next->pcb->list=-1;
		running=NULL;
		printf("Lack of resource,process has been blocked on %s\n",RID);
		schedule();
	}
	else {
		running->pcb->occupied_resource[r]+=n;
		resources[r].available_number-=n;
		printf("process %s requests %d %s\n",running->pcb->PID,n,RID);
	}
	return 0;
}

int int_pri(char pri){
	if (pri=='0') return 0;
	else if (pri=='1') return 1;
	else if (pri==2) return 2;
	else return -1;
}

int release(inode* proc,char* RID, int n){//proc指向要释放资源的进程的inode,test shell调用rel函数时需带running作为参数
	int r,i;
	inode* p;
	if (n<=0) return -1;
	if (strcmp(RID,"R1")==0) r=0;
	else if (strcmp(RID,"R2")==0) r=1;
	else if (strcmp(RID,"R3")==0) r=2;
	else if (strcmp(RID,"R4")==0) r=3;
	else { 
		printf("parameter error,there is no this resource\n");
		return -1;
	}
	if (n>proc->pcb->occupied_resource[r]){
		printf("parameter error, there is no so many resources\n");
		return -1;
	}
	proc->pcb->occupied_resource[r]-=n;
	resources[r].available_number+=n;

	//唤醒阻塞进程
	p=resources[r].p->next;
	while (p!=NULL&&resources[r].available_number > p->pcb->waiting_resource[r]){
		resources[r].available_number-=p->pcb->waiting_resource[r];
		resources[r].p->next=resources[r].p->next->next;//将p从该资源的阻塞队列中删除
		p->pcb->occupied_resource[r]=p->pcb->waiting_resource[r];
		p->pcb->waiting_resource[r]=0;
		p->pcb->list=0;//加入ready list
		p->pcb->status='1';
		list_insert(ready_list[int_pri(p->pcb->priority)],p);//插入对应就绪队列
		p=resources[r].p->next;
	}
	schedule();
	return 0;
}

int rfree(inode* p){//将进程p占用的资源释放
	release(p,"r1",p->pcb->occupied_resource[0]);
	release(p,"r2",p->pcb->occupied_resource[1]);
	release(p,"r3",p->pcb->occupied_resource[2]);
	release(p,"r4",p->pcb->occupied_resource[3]);
	return 0;
}