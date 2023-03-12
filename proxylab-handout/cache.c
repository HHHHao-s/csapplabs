
#include "csapp.h"



// int main()
// {

//     HPage ph;
//     cache_init(&ph);

//     for (int i = 0; i < 20; i++)
//     {
//         char *data = Malloc(MAX_OBJECT_SIZE / 100);
//         char title[MAX_TITLE_SIZE];
//         sprintf(title, "%d title", i);
//         sprintf(data, "%d data", i);
//         cache_push(&ph, title, data, MAX_OBJECT_SIZE / 100);
//         // cache_print(&ph);
//     }
//     cache_print(&ph);
//     size_t size;
//     printf("%p\n", cache_pull(&ph, "1 title", &size));
//     printf("%p\n", cache_pull(&ph, "17 title", &size));
//     printf("%p\n", cache_pull(&ph, "5 title", &size));
//     printf("%p\n", cache_pull(&ph, "13 title", &size));
//     printf("%p\n", cache_pull(&ph, "11 title", &size));
//     printf("%p\n", cache_pull(&ph, "12 title", &size));

//     cache_print(&ph);
//     for (int i = 0; i < 5; i++)
//     {
//         char *data = Malloc(MAX_OBJECT_SIZE / 100);
//         char title[MAX_TITLE_SIZE];
//         sprintf(title, "%d title", i);
//         sprintf(data, "%d data", i);
//         cache_push(&ph, title, data, MAX_OBJECT_SIZE / 100);
//         // cache_print(&ph);
//     }
//     cache_print(&ph);
//     return 0;
// }
