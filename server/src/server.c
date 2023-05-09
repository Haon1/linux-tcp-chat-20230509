#include "../include/server.h"


int people = 0;						//当前连接服务端数量
struct client *list_head = NULL;	//链表头

/*
	rcv：读取并处理客户消息
*/
void *receive_handler(void *arg)
{
	int client_socket = *((int *)arg);		//取出客户端 socket
	
	//通过socket 获取客户ip信息
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	getpeername(client_socket,(struct sockaddr *)&addr,&addrlen);		//不用的话不知道用户地址
	
	
	//处理连接信息
	char login[1024]={0};
	read(client_socket,login,1024);
	char this_name[50]={0};
	sscanf(login,"login %s",this_name);
	//add_client(this_name,socket,addr);
	//recode();
	people++;
	
	//struct client *p = get_client_bysock(client_socket);	//通过socket获取该客户信息
													//不用的话不知道名字
	//printf("\n[%d] %s -->fd:%d  ip:%s  port:%d  接入\n\n",people,p->name,socket,\
														inet_ntoa(p->addr.sin_addr),\
														ntohs(p->addr.sin_port));
	
	//读取消息
	while(1)
	{
		char msg[4096]={0};
		
		int size = read(client_socket,msg,4096);
		if(size == 0)
		{	
			//printf("%s ip:%s 掉线\n",p->name,inet_ntoa(p->addr.sin_addr));
			
			close(client_socket);											
			//del_client(client_socket);
			//recode();
			pthread_exit(NULL);
		}
		
		int type = msg[0];		//提出消息类型
		//int type;
		//sscanf(msg,"%d@",type);
		//printf("type = %d\n",type);
		//printf("read size = %d\n",size);
		

		if(type == MSG_FORWARDING)	
		{
			char *first = strstr(msg,"@");		//第一个@
			char *second = strstr(first+1,"@");	//第二个@

			char dest_name[100]={0};
			strncpy(dest_name,first+1,(int)(second-first)- 1);	//提取接收方名字
			
			char *text = second+1;		//拿出消息正文
			
			//printf("%s要发给%s\n",p->name,dest_name);
			//struct client *rcv_client = get_client_byname(dest_name);	//获取目的socket
			
			char trans[1024]={0};
			//sprintf(trans,"%d@%s@%s",MSG_FORWARDING,p->name,text);
			//write(rcv_client->socket,trans,strlen(trans));					//转发
		}
		else if(type == GET_ONLINE_LIST)		//3 要获取列表	type
		{
			//char *list = get_list();	//获取本地列表,get里面申请堆空间
			//char get[1024]={0};
			//sprintf(get,"%d@%s",ONLINE_FEEDBACK,list);
			
			//write(socket,get,strlen(get));	//发给客户端
			
			//free(list);
		}
	}
}

struct client *init_list_head()
{
	//申请堆空间并初始化
	struct client *list_head = (struct client *)malloc(sizeof(struct client));
	memset(list_head,0,sizeof(list_head));
	list_head->next = NULL;
	return list_head;
}

int info_2_file(int socket,struct sockaddr_in *server_addr)
{
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	//getpeername(socket,(struct sockaddr *)&addr,&addrlen);		//不用的话不知道用户地址
	//getsockname(socket,(struct sockaddr *)&addr,&addrlen);
	struct hostent *info = gethostbyname("Haon");
	char buf[100]={0};
	for(int i=0;info->h_addr_list[i];i++)
	{
		printf("%s\n",inet_ntop(info->h_addrtype,info->h_addr_list[i],buf,100));
	}
	FILE *fp = fopen("./1.txt","w+");
	if(fp == NULL)
		return -1;

	fprintf(fp, "%s %d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
	fclose(fp);
}

int main(int argc, char *argv[])
{
	list_head = init_list_head();
	
	//创建 socket
	int server_socket = socket(AF_INET,SOCK_STREAM,0);
	if(server_socket < 0)
	{
		fprintf(stderr,"%s","socket err");
		perror("");
		return -1;
	}
	
	//设置端口复用
	int on = 1;
	setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
	//设置服务器 ip 信息
	struct sockaddr_in server_addr,lisetn_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	//绑定信息
	int retval = bind(server_socket,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(retval < 0)
	{
		fprintf(stderr,"%s","bind err");
		perror("");
		
		close(server_socket);
		return 0;
	}
	
	//监听
	retval = listen(server_socket,1023);
	if(retval < 0)
	{
		fprintf(stderr,"%s","listen err");
		perror("");
		
		close(server_socket);
		return 0;
	}
	
	retval = info_2_file(server_socket,&server_addr);
	if(retval < 0)
	{
		fprintf(stderr,"%s","info_2_file err ");
		perror("");
		
		close(server_socket);
		return 0;
	}
	
	printf("等待客户端连接...\n");
	
	
	//等待客户端连接
	while(1)
	{
		int client_socket = accept(server_socket,NULL,NULL);
		
		if(client_socket > 0)
		{
			pthread_t tid;
			pthread_create(&tid,NULL,receive_handler,&client_socket);
			pthread_detach(tid);
		}
		else printf("连接失败\n");
		
	}
}