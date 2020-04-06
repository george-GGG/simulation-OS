#include <stdio.h>
#include <string.h>

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
        //printf("%s",usrcmd);
		//recognise	
		judge=judgeSpace(usrcmd);
		if (judge==1){
			if (usrcmd[0]==' ') {printf("Illegal comand!\n");continue;}
			else
				cmd=strtok(usrcmd," ");}
		//bug预警***cmd为usrcmd第一个空格前的部分
		else if(judge==0)
            //printf("judge=0\n");
			strcpy(cmd,usrcmd);

		if (!strcmp(cmd,"init"))
			//init();
            printf("init successly\n");

		else if (!strcmp(cmd,"cr")){
			para1=strtok(NULL," ");
			para2=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			} 
			//call function
			//create(PID,priority);
            printf("created process: %s %s",para1,para2);
		}
		else if (!strcmp(cmd,"list")){
			para1=strtok(NULL," ");//继续分割usrcmd
			if (!strcmp(para1,"ready"))
				printf("list of ready processes\n");//call func
			else if (!strcmp(para1,"block"))
				printf("list of blocked processes\n");//call func
			else if (!strcmp(para1,"res"))
				printf("list of resources processes\n");//call func
			else {printf("parameter error\n"); continue;}
		}
		
		else if (!strcmp(cmd,"to"))
			printf("time out\n");//call function
		else if (!strcmp(cmd,"req")){
			para1=strtok(NULL," ");
			para2=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			}
			printf("func req\n");
			//call function
		}
		else if (!strcmp(cmd,"rel")){
			para1=strtok(NULL," ");
			para2=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			}
			printf("func rel\n");
			//call function
		}
		else if (!strcmp(cmd,"de")){
			para1=strtok(NULL," ");
			if (para1==NULL){
				printf("parameter error\n");
				continue;
			}
			//strcpy(PID,para1);
			printf("func de\n");//call function
		}
		else if (!strcmp(cmd,"exit"))
			exit(0);
		else  
		   {printf("无此命令,请重新输入\n");continue;}

	}
	return 0;
}
int main(){
    test_shell();
    return 1;
}