
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define STRING_LENGTH 1024
/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";


#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_TITLE_SIZE 256

typedef struct _page
{

    size_t size;
    struct _page *pred, *succ; // the neighbour of this page
    char title[MAX_TITLE_SIZE];
    void *data;
} Page;

typedef struct
{
    size_t nleft;
    Page *recent, *least;
    sem_t write_lock;
} HPage;

void cache_print(HPage *ph);

void cache_init(HPage *ph)
{
    ph->nleft = MAX_CACHE_SIZE;
    Page *board = Malloc(sizeof(Page));
    board->size = strlen("Nothing");
    board->pred = NULL;
    board->succ = NULL;
    board->data = Malloc(board->size);
    strcpy(board->data, "Nothing");
    strcpy(board->title, "Nothing");
    ph->recent = ph->least = board;

    Sem_init(&ph->write_lock, 0, 1);
}

void cache_push(HPage *ph, char *title, void *cache_data, size_t size)
{ // there is no itself in the cache and use lru
    if (size > MAX_OBJECT_SIZE)
        return;
    else
    {
        P(&ph->write_lock);
        if (size > ph->nleft)
        { // have not enough space
            do
            {
                Page *temp = ph->least;
                ph->least = temp->succ;
                ph->least->pred = NULL;
                size_t desize = temp->size;
                Free(temp); // release the space
                ph->nleft += desize;
                // cache_print(ph);
            } while (size >= ph->nleft);
        }
        Page *add = Malloc(sizeof(Page));
        add->pred = ph->recent;
        ph->recent->succ = add;
        add->succ = NULL;
        add->size = size;
        add->data = cache_data;
        strcpy(add->title, title);
        ph->recent = add;
        ph->nleft -= size;
        printf("cache_push:%s\n", add->title);
        V(&ph->write_lock);
    }
}

void *cache_pull(HPage *ph, char *title, size_t *size) // get pointer to data, and set size
{

    P(&ph->write_lock);
    void *ret = NULL;
    for (Page *p = ph->recent; p; p = p->pred)
    {
        if (strcmp(p->title, title) == 0)
        {
            *size = p->size;

            if (p == ph->recent)
                ;                    // already recent
            else if (p == ph->least) // move it to the recently
            {

                ph->least = p->succ;
                ph->least->pred = NULL;

                p->succ = NULL;
                p->pred = ph->recent;
                ph->recent->succ = p;
                ph->recent = p;
            }
            else
            {
                Page *pa = p->pred;
                Page *pb = p->succ;
                pa->succ = pb;
                pb->pred = pa;
                ph->recent->succ = p;
                p->pred = ph->recent;
                p->succ = NULL;
                ph->recent = p;
            }
            ret = p->data;
            printf("cache_pull:%s\n", p->title);
            break;
        }
    }
    V(&ph->write_lock);
    return ret;
}

void page_print(Page *p)
{
    printf("self:%p\nsize:%zu\ntitle:%s\npred:%p\nsucc:%p\ndata:", p, p->size, p->title, p->pred, p->succ);

    fflush(stdout);
    Write(STDOUT_FILENO, p->data, 4); // only part of data
    putchar('\n');
}

void cache_print(HPage *ph)
{
    printf("whole size:%zu\n", MAX_CACHE_SIZE - ph->nleft);

    for (Page *p = ph->least; p != NULL; p = p->succ)
    {
        printf("title:%s\n", p->title);
        // page_print(p);
    }
}








void sigpipe_handler(int sig){
    unix_error("pipe error");
}





HPage ph;



