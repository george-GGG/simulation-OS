int create(char* PID, char priority){
	int i;
	PCB* p;//指向新创建的PCB
	inode* q;//用于遍历各wlist类型的链表
	inode* add;//指向新建的wlist节点

	p=(PCB*)malloc(sizeof(PCB));//创建新PCB
	//初始化PCB
	strcpy(p->PID,PID);//PID
	p->priority=priority;//priority
	p->status='1';
	p->list= 0;
	p->parent=running;//将正在运行的进程设为父进程
	//将此进程加入父进程的子进程列表
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;
	add->next=NULL;
	q= running->pcb->children;//用q遍历running进程的子进程
	if (q==NULL) running->pcb->children=add;
	while(q!=NULL){
		if (q->next==NULL) {q->next=add;break;}
		else q=q->next;
	}//插入children对列末尾

	p->children=NULL;
	for (i=0;i<4;i++){
		p->occupied_resource[i]=0;
		p->waiting_resource[i]=0;}

	//添加进ready_list,需要重新建立一个inode节点
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;//ready_list节点指向PCB,一一对应******
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

int kill_tree(struct PCB* p){
    //kill_tree
}
