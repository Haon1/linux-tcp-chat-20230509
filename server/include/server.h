#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>

struct client
{
	char name[20];
	int code;						/*	序号			*/
	int socket;						/*	socket 描述符	*/
	struct sockaddr_in addr;		/*	客户地址		*/
	
	struct client *next;			/*	链表			*/
};

typedef enum
{
	GET_ONLINE_LIST,		// 0x00 获取在线列表
	MSG_FORWARDING,			// 0x01 消息转发
	ONLINE_FEEDBACK,		// 0x02 服务器返回信息
}Operater_type;

extern int people;
extern struct client *list_head;	//客户端链表头



/*			函数声明			*/
void *receive_handler(void *arg);


struct client *init_client(char *name,int socket,struct sockaddr_in addr);
void add_client(char *name,int socket,struct sockaddr_in addr);
void del_client(int socket);
void show_client();
void recode();

struct client *get_client_bycode(int code);		//序号获取，主动发需要
struct client *get_client_byname(char *name);	//通过名字获取，转发时获取目标客户需要
struct client *get_client_bysock(int socket);	//通过socket获取，转发获取源客户需要

void putmsg(char *msg);
char *get_list();


int Scan();
char *mystrcat(char *dest,const char *src,int num);




#endif