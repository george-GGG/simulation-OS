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

	//唤醒阻塞进程,若proc自身为阻塞进程则需另外处理。。。
	if (proc->pcb->waiting_resource[r]>0) proc->pcb->waiting_resource[r]+=n;//自身为r上的阻塞进程时
	p=resources[r].p->next;
	while (p!=NULL&&resources[r].available_number > p->pcb->waiting_resource[r]){
		resources[r].available_number-=p->pcb->waiting_resource[r];
		resources[r].p->next=resources[r].p->next->next;//将p从该资源的阻塞队列中删除
		p->pcb->occupied_resource[r]=p->pcb->waiting_resource[r];
		p->pcb->waiting_resource[r]=0;
		p->pcb->list=0;//加入ready list
		p->pcb->status='1';
		list_insert(ready_list[p->pcb->priority],p);//插入对应就绪队列
		p=resources[r].p->next;
	}
	schedule();
	return 0;
}

int rfree(inode* p){//destroy时用于将进程p占用的资源释放
	//若p自身就是阻塞进程，则要先在阻塞队列里删掉p,否则释放资源后唤醒进程时会出错***
	if (p->pcb->status=='2') list_delete(resources[p->pcb->list-1].p,p->pcb->PID);//自身为阻塞进程
	release(p,"R1",p->pcb->occupied_resource[0]);
	release(p,"R2",p->pcb->occupied_resource[1]);
	release(p,"R3",p->pcb->occupied_resource[2]);
	release(p,"R4",p->pcb->occupied_resource[3]);
	return 0;
}