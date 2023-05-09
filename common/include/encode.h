#ifndef _ENCODE_H__
#define _ENCODE_H__

int packet_encode(unsigned char *buf, int length);
int packet_decrypt_encode(const unsigned char *buf, int *length);


#endif