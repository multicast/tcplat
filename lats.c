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

// Function designed for chat between client and server.
void func(int sockfd) {
	unsigned int buff;
	read(sockfd, &buff, sizeof(buff));
	printf("%ul ");
	buff = (unsigned int)((unsigned int)((rand()%4) * 1000000000) + (rand()%10) * 100000000 + (rand()%10) *
                                 10000000 + (rand()%10) * 1000000 + (rand()%10) * 100000 + (rand()%10) * 10000 + (rand()%10) *
                                 1000 + (rand()%10) * 100 + (rand()%10) * 10 + rand()%10);
	write(sockfd, &buff, sizeof(buff));
}

#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))

// Driver function
int main(int argc, char *argv[])
{
	struct timespec ts1, ts2;
	int sockfd, connfd, len, opt=1, reps;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("server: socket: ");
		exit(0);
	}

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof opt);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof opt);
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(MAX(1,MIN(65535,atol(argv[1]))));

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		perror("server: bind: ");
		exit(0);
	}

	// Now server is ready to listen and verification
	if ((listen(sockfd, 1111)) != 0) {
		perror("server: listen");
		exit(0);
	}
	len = sizeof(cli);
	reps = MIN(1, atoi(argv[2]));
	
	// time
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts1);
	while(1) {
		int g=reps;
		if(!g--) { printf("."); fflush( stdout ); g=reps; }

		connfd = accept(sockfd, (SA*)&cli, &len);
		if (connfd < 0) {
			perror("server: acccept failed:");
			exit(0);
		}
		func(connfd);
		close(connfd);
	}
	close(sockfd);

	clock_gettime(CLOCK_MONOTONIC_RAW, &ts2);
	if(ts2.tv_nsec < ts1.tv_nsec) {
		ts2.tv_nsec += 1000000000;
		ts2.tv_sec--;
	}
	printf("total %ld.%09ld\n", (long)(ts2.tv_sec - ts1.tv_sec), ts2.tv_nsec - ts1.tv_nsec);
	return 0;
}
