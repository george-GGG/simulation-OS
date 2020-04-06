#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//����ṹ��
typedef struct wlist{//waiting_list:ready list,block list�Ľڵ�
	struct PCB* p;//ָ���ӦPCB
	struct wlist* next;
}wlist;

typedef struct complete_list//һ��PCB��Ӧһ��complete_list�ڵ㣬Ŀ���Ǹ����н��̣����ܴ��ڲ�ͬ״̬������һ�������б����������
{
	PCB* pcb;
	complist* next;
}complist;


typedef struct PCB{
	char PID[5];	
	int resource[4];//��ӦR1,R2,R3,R4
	char status;//0:run, 1:ready, 2:block,3:none
	struct wlist* list;//0:ready list; 1:block list; 2:NULL(running)
	struct PCB* parent;//������ֻ��һ��������ֱ��ָ����PCB
	struct wlist* children;//�ӽ��̲�ֹһ��������Ҫ��һ��wlist�б�wlistÿ���ڵ�ָ���Ӧ�ӽ���PCB
	char priority;//0,1,2
}PCB;


/*
struct rlist{//ready_list
	char priority;//��¼�˽ڵ��Ӧ�����ȼ�
	struct PCB* p;//ָ���ӦPCB
	struct rlist* next;
};
*/

typedef struct RCB{
	char RID[5];
	int initial_number;
	int available_number;
	struct wlist* p;//waiting list
}RCB;

//ȫ�ֱ���,͵������һ��ʼ������ȫ�ֱ����������ڵ���init����֮�������Ͳ��ÿ���˫��ָ�룬���򵥡�
//struct PCB** block_list;//����Ҫ��������������
//struct PCB** running_process;//��ȫ�ֱ�������Ҫ����ָ��
complist* comp_list;//comp_listͷָ��
wlist ready_list[3];//��Ӧ�������ȼ�0,1,2
PCB* running;
RCB resource_list[4];

//��������,���������⺯������˳��Ӱ�����
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
	struct wlist* p;//��¼���ȼ���ߵĽ���
	struct wlist* q;//���ڱ���wlist
	struct wlist* add;//�½�ready_list�ڵ�
	int prior_p, prior_running;
	//find highest piority process p
	if (ready_list[2].next !=NULL) p=ready_list[2].next;
	else if (ready_list[1].next!=NULL) p=ready_list[1].next;
	else p=ready_list[0].next;

	//�ж��Ƿ��������������е��κ�һ��,��������ռ
	prior_p=(int)p->p->priority;
	//����5
	if (running==NULL){
		ready_list[prior_p].next=p->next;//�ӵȴ��������Ƴ�p����
		running=p->p;//��p�������н���
		printf("* process %s is running\n",p->p->PID);
	}
	//����3��4
	prior_running=(int)running->priority;
	if (prior_p > prior_running || running->status!='0'){
		ready_list[prior_p].next=p->next;//�ӵȴ��������Ƴ�p����
		//��running���������Ӧ�ȴ�����β
		add=(struct wlist*)malloc(sizeof(struct wlist));
		add->next=NULL;
		add->p=running;
		q=ready_list[prior_running].next;
		if (q==NULL) ready_list[prior_running].next=add;
		while(q!=NULL){
			if (q->next==NULL) {q->next=add;break;}
			else q=q->next;
		}
		//��p�������н���
		running=p->p;
		printf("* process %s is running\n",p->p->PID);
	}
}

int init(){
	int i;
	struct PCB* p;//ָ���½���init PCB
	p=(struct PCB*)malloc(sizeof(struct PCB));//create PCB of init

	//p��ʼ��
	p->parent=NULL;
	p->children=NULL;
	strcpy(p->PID,"init");//PID
	p->status='0';
	p->list=NULL;//NULL��ʾ��������
	p->priority='0';
	for (i=0;i<4;i++)
		p->resource[i]=0;

	running=p;//����runing����
	printf("* process init is running");
	return 1;

}

int create(char* PID, char priority){
	int i;
	struct PCB* p;//ָ���´�����PCB
	struct wlist* q;//���ڱ�����wlist���͵�����
	struct wlist* add;//ָ���½���wlist�ڵ�

	p=(struct PCB*)malloc(sizeof(struct PCB));//������PCB
	//��ʼ��PCB
	strcpy(p->PID,PID);//PID
	p->priority=priority;//priority
	p->status='1';
	p->list= ready_list;
	p->parent=running;//���������еĽ�����Ϊ������
	//���˽��̼��븸���̵��ӽ����б�
	add=(struct wlist*)malloc(sizeof(struct wlist));
	add->p=p;
	add->next=NULL;
	q=running->children;//��q����running���̵��ӽ���
	if (q==NULL) running->children=add;
	while(q!=NULL){
		if (q->next==NULL) {q->next=add;break;}
		else q=q->next;
	}

	p->children=NULL;
	for (i=0;i<4;i++)
		p->resource[i]=0;

	//��ӽ�ready_list
	add=(struct wlist*)malloc(sizeof(struct wlist));
	add->p=p;//ready_list�ڵ�ָ��PCB,һһ��Ӧ******
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

int destroy(char* PID){//�������ӽ��̶�ɱ��֮���schedule//***********δ���update
	//1�����δ�running���С�ready���С�block������Ѱ��PID��Ӧ��p
	//2����p����rfree()
	//3��update������p�йص�ָ��Ͷ��У�free��p��
	//4����p�������ӽ��̵ݹ����destroy����
	//schedule()
	int i;
	struct wlist* q;//���ڱ���wlist
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
					i=2;//������ѭ��******
					break;}
				else q=q->next;
			}
		}
		if (p==NULL){//����û�ҵ�p,����block��������
			//search in block list
			for (i=0;i<4;i++){
				if (resource_list[i].p==NULL) continue;
			q=resource_list[i].p;
			while(q!=NULL){
				if (q->p->PID==PID) {
					p=q;
					i=2;//������ѭ��******
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
	//��pռ�õ���Դ�ͷţ�source_list�б���Ӧ��Դֵ����
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

int judgeSpace(char* str){//�ж���������Ƿ��пո�
	int i;
	for (i=0;i<strlen(str);i++)
		if (str[i]==' ') return 1;
	if (i==strlen(str)) return 0;
}

int test_shell(){//Ҫ���ý��̹������Դ�����������Է���ǰ������������
	char usrcmd[30];//record usr comand
	char* cmd;//�����ַ�����һ���ո�֮ǰ������
	char* para1;//parameters
	char* para2;
	int judge;
	char PID[10];//process id
	char priority;
	int n;//number of resource
	char resource[5];
	printf("designed by Chen Zhiguo\n");
	printf("����exit�˳�\n\n");
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
		//Ӧ���ȴ����޲��������ʹ��strtok�����ָ�����������
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
			else {printf("���ȼ�������������������\n"); continue;}
			//call function
			create(PID,priority);
		}
		else if (!strcmp(cmd,"list")){
			para1=strtok(NULL," ");//�����ָ�usrcmd
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
			else {printf("����������������������\n"); continue;}
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
			else {printf("����������������������\n"); continue;}
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
		   {printf("�޴�����,����������\n");continue;}

	}
	return 0;
}

int main(){
	int i;
//	running_process=&running;//�ö���ָ�����������������ı�ֵ
	//ָ���ʼ��ΪNULL
	running=NULL;
	for (i=0;i<3;i++)
		ready_list[i].next=NULL;
	for (i=0;i<4;i++)
		resource_list[i].p=NULL;
	test_shell();
	return 0;
}