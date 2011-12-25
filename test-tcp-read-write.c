#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>

int tcpfd;




int recvdata()
{

	int listenfd,connfd,ret,n;
	socklen_t clilen;
	struct sockaddr_in cli_addr,srv_addr;
	memset(&cli_addr,0,sizeof(cli_addr));
	memset(&srv_addr,0,sizeof(srv_addr));
	/* set listen addr */
	srv_addr.sin_family=AF_INET;
	srv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	srv_addr.sin_port=htons(4444);
	
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd==-1) {
		
		return -1;
	}
	int on=1;
	ret = setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );		
	if(ret==-1) {
		
		return -1;
	}
	
	ret=bind(listenfd,(struct sockaddr *)&srv_addr,sizeof(srv_addr));
	if(ret==-1) {
		
		return -1;
	}
	
	ret=listen(listenfd, 11);
	if(ret==-1) {
		
		return -1;
	}
	


	clilen=sizeof(cli_addr);
	
	char recvbuf[202400]={0};

	connfd=accept(listenfd,(struct sockaddr *)&cli_addr,&clilen);
	if(connfd==-1) {
		
		perror("accept:");
	}
		
		
	while( (n=read(connfd,recvbuf,sizeof(recvbuf)-1)) )
	{
		printf("%s##%d##\n",recvbuf,n);
		memset(recvbuf,0,sizeof(recvbuf));
		sleep(5);
	}
	printf("##%d##\n",n);
		
		
	
	return 0;
}

int senddata(const char *ip)
{

	int ret,len;

	/* ignore SIGPIPE */
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGPIPE);
	if (sigaction(SIGPIPE, &sa, 0)) {
	    perror("failed to ignore SIGPIPE");
	    exit(-1);
	}

	/* set server address */
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	inet_aton(ip,&server_addr.sin_addr);
	server_addr.sin_port=htons(4444);		
	
	len=sizeof(server_addr);


	tcpfd=socket(AF_INET,SOCK_STREAM,0);
	if(tcpfd==-1) {
		perror("socket");
		exit(-1);
	}
	ret=connect(tcpfd,(struct sockaddr *)&server_addr,len);
	if(ret==-1) {
		perror("connect");
		exit(-1);
	}
	
	
	char sendbuf[1024]={0};
	long i=0,n;
	while(1) {
		sprintf(sendbuf,"%ld-0-----5-----10-----15-----20-----25-----30-----35-----40-----45-----50-----55-----60",i);
		n=write(tcpfd,sendbuf,strlen(sendbuf));
		if(n<0) {
			perror("write");
			break;
		}
		printf("%s##%d##\n",sendbuf,n);
		memset(sendbuf,0,sizeof(sendbuf));
		i++;
		//usleep(1000);
	}

}


void sig_hanle(int no)
{
	printf("shutdown()\n");
	shutdown(tcpfd,SHUT_RDWR);
	return ;
}

int main(int argc, char **argv)
{


	struct sigaction sa;
	sa.sa_flags=0;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGINT);
	sa.sa_handler=sig_hanle;
	if ( sigaction(SIGINT,&sa,NULL)<0 ) {
	
		return -3;
	}

	switch(atoi(argv[2])){
	case 0:
		recvdata();
		break;
	case 1:
		senddata(argv[1]);
		break;
	default:
		;
	}
	
	return 0;
	
}	
	
