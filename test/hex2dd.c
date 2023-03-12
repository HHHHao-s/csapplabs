#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char **args){

    
    if(argc>1){
        char out[100];
        char num[100];
        uint32_t a=0;
        uint32_t b=0;
        sscanf(args[1],"%p", &a);
        b = ntohl(a);
        char* no = inet_ntop(AF_INET, &b, out, 100);

        if(no){
            printf("%s", no);
        }
    }
    return 0;
}