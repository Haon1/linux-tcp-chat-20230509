#include <math.h>

int packet_encode(unsigned char *buf, int length)
{
    int bytes = 0;
 
    do {
        char d = length % 128;
        length /= 128;
        if (length > 0) {
            d |= 0x80;
        }
        buf[bytes++] = d;
    } while (length > 0);
    return bytes;
}

int packet_decrypt_encode(const unsigned char *buf, int *length)
{
	int bytes = 0;
	char d;
	*length = 0;

	
	while(1)	
	{
		d = buf[bytes];
		
		if(!(d & 0x80))
			break;
		
		*length += (d^0x80) * pow(128,bytes++);
	}

	*length += buf[bytes] * pow(128,bytes);
	bytes++;
	
	return bytes;
}