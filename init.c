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