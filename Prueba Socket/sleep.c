#include <unistd.h>
#include <stdio.h>

void wait( int seconds )
{
    sleep(seconds);
}

int main( int argc, char **argv)
{
	printf("Let's wait 3 seconds\n");
    wait(3);
 	printf("It's done\n");

    return 0;
}