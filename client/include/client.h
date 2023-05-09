#ifndef _CLIENT_H
#define _CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

typedef enum
{
	GET_ONLINE_LIST,		// 0x00 获取在线列表
	MSG_FORWARDING,			// 0x01 消息转发
	ONLINE_FEEDBACK,				// 0x02 服务器返回信息
}Operater_type;


//void sndto_other_client(int socket);


#endif