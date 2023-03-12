#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char **args){


    if(argc>1){

        uint32_t a=0;
        uint32_t b=0;
                
        int no = inet_pton(AF_INET, args[1], &a);
        
        if(no>0){
            b = ntohl(a);
            printf("%p", b);
        }
    }
    return 0;
}