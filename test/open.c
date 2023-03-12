#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>


int open_clientfd(char *hostname, uint16_t port){
    int clientfd;
    struct sockaddr_in serveraddr;
    struct hostent *hp;

    if(clientfd = socket(AF_INET,SOCK_STREAM,0)<0){
        return -1;
    }
    if(hp=gethostbyname(hostname)==NULL){
        return -2;
    }
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy(hp->h_addr_list[0],&serveraddr.sin_addr.s_addr,hp->h_length);
    serveraddr.sin_port=htons(port);

    if(connect(clientfd,&serveraddr,sizeof(serveraddr))<0){
        return -1;
    }
    return clientfd;

}


int open_listenfd(u_int16_t port){
    int listenfd,optval=1;
    if(listenfd = socket(AF_INET,SOCK_STREAM,0) <0){
        return -1;
    }

    struct sockaddr_in serveraddr;
/* Eliminates "Address already in use" error from bind. */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
		   (const void *)&optval , sizeof(int)) < 0)
	return -1;
    
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htns(port);

    if(bind(listenfd,&serveraddr,sizeof(serveraddr))<0){
        return -1;
    }
    if(listen(listenfd,1024))
    return -1;

    return listenfd;

}


int main(){

    

    return 0;
}