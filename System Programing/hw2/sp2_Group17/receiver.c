#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

int isEnd = 0;
#define BUFFER_LENGTH 1024

// reference : http://stackoverflow.com/questions/3756323/getting-the-current-time-in-milliseconds
long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

// reference : http://www.paulgriffiths.net/program/c/srcs/echoservsrc.html
void threadMainFunction(void *data){
	int port = *((int*)data);
	int listenSocket;
	int connectedSocket;
	struct    sockaddr_in servaddr;  /*  socket address structure  */
	char buffer[BUFFER_LENGTH];
	FILE *pFile;

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);
	

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%d.txt", port);
	pFile = fopen(buffer, "w");

	if ( (listenSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}


	if ( bind(listenSocket, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
		fprintf(stderr, "Error calling bind()\n");
		exit(EXIT_FAILURE);
	}

	if ( listen(listenSocket, 0) < 0 ) {
		fprintf(stderr, "Error calling listen()\n");
		exit(EXIT_FAILURE);
	}

	printf("listen port %d\n", port);

	if ( (connectedSocket = accept(listenSocket, NULL, NULL) ) < 0 ) {
		fprintf(stderr, "Error calling accept()\n");
		exit(EXIT_FAILURE);
	}

	printf("accpet port %d\n", port);


	while ( isEnd == 0 ) {
		int length = recv(connectedSocket, buffer, BUFFER_LENGTH, 0);
		if(length > 0){
			fprintf(pFile, "%lld\n", current_timestamp());
		}
	}
	fclose(pFile);
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

	getchar();
	isEnd = 1;

	for(int i = 0; i < 3; i++){
		pthread_join(p_thread[i], (void **)&status);
	}
	return 0;
}