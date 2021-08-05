       #include <sys/socket.h>
       #include <netinet/in.h>
       #include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define BUFLEN 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd) {
	unsigned int buff = (unsigned int)((unsigned int)((rand()%4) * 1000000000) + (rand()%10) * 100000000 + (rand()%10) *
				 10000000 + (rand()%10) * 1000000 + (rand()%10) * 100000 + (rand()%10) * 10000 + (rand()%10) *
				 1000 + (rand()%10) * 100 + (rand()%10) * 10 + rand()%10);
	write(sockfd, &buff, sizeof(buff));
	read(sockfd, &buff, sizeof(buff));
}

#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))

int main(int argc, char *argv[])
{
	struct timespec ts1, ts2;
	int sockfd, connfd, opt=1, reps;
	struct sockaddr_in servaddr, cli;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(MAX(1,MIN(65535,atol(argv[2]))));

	reps = MAX(1, atoi(argv[3]));

	clock_gettime(CLOCK_MONOTONIC_RAW, &ts1);
	while(reps--) {
		
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
			perror("socket");
			exit(0);
		}
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof opt);
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof opt);
		
		if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
			perror("c: connect");
			exit(0);
		}
		func(sockfd);
		close(sockfd);
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts2);
	if(ts2.tv_nsec < ts1.tv_nsec) {
		ts2.tv_nsec += 1000000000;
		ts2.tv_sec--;
	}
	printf("total %ld.%09ld\n", (long)(ts2.tv_sec - ts1.tv_sec), ts2.tv_nsec - ts1.tv_nsec);
	return 0;
}
