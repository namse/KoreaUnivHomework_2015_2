#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define BUFFER_LENGTH 1024

// reference : http://www.paulgriffiths.net/program/c/srcs/echoservsrc.html
void threadMainFunction(void *data){
	int port = *((int*)data);
	int connectingSocket;
	struct    sockaddr_in servaddr;
	char buffer[BUFFER_LENGTH];
	FILE *pFile;

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%d.txt", port);
	pFile = fopen(buffer, "w");
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("192.168.56.3");
	servaddr.sin_port        = htons(port);


	if ( (connectingSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}

	if ( connect(connectingSocket, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
		fprintf(stderr, "Error calling connect() %d\n", errno);
		exit(EXIT_FAILURE);
	}

	printf("connected port %d\n", port);

	while ( 1 ) {
		write(connectingSocket, "A", sizeof("A"));
		usleep(1000);
	}

	return;
}


// reference : http://www.joinc.co.kr/modules/moniwiki/wiki.php/Site/Thread/Beginning/PthreadApiReference
int main(){
	pthread_t p_thread[3];
	int threadID;
	int status;
	int port[3] = {9001, 9002, 9003};

	for(int i = 0; i < 3; i ++){
		threadID = pthread_create(&p_thread[i], NULL, threadMainFunction, (void *)&port[i]);
		if (threadID < 0)
		{
			perror("thread create error : ");
			exit(0);
		}
		printf("thread for port %d created\n", port[i]);
	}

	for(int i = 0; i < 3; i++){
		pthread_join(p_thread[i], (void **)&status);
	}
	return 0;
}