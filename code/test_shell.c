#include"global.h"
#include<string.h>
int judgeSpace(char* str){//判断输入语句是否有空格
	int i;
	for (i=0;i<strlen(str);i++)
		if (str[i]==' ') return 1;
	if (i==strlen(str)) return 0;
	else return -1;
}

int disp_ready(){
	int i;
	inode *p;
	printf("ready list:\n");
	for (i=2;i>=0;i--){
		printf(" %d: ",i);
		p=ready_list[i]->next;
		while(p!=NULL){
			printf("-%s",p->pcb->PID);
			p=p->next;
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

int disp_block(){
	int i;
	inode *p;
	printf("block list:\n");
	for (i=0;i<4;i++){
		printf(" R%d: ",i+1);
		p=resources[i].p->next;
		while(p!=NULL){
			printf("-%s",p->pcb->PID);
			p=p->next;
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

int disp_resource(){
	int i;
	printf("resources:\n");
	for (i=0;i<4;i++){
		printf(" R%d: ",i+1);
		printf("%d\n",resources[i].available_number);
	}
	return 0;
}

int test_shell(){//要调用进程管理和资源管理器，所以放在前两个函数后面
	char usrcmd[30];//record usr comand
	char* cmd;//输入字符串第一个空格之前的内容
	char* para1;//parameters
	char* para2;
	int judge;
	char PID[10];//process id
	int priority;
	int n;//number of resource
	printf("designed by GeorgeGGG\n");
	printf("type 'help' to get help,type 'exit' to exit\n\n");
	printf("********************************************\n");
	
	while(1){
		//input
		printf("shell> ");
		gets(usrcmd);
		//recognise	
		judge=judgeSpace(usrcmd);
		if (judge==1){
			if (usrcmd[0]==' ') {printf("Illegal comand!");continue;}
			else
				cmd=strtok(usrcmd," ");}
		//应该先处理无参数命令，再使用strtok函数分隔后处理含参命令
		else if(judge==0)
			//strcpy(cmd,usrcmd);
			cmd=usrcmd;//尝试直接赋值地址

		if (!strcmp(cmd,"init"))
			init();
		else if (!strcmp(cmd,"help"))
			printf("********************************************\n-cr <pid> <priority>    :create a process\n\
-de <pid>    :destroy a process and its children\n-exit    :exit\n-help    :help\n-init    :init the system and create the init process\n\
-list\n    block    :list the block processes\n    res    :list the resources available\n    ready    :list the ready processes\n\
-req <rid> <qauntity>    :running process request resource\n-rel <rid> <qauntity>    :running process release resource\n\
-to    :time out\n********************************************\n");
		else if (!strcmp(cmd,"cr")){
			para1=strtok(NULL," ");
			para2=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			}
			strcpy(PID,para1);
			if (*para2=='2') priority=2;
			else if (*para2=='1') priority=1;
			else if (*para2=='0') priority=0;
			else {printf("priority error\n"); continue;}
			//call function
			create(PID,priority);
		}
		else if (!strcmp(cmd,"list")){
			para1=strtok(NULL," ");//继续分割usrcmd
			if (!strcmp(para1,"ready"))
				disp_ready();//call func
			else if (!strcmp(para1,"block"))
				disp_block();//call func
			else if (!strcmp(para1,"res"))
				disp_resource();//call func
			else {printf("parameter error\n"); continue;}
		}
		
		else if (!strcmp(cmd,"to"))
			time_out();//call function
		else if (!strcmp(cmd,"req")){
			para1=strtok(NULL," ");
			para2=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error1\n");
				continue;
			}
			if (*para2=='4') n=4;
			else if (*para2=='3') n=3;
			else if (*para2=='2') n=2;
			else if (*para2=='1') n=1;
			else {printf("parameter error2\n"); continue;}
			request(para1,n);//call function
		}
		else if (!strcmp(cmd,"rel")){
			para1=strtok(NULL," ");
			para2=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error1\n");
				continue;
			}
			if (*para2=='4') n=4;
			else if (*para2=='3') n=3;
			else if (*para2=='2') n=2;
			else if (*para2=='1') n=1;
			else {printf("parameter error2\n"); continue;}
			release(running,para1,n);//call function
		}
		else if (!strcmp(cmd,"de")){
			para1=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			}
			strcpy(PID,para1);
			destroy(PID);//call function
		}
		else if (!strcmp(cmd,"exit"))
			exit(0);
		else  
		   {printf("command error, try again\n");continue;}
	}
	return 0;
}