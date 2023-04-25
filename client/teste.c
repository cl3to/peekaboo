#include <stdio.h>

int main(int argc, char **argv)
{
    int a = 10, b = 20, c = 30;
    char *format = "a = %d, b = %d\n";
    printf(format, a, b, c);
    return 0;
}