int parseFir(char *in, char *method, char *host, char *port , char *location){ // return 1 if method ==1
    char desc[STRING_LENGTH],buf[STRING_LENGTH];
    sscanf(in, "%s http://%[^/|^ ]%s %s",method,buf,location,desc);
    
    sscanf(buf,"%[^:]%s", host,port); // in case with port
    
    if(port[0]=='\0'){
        strcpy(port,"80");
    }else{
        strcpy(buf, port+1);
        strcpy(port, buf);
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
    printf("%d\n", clientfd);
    char remote_location[MAXLINE], client_request[MAXBUF], remote_host[MAXLINE],remote_method[MAXLINE],remote_port[MAXLINE];
    
    Rio_readlineb(&rio, client_request, MAXBUF);
    int rc = parseFir(client_request,remote_method, remote_host,remote_port, remote_location);
    printf("received: %s", client_request);
    if(rc == 0){ // not get, just sent

        // char buf[MAXBUF];
        // while(Rio_readlineb(&rio, buf, MAXBUF)){
        //     sprintf(client_request, "%s%s", client_request, buf);
        // }
        // int remotefd = Open_clientfd(remote_host, remote_port);
        // Rio_writen(remotefd, buf, )

        //当tcp连接没关闭时，read会一直阻塞，read返回0时代表tcp连接关闭
        
    }else{
        size_t data_size;
        void *data;
        if((data=cache_pull(&ph, remote_location, &data_size))!=NULL){
            Rio_writen(clientfd, data, data_size);
        }else{
            char buf[MAXBUF];
            sprintf(buf,"%s %s HTTP/1.0\r\n", remote_method, remote_location);
            sprintf(buf,"%sHost: %s\r\n",buf, remote_host);
            sprintf(buf,"%s%s",buf, user_agent_hdr);
            sprintf(buf,"%sConnection: close\r\n", buf);
            sprintf(buf,"%sProxy-Connection: close\r\n", buf); 
            sprintf(buf,"%s\r\n", buf); 
            printf("%s", buf);
            int remotefd = Open_clientfd(remote_host,remote_port);// 向网站发送请求
            Rio_writen(remotefd, buf, strlen(buf));
            
            printf("write to remote done\n");  

            void *getbuf = Malloc(MAX_OBJECT_SIZE); // 方便插入到缓存中         
            int rec = Rio_readn(remotefd, getbuf, MAX_OBJECT_SIZE);
            if(rec>0){

                rio_writen(clientfd, getbuf, rec);
                Realloc(getbuf,rec);
                cache_push(&ph, remote_location, getbuf, rec);      
            }
            Close(remotefd);

        }
       
    }
    
    
    
    
    Close(clientfd);

}

typedef struct{
    int n, rear, front,size;
    int *fds;
    sem_t s_left, s_item, s_bin;

}sem_q_t;

void sem_q_init(sem_q_t *q, int size){// init a aysc queue
    q->size = q->n = size;
    q->rear = q->front = 0; // if == then no items
    q->fds = Malloc(size*sizeof(int));
    
    Sem_init(&q->s_bin, 0, 1);
    Sem_init(&q->s_left, 0, size);
    Sem_init(&q->s_item, 0, 0);

}

void sem_q_deinit(sem_q_t *q){   
    Free(q->fds);
    sem_close(&q->s_bin);
    sem_close(&q->s_left);
    sem_close(&q->s_item);
}


void sem_q_push(sem_q_t *q, int fd){
    P(&q->s_left);

    P(&q->s_bin);
    
    q->fds[q->rear] = fd;
    q->rear = (q->rear+1)%(q->size);
    V(&q->s_bin);

    V(&q->s_item);
    
}

int sem_q_rm(sem_q_t *q){
    P(&q->s_item);

    P(&q->s_bin);
    
    int fd = q->fds[q->front];
    q->front = (q->front+1)%q->size;
    V(&q->s_bin);

    V(&q->s_left);
    return fd;
}

void thread(void *p){
 
    sem_q_t *q = (sem_q_t *)p;
    while (1){
        int fd =sem_q_rm(q);
        Doit(fd);
    }
}


int main(int argc, char **argv)
{
    
    // char remote_location[MAXLINE], *client_request="GET http://www.cmu.edu/hub/index.html HTTP/1.1", remote_host[MAXLINE],remote_method[MAXLINE],remote_port[MAXLINE];
    // parseFir(client_request,remote_method, remote_host,remote_port, remote_location);
    // printf("%s %s %s %s", remote_method, remote_host,remote_port, remote_location);
    if(argc != 2){
        printf("%s", argv[0]);
    }else{
        Signal(SIGPIPE, sigpipe_handler);

        char hostname[MAXLINE], port[MAXLINE];
        int listenfd = Open_listenfd(argv[1]);
        sem_q_t q;
        sem_q_init(&q, 8);
        pthread_t pt;
        for(int i=0;i<8;i++){ // worker thread
            Pthread_create(&pt, NULL, thread, (void *) &q);
        }

        cache_init(&ph);

        while (1){
            struct sockaddr_storage clientaddr;
            int addrlen = sizeof(clientaddr);
            int clientfd = Accept(listenfd, (SA *)&clientaddr, &addrlen);
            printf("%d\n", clientfd);
            sem_q_push(&q, clientfd);      
            Getnameinfo((SA *) &clientaddr, addrlen, hostname, MAXLINE, 
                        port, MAXLINE, 0);          
            printf("Accepted connection from (%s, %s)\n", hostname, port);
        
        }
    }
    
    

    
    return 0;
}
