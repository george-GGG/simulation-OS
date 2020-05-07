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
			if (usrcmd[0]==' ') {printf("Illegal comand!");continue;}
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