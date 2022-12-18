// Christopher Cao
// ch282858
// COP4600 - Fall 2022

#include <stdio.h>
#include "udp.h"

#define BUFFER_SIZE (1000)

// client code
int main(int argc, char *argv[]) 
{
	struct sockaddr_in addrSnd, addrRcv;
	struct timeval timeout;
	
	timeout.tv_sec = 30;
	timeout.tv_usec = 10;
	
	int sd = UDP_Open(20000);
	int rc = UDP_FillSockAddr(&addrSnd, "localhost", 10000);
	
	char message[BUFFER_SIZE];
	sprintf(message, "hello world");
	
	printf("client:: send message [%s]\n", message);
	printf("client:: waiting for reply\n");
	
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
	
	while (1)
	{
		rc = UDP_Read(sd, &addrRcv, message, BUFFER_SIZE);
		rc = UDP_Write(sd, &addrSnd, message, BUFFER_SIZE);
		
		if (rc < 0) 
		{
			printf("client:: failed to send\n");
			continue;
		}
		
		else
		{
			break;
		}
	}
	
	printf("client:: got reply [size:%d contents:(%s)\n", rc, message);
	
	return 0;
}
