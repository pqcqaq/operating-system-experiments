#include <windows.h>
#include <stdio.h>

int main()
{
    printf("Child process begins\n");
    Sleep(5000);  // Sleep for 5 seconds
    printf("Child process ends\n");
    return 0;
}
