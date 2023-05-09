#include "../include/client.h"


int _scanf()
{
	int value;
	while(1)
	{
		int retval = scanf("%d",&value);
		
		if(retval==1 && value>=0 && value<=2 && getchar()=='\n')
		{
			return value;
		}
		else printf("bad input!\n");
		
		while(getchar()!='\n');
	}
}


/*
	rcv:读取服务器消息并处理
*/
void *rcveive_handler(void *arg)
{
	int socket = *((int *)arg);
	
	while(1)
	{
		char msg[1024]={0};
		
		int size = read(socket,msg,1024);		
		if(size == 0)
		{
			printf("服务器断开连接...\n");
			close(socket);
			exit(0);
			//pthread_exit(NULL);
		}
		
		//取出消息类型
		int type = msg[0];
		printf("type = %d\n",type);
		
		
		if(type == MSG_FORWARDING)	//和好友聊天	type@dest_scoket@t_msg
		{
			//type alllength srciplen ip srcportlen port textlen text
			char *first = strstr(msg,"@");		//第一个@
			char *second = strrchr(msg,'@');	//第二个@
			char fri_name[50]={0};
			strncpy(fri_name,first+1,(int)(second - first - 1));	//提取名字
			
			char *text = second+1;		//拿出消息正文

			printf("%s:%s\n",fri_name,text);
			
			char tmp[1024]={0};
			sprintf(tmp,"%s: %s",fri_name,text);

		}
		else if(type == ONLINE_FEEDBACK)		//返回的是列表	GET@列表
		{
			char *p = msg+1;			//取出列表
			system("clear");
			printf("--------在线好友---------\n%s\n",p);
		}
	}
}


int main(int argc,char *argv[])
{
	if(argc != 4)
	{
		printf("参数错误  ./client  服务器ip  服务器端口 你的名字\n");
		return 0;
	}
	
	//创建socket
	int client_socket = socket(AF_INET,SOCK_STREAM,0);
		if(client_socket < 0)
		{
			perror("socket err:");
			return 0;
		}
	
	//设置服务器属性
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	//连接小区服务器
	int retval = connect(client_socket,(struct sockaddr *)&server_addr,sizeof(server_addr));
	
	if(retval < 0)
	{
		perror("connect community err");
		return 0;
	}

	
	//给社区服务器发送上线消息
	char login[50]={0};
	sprintf(login,"login %s",argv[3]);
	write(client_socket,login,strlen(login));

	//开线程去读消息
	pthread_t tid,tid1;
	pthread_create(&tid,NULL,rcveive_handler,&client_socket);	//读消息
	pthread_detach(tid);
	
	//发消息
	while(1)
	{
		printf("1.在线列表  2.唠嗑   0.夯机\n");
		
		int select = _scanf();
		
		if(select == 1)				//在线列表
		{
			char type[20]={0};
			sprintf(type,"%d",GET_ONLINE_LIST);			
			write(client_socket,type,strlen(type));		//获取一下列表
			usleep(100000);		//让出时间片先打印列表
		}
		else if(select == 2)
		{
			//sndto_serv(client_socket);
		}
		else			//夯机
		{
			close(client_socket);
			exit(0);
		}	
	}
}
