#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//定义结构体
typedef struct wlist{//waiting_list:ready list,block list的节点
	struct PCB* p;//指向对应PCB
	struct wlist* next;
}wlist;

typedef struct complete_list//一个PCB对应一个complete_list节点，目的是给所有进程（可能处于不同状态）建立一个索引列表，方便检索。
{
	PCB* pcb;
	complist* next;
}complist;


typedef struct PCB{
	char PID[5];	
	int resource[4];//对应R1,R2,R3,R4
	char status;//0:run, 1:ready, 2:block,3:none
	struct wlist* list;//0:ready list; 1:block list; 2:NULL(running)
	struct PCB* parent;//父进程只有一个，所以直接指向其PCB
	struct wlist* children;//子进程不止一个，所以要建一个wlist列表，wlist每个节点指向对应子进程PCB
	char priority;//0,1,2
}PCB;


/*
struct rlist{//ready_list
	char priority;//记录此节点对应的优先级
	struct PCB* p;//指向对应PCB
	struct rlist* next;
};
*/

typedef struct RCB{
	char RID[5];
	int initial_number;
	int available_number;
	struct wlist* p;//waiting list
}RCB;

//全局变量,偷个懒，一开始就声明全局变量而不是在调用init（）之后，这样就不用考虑双重指针，更简单。
//struct PCB** block_list;//不需要单独的阻塞序列
//struct PCB** running_process;//用全局变量不需要二级指针
complist* comp_list;//comp_list头指针
wlist ready_list[3];//对应三个优先级0,1,2
PCB* running;
RCB resource_list[4];

//函数声明,加声明避免函数定义顺序影响调用
int schedule();
int init();
int create(char* PID, char priority);
int destroy(char* PID);
int rfree(struct PCB* p);
int request(char* RID, int n);
int release(char* RID, int n);
int kill_tree(struct PCB* p);
int time_out();
int judgeSpace(char* str);

int schedule(){
	struct wlist* p;//记录优先级最高的进程
	struct wlist* q;//用于遍历wlist
	struct wlist* add;//新建ready_list节点
	int prior_p, prior_running;
	//find highest piority process p
	if (ready_list[2].next !=NULL) p=ready_list[2].next;
	else if (ready_list[1].next!=NULL) p=ready_list[1].next;
	else p=ready_list[0].next;

	//判断是否满足三个条件中的任何一个,满足则抢占
	prior_p=(int)p->p->priority;
	//条件5
	if (running==NULL){
		ready_list[prior_p].next=p->next;//从等待序列中移除p进程
		running=p->p;//将p放入运行进程
		printf("* process %s is running\n",p->p->PID);
	}
	//条件3、4
	prior_running=(int)running->priority;
	if (prior_p > prior_running || running->status!='0'){
		ready_list[prior_p].next=p->next;//从等待序列中移除p进程
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

int init(){
	int i;
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
		p->resource[i]=0;

	running=p;//加入runing序列
	printf("* process init is running");
	return 1;

}

int create(char* PID, char priority){
	int i;
	struct PCB* p;//指向新创建的PCB
	struct wlist* q;//用于遍历各wlist类型的链表
	struct wlist* add;//指向新建的wlist节点

	p=(struct PCB*)malloc(sizeof(struct PCB));//创建新PCB
	//初始化PCB
	strcpy(p->PID,PID);//PID
	p->priority=priority;//priority
	p->status='1';
	p->list= ready_list;
	p->parent=running;//将正在运行的进程设为父进程
	//将此进程加入父进程的子进程列表
	add=(struct wlist*)malloc(sizeof(struct wlist));
	add->p=p;
	add->next=NULL;
	q=running->children;//用q遍历running进程的子进程
	if (q==NULL) running->children=add;
	while(q!=NULL){
		if (q->next==NULL) {q->next=add;break;}
		else q=q->next;
	}

	p->children=NULL;
	for (i=0;i<4;i++)
		p->resource[i]=0;

	//添加进ready_list
	add=(struct wlist*)malloc(sizeof(struct wlist));
	add->p=p;//ready_list节点指向PCB,一一对应******
	add->next=NULL;
	if (priority=='0'){
		q=&ready_list[0];
		if (q->next==NULL)
			q->next=add;
		else {
			do{
				q=q->next;
			}while (q->next!=NULL);
			q->next=add;
		}
	}
	else if (priority=='1'){
		q=&ready_list[1];
		if (q->next==NULL)
			q->next=add;
		else {
			do{
				q=q->next;
			}while (q->next!=NULL);
			q->next=add;
		}
	}
	else if (priority=='2'){
		q=&ready_list[2];
		if (q->next==NULL)
			q->next=add;
		else {
			do{
				q=q->next;
			}while (q->next!=NULL);
			q->next=add;
		}
	}
	else return 0;
	schedule();
}

int destroy(char* PID){//将所有子进程都杀掉之后才schedule//***********未完成update
	//1、依次从running序列、ready序列、block序列中寻找PID对应的p
	//2、对p调用rfree()
	//3、update所有与p有关的指针和队列，free（p）
	//4、对p的所有子进程递归调用destroy（）
	//schedule()
	int i;
	struct wlist* q;//用于遍历wlist
	struct PCB* p;
	p=NULL;
	q=NULL;
	//search in running
	if (!strcmp(running->PID,PID)) p=running;
	else {
		//search in ready_list
		for (i=0;i<3;i++){
			if (ready_list[i].next==NULL) continue;
			q=ready_list[i].next;
			while(q!=NULL){
				if (q->p->PID==PID) {
					p=q;
					q->
					i=2;//结束外循环******
					break;}
				else q=q->next;
			}
		}
		if (p==NULL){//若还没找到p,则在block队列中找
			//search in block list
			for (i=0;i<4;i++){
				if (resource_list[i].p==NULL) continue;
			q=resource_list[i].p;
			while(q!=NULL){
				if (q->p->PID==PID) {
					p=q;
					i=2;//结束外循环******
					break;}
				else q=q->next;
				}
			}
		}
	}
	//rfree(p)
	rfree(p);

}

int rfree(struct PCB* p){
	//将p占用的资源释放，source_list列表相应资源值增加
	int i;
	for (i=0;i<4;i++)
		resource_list[i]+=p->resource[i]
}

int request(char* PID, char* RID){

}

int release(char* RID, int n, struct RCB* resource_list, struct wlist* ready_list, struct PCB** running_process, struct PCB* running){

}

int kill_tree(struct PCB* p){

}

int time_out(){

}


int process_manager(char* PID, struct PCB** ready_list, struct PCB** block_list){
	//ready to block
	//block to ready
	return 0;
}

int resource_manager(char* RID, struct RCB** resource_list){

}

int judgeSpace(char* str){//判断输入语句是否有空格
	int i;
	for (i=0;i<strlen(str);i++)
		if (str[i]==' ') return 1;
	if (i==strlen(str)) return 0;
}

int test_shell(){//要调用进程管理和资源管理器，所以放在前两个函数后面
	char usrcmd[30];//record usr comand
	char* cmd;//输入字符串第一个空格之前的内容
	char* para1;//parameters
	char* para2;
	int judge;
	char PID[10];//process id
	char priority;
	int n;//number of resource
	char resource[5];
	printf("designed by Chen Zhiguo\n");
	printf("输入exit退出\n\n");
	printf("************************************\n");
	
	while(1){
		//input
		printf("shell> ");
		gets(usrcmd);
		//recognise	
		judge=judgeSpace(usrcmd);
		if (judge==1){
			if (usrcmd[0]==' ') {printf("Illegal comand!";continue;)}
			else
				cmd=strtok(usrcmd," ");}
		//应该先处理无参数命令，再使用strtok函数分隔后处理含参命令
		else if(judge==0)
			strcpy(cmd,usrcmd);

		if (!strcmp(cmd,"init"))
			init();

		else if (!strcmp(cmd,"cr")){
			para1=strtok(NULL," ");
			para2=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			}
			strcpy(PID,para1);
			if (para2=="2") priority='2';
			else if (para2=="1") priority='1';
			else if (para2=="0") priority='0';
			else {printf("优先级错误，请重新输入命令\n"); continue;}
			//call function
			create(PID,priority);
		}
		else if (!strcmp(cmd,"list")){
			para1=strtok(NULL," ");//继续分割usrcmd
			if (!strcmp(para1,"ready"))
				//call func
			else if (!strcmp(para1,"block"))
				//call func
			else if (!strcmp(para1,"res"))
				//call func
			else {printf("parameter error\n"); continue;}
		}
		
		else if (!strcmp(cmd,"to"))
			//call function
		else if (!strcmp(cmd,"req")){
			para1=strtok(NULL," ");
			para2=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			}
			strcpy(resource,para1);
			
			if (para2=="4") n=4;
			else if (para2=="3") n=3;
			else if (para2=="2") n=2;
			else if (para2=="1") n=1;
			else {printf("参数错误，请重新输入命令\n"); continue;}
			//call function
		}
		else if (!strcmp(cmd,"rel")){
			para1=strtok(NULL," ");
			para2=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			}
			strcpy(resource,para1);
			
			if (para2=="4") n=4;
			else if (para2=="3") n=3;
			else if (para2=="2") n=2;
			else if (para2=="1") n=1;
			else {printf("参数错误，请重新输入命令\n"); continue;}
			//call function
		}
		else if (!strcmp(cmd,"de")){
			para1=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			}
			strcpy(PID,para1);
			//call function
		}
		else if (!strcmp(cmd,"exit"))
			exit(0);
		else  
		   {printf("无此命令,请重新输入\n");continue;}

	}
	return 0;
}

int main(){
	int i;
//	running_process=&running;//用二级指针才能在其他函数里改变值
	//指针初始化为NULL
	running=NULL;
	for (i=0;i<3;i++)
		ready_list[i].next=NULL;
	for (i=0;i<4;i++)
		resource_list[i].p=NULL;
	test_shell();
	return 0;
}