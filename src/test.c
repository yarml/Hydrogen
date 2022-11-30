#include <stdlib.h>
#include <stdio.h>

#include <dtc/str.h>

int main()
{
    status s;

    dtc_str *str;

    s = dtc_str_init("Hello, World!\n", &str);

    if(s < 0)
    {
        printf("Error initializing string. Error code %d\n", s);
        exit(1);
    }

    char *ss;

    s = dtc_str_buf(str, &ss);

    if(s < 0)
    {
        printf("Error getting string buffer. Error code: %d\n", s);
        exit(1);
    }

    puts(ss);

}
