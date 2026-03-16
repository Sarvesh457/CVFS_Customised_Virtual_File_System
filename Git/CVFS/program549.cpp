#include<stdio.h>
#include<stdlib.h>

//conditional pre processing
int main()
{
    #ifdef _WIN32           
        system("dir");
    #else 
        system("ls");
    #endif

    return 0;
}