
#include "csapp.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define STRING_LENGTH 1024
/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";





int parseFir(char *in, char *method, char *host, char *port , char *location){ // return 1 if method ==1
    char method[STRING_LENGTH], location[STRING_LENGTH],desc[STRING_LENGTH];
    sscanf(in, "%s http://%[^/]%s %s",method,host,location,desc);
    sscanf(host,"%[^:]:%s", host,port); // in case with port
    if(port[0]=='\0'){
        strcpy(port,"80");
    }
    if(strcmp("GET", method)==0) return 1;
    else return 0;
}

void parseHeader(char *in, char *Header, char *info){  
    sscanf(in ,"%[^:]: %[^\r]\r\n",Header,info);
}


void Doit(int clientfd){
    rio_t rio;
    Rio_readinitb(&rio, clientfd);

    char remote_location[MAXLINE], client_request[MAXBUF], remote_host[MAXLINE],remote_method[MAXLINE],remote_port[MAXLINE];
    char buf[MAXBUF];
    Rio_readlineb(&rio, client_request, MAXBUF);
    int rc = parseFir(client_request,remote_method, remote_host,remote_port, remote_location);

    if(rc == 0){ // not get, just sent

        sprintf(buf,"%s %s HTTP/1.0\r\n", remote_method, remote_location);
        sprintf(buf,"%sHost: %s\r\n",buf, remote_host);
        sprintf(buf,"%s%s",buf, user_agent_hdr);
        sprintf(buf,"%sConnection: close\r\n", buf);
        sprintf(buf,"%sProxy-Connection: close\r\n", buf);
    }else{
        sprintf(buf,"%s %s HTTP/1.0\r\n", remote_method, remote_location);
        sprintf(buf,"%sHost: %s\r\n",buf, remote_host);
        sprintf(buf,"%s%s",buf, user_agent_hdr);
        sprintf(buf,"%sConnection: close\r\n", buf);
        sprintf(buf,"%sProxy-Connection: close\r\n", buf);      
    }

    int remotefd = Open_clientfd(remote_host, remote_port);


}

int main(int argc, char **argv)
{
    
    if(argc != 2){
        printf("%s", argv[0]);
    }
    char hostname[MAXLINE], port[MAXLINE];
    int listenfd = Open_listenfd(argv[1]);
    while (1){
        struct sockaddr_storage clientaddr;
        int addrlen = sizeof(clientaddr);
        int clientfd = Accept(listenfd, (SA *)&clientaddr, &addrlen);      
        Getnameinfo((SA *) &clientaddr, addrlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        Do(clientfd);
    }
    

    char out[STRING_LENGTH];
    parseFir(out);
    printf("%s", out);
    return 0;
}
