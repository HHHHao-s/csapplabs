#include "csapp.h"


int main(){

    struct tester{
        u_int32_t type;
        u_int32_t data; 
        u_int32_t info;
    } test1;
    
    int listenfd = Open_listenfd(10000);
    
    struct sockaddr_in clientaddr;
    socklen_t len;
    while(1){
        // printf("ready to accept\n");
        len = sizeof(clientaddr);
        int connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &len);
        // printf("accepted:");
        // rio_t rio;
        // rio_readinitb(&rio,connfd);
        rio_readn(connfd,&test1,sizeof(test1));
        
        printf("%x %x %x\n", test1.type,test1.data,test1.info);
        Close(connfd);
    }
    





}