#include"global.h"

int create(char* PID, char priority){
	int i;
	PCB* p;//指向新创建的PCB
	inode* q;//用于遍历各inode类型的链表
	inode* add;//指向新建的inode节点

	p=(PCB*)malloc(sizeof(PCB));//创建新PCB
	//初始化PCB
	strcpy(p->PID,PID);//PID
	p->priority=priority;//priority
	p->status='1';//ready
	p->list= 0; //ready list
	p->parent=running;//将正在运行的进程设为父进程
	//创建索引节点并加入com_list
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;
	add->next=NULL;
	q= comp_list;//用q遍历running进程的子进程
	while(q->next!=NULL){
		q=q->next;
	}
    q->next=add;

	//将此进程加入父进程的子进程列表
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;
	add->next=NULL;
	q= running->pcb->children;//用q遍历running进程的子进程
	while(q->next!=NULL){
		q=q->next;
	}
    q->next=add;//插入children对列末尾
	//孩子节点
	p->children=(inode*)malloc(sizeof(inode));
    p->children->next=NULL;//孩子队列带头节点
	//资源初始化
	for (i=0;i<4;i++){
		p->occupied_resource[i]=0;
		p->waiting_resource[i]=0;}

	//添加进ready_list,需要重新建立一个inode节点
	add=(inode*)malloc(sizeof(inode));
	add->pcb=p;//ready_list节点指向PCB,一一对应******
	add->next=NULL;
	if (priority=='0'){
		q=ready_list[0];
		while(q->next!=NULL){
            q=q->next;
        }
        q->next=add;
	}
	else if (priority=='1'){
		q=ready_list[1];
		while(q->next!=NULL){
            q=q->next;
        }
        q->next=add;
	}
	else if (priority=='2'){
		q=ready_list[2];
		while(q->next!=NULL){
            q=q->next;
        }
        q->next=add;
	}
	else return 0;
	schedule();
}

int destroy(char* PID){//将所有子进程都杀掉之后才schedule//***********未完成update
	//从com_list中找到对应inode（保存该inode，以便最后free pcb），进而找到进程pcb
	//将pcb在各个队列中的inode找到并删除
	//回收被该进程占用的资源
	//对pcb的子进程调用destroy
	//free（inode），回收该pcb的内存
	//schedule()

}

int kill_tree(struct PCB* p){
    //kill_tree
}
