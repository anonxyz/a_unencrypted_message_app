#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#define PORT "80"

#define MAXSIZE 100
void *get_in_addr(struct sockaddr *sa)
{
if(sa->sa_family==AF_INET)
{
return &(((struct sockaddr_in*)sa)->sin_addr);
}
else{
return &(((struct sockaddr_in6*)sa)->sin6_addr);

}
}

int main(int argc,char * argv[])
{
int rv,sockfd,recvbytes;
struct addrinfo hints,*servinfo,*p;
char s[INET6_ADDRSTRLEN];
char buf[MAXSIZE];
if (argc!=2)

{
printf("usage : client.exe hostname");
return 1;
}
memset(&hints,0,sizeof hints);
hints.ai_family=AF_UNSPEC;
hints.ai_socktype=SOCK_STREAM;
if((rv=getaddrinfo(argv[1],PORT,&hints,&servinfo)!=0))
{
perror("getaddrinfo");
return 1;
}
for(p=servinfo;p!=NULL;p=p->ai_next)
{
if ((sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1)
{
perror("socket");
continue;
}
if(connect(sockfd,p->ai_addr,p->ai_addrlen)==-1)
{
close(sockfd);
perror("connect");
continue;

}
break;
}
if(p==NULL)
{
printf("p is NULL");
return 1;
}

inet_ntop(p->ai_family,get_in_addr((struct sockaddr *)p->ai_addr),s,sizeof s);

printf("client connecting to %s",s);
freeaddrinfo(servinfo);
if((recvbytes=recv(sockfd,buf,MAXSIZE-1,0))==-1)
{
perror("recv");

}
buf[recvbytes]='\0';
printf("\nclient received %s",buf);
close(sockfd);
return 0;
}
