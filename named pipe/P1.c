#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[])
{
    float L[6];
    for (int i = 1; i < 6; i++)
    {
        printf("Temerature at Location L%d: ", i);
        scanf("%f", &L[i]);
        if (L[i] >= 15 && L[i] <= 45)
            ;
        else
        {
            printf("Enter bw 15 & 45\n");
            return -1;
        }
    }
    mkfifo("temp", 0777);
    int fd = open("temp", O_WRONLY);
    write(fd, L, sizeof(float) * 6);
    close(fd);

    mkfifo("category", 0777);
    fd = open("category", O_RDONLY);
    int cat[6];
    read(fd, cat + 1, sizeof(int) * 5);
    int tm;
    float c;
    for (int i = 1; i < 6; i++)
    {
        read(fd, &tm, sizeof(int));
        if (tm == 0)
            c = 0;
        else if (tm == 1)
            c = -3;
        else if (tm == 2)
            c = -1.5;
        else if (tm == 3)
            c = 2;
        else if (tm == 4)
            c = 2.5;

        printf("Temperature %.2f at L%d\n", L[i] + c, i);
    }
    close(fd);
}