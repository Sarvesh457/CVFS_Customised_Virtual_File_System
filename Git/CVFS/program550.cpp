#include<stdio.h>
#include<stdlib.h>

//conditional pre processing
int main()
{
    #ifdef _WIN32           
        system("cls");
    #else 
        system("clear");
    #endif

    return 0;
}