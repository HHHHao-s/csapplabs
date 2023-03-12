
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cachelab.h"

int g_miss=0,g_hit=0,g_eviction=0;


int s2i(char s[]){
//	printf("%s:", s);
	char *a = s;
	int ret=0;
	while(*a != 0){
		ret = ret*10 + (*a - '0');
		a++;
	}
//	printf("%d\n", ret);
	return ret;
}


typedef struct line{
	long flag;
	int control_pos;
	char isExist;
	
	
}Line,*pLine;

typedef struct lru_control{
	int par;
	int kid;
	
}*pControl,Control;


typedef struct set{
	pLine lines; // the array of line
	pControl controls; // the array of control for lru
	int least,most; // for the use of lru
	
}Set,*pSet;


void swap(pControl c1, pControl c2){
	Control ct = *c1;
	*c1 = *c2;
	*c2 = ct;
} 


int Adjust(pControl controls,pSet set, int most, int l){
	if(l == most){
		return l;
	}
	int t1 = controls[l].par;
	int t2 = controls[most].kid;
	int t3 = controls[l].kid;
	if(l!=t2){
		controls[l].kid = t2;
		controls[t2].par = l;	
	}
	if(t3!=t1){
		controls[t1].kid = t3;
		controls[t3].par = t1;
	}
	controls[most].kid = l;
	controls[l].par = most;
	set->most = l;
		
	
	
	
	
	return l;
}


int LRU(pControl controls,pSet set){
	int ret = set->least;
	controls[set->most].kid = set->least;
	controls[set->least].par = set->most;
	set->most = set->least;
	set->least = controls[set->least].kid;
	return ret;
}



int Access(long flag, pLine lines, pControl controls,pSet set, int E){ // 0->hit 1->miss 2->eviction
	int i=0;
	int ret=2;
	
	for(;i<E;i++){
		if(flag == lines[i].flag && lines[i].isExist == 1 ){			

			g_hit++;
			Adjust(controls, set,set->most, i);
			
			return 0;			
		}
	}
	
	// miss condition 
	int pos = LRU(controls,set);
	
	g_miss++;
		
	if(lines[pos].isExist == 0){ // miss
		ret = 1;
	}else{
		g_eviction++;		
		ret = 2;
	}
	
	
	lines[pos].flag = flag;
	lines[pos].isExist = 1;

	return ret;
}


void operate(char instruction, long add,long size, int s,int E,int b, int v, pSet sets){
	
	long flag=0,index=0; // flag, index of Set, offect of Block

	index = (add >> b) & ~(-1<<s);
	flag = add >> (b+s);
	
	
		if(instruction == 'L' || instruction == 'S'){
			int res = Access(flag, sets[index].lines,sets[index].controls,&sets[index],E);
			if(v==1){
				if(res == 0){
				printf("%c %lx,%ld hit\n", instruction, add, size);
				}else if(res == 1){
					printf("%c %lx,%ld miss\n", instruction, add, size);
				}else if(res == 2){
					printf("%c %lx,%ld miss eviction\n", instruction, add, size);
				}
			}
			
		}else if(instruction == 'M'){
			int res1 = Access(flag, sets[index].lines,sets[index].controls,&sets[index],E);
			int res2 = Access(flag, sets[index].lines,sets[index].controls,&sets[index],E);
			if(v==1){
					printf("%c %lx,%ld", instruction, add, size);
				if(res1 == 0 ){
					printf(" hit");
				}else if(res1 == 1){
					printf(" miss");			
				}else if(res1 == 2){
					printf(" miss eviction");
				}
			
				if(res2 == 0){
					printf(" hit\n");
				}else if(res2 == 1){
					printf(" miss\n");			
				}else if(res2 == 2){
					printf(" miss eviction\n");
				}
			}
						
		}
	
	
}






void sim(int s,int E,int b,int h, int v, char filename[]){
//	printf("%d %d %d %d %d %s",s,E,b,h,v,filename);
	
	int S = 1<<s;
	
	if(h == 1){
		printf("nothing here");
		return;
	}
	pSet sets = (pSet)malloc(sizeof(Set)*S); // array of set
	
	for(int i=0;i<S;i++){				
		sets[i].lines = (pLine)malloc(sizeof(Line)*E);
		sets[i].controls = (pControl)malloc(sizeof(Line)*E);
		sets[i].least = 0;
		sets[i].most = E-1;
		for(int j=0;j<E;j++){
			sets[i].lines[j].flag = 0;
			sets[i].lines[j].isExist = 0;			
			sets[i].controls[j].par = (j+E-1) % E;
			sets[i].controls[j].kid = (j+1) % E;
			
			
		}	
	}
	

	FILE * pf = fopen(filename,"r");
	if (pf == NULL)
	{
		perror("fopen is failed !");
		return;
	}
	char type;
	long add,size;
	
	while(fscanf(pf," %c %lx,%ld", &type, &add, &size) != EOF){
		if(type == 'I'){
			continue;
		}else if(type == 'M' || type == 'S' || type == 'L'){
			operate(type,add,size,s,E,b,v,sets);
		}
		
	}
	fclose(pf);
	
}



int main(int arg, char* args[])
{
//	printf("%d\n" ,arg);
	int s=0,E=0,b=0,h=0,v=0;
	char *filename = 0;
	int i = 1;
	char *t=0;
	for(;i<arg;i++){	
		t = args[i];
		
		
		if(!strcmp(t,"-s")){
			
			s = s2i(args[++i]);
		}else if(!strcmp(t,"-E")){
			E = s2i(args[++i]);
		}else if(!strcmp(t,"-b")){
			b = s2i(args[++i]);
		}else if(!strcmp(t,"-t")){
			filename = args[++i];
		}else if(!strcmp(t,"-v")){
			v = 1;
		}else if(!strcmp(t,"-h")){
			h = 1;
		}
	}
	sim(s,E,b,h,v,filename);
   printSummary(g_hit, g_miss, g_eviction);
    return 0;
}
