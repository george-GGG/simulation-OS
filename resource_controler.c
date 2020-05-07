int request(char* PID, char* RID){

}

int rfree(struct PCB* p){
	//将p占用的资源释放，source_list列表相应资源值增加
	int i;
	for (i=0;i<4;i++)
		resource_list[i]+=p->resource[i];
}

int release(char* RID, int n, struct RCB* resource_list, struct wlist* ready_list, struct PCB** running_process, struct PCB* running){

}