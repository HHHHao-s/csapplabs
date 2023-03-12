#include "csapp.h"


int main(){

    struct tester{
        u_int32_t type;
        u_int32_t data; 
        u_int32_t info;
    };
    struct tester test2 = { 0xffeeddcc,0xff00ee11,0x00112233};
    int clientfd = open_clientfd("127.0.0.1",10000);

    
    // fflush(clientfd);
    Close(clientfd);

    
    // struct sockaddr_in clientaddr;
    // socklen_t len;
    // while(1){
    //     int connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &len);
    //     rio_t rio;
    //     rio_readinitb(&rio,connfd);
    //     rio_readn(connfd,&test1,sizeof(test1));
    //     printf("%u %u %u", test1.type,test1.data,test1.info);
    //     Close(connfd);
    // }
    
}