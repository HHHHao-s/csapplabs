#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



int main(int arg, char *args[]){

    if(arg != 2){
        return -1;
    }
    
    struct stat tep_stat;
    

    int no = stat(args[1], &tep_stat);

    if(no<0){
        return -1;
    }

    off_t size = tep_stat.st_size;

    int fp = open(args[1],O_RDONLY, 0);

    if(fp<0){
        return -1;
    }
    
    void * bufp = mmap(NULL,size,PROT_READ,MAP_PRIVATE,fp,0);
    write(STDOUT_FILENO,bufp,size);


    return 0;
}
