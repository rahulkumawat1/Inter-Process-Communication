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
    int fd = open("avg_std", O_RDONLY);
    float avg, std_deviation;
    read(fd, &avg, sizeof(float));
    read(fd, &std_deviation, sizeof(float));
    printf("avg: %.2f", avg);
    printf("std: %.2f", std_deviation);
    close(fd);

    float L[6];
    fd = open("temp", O_RDONLY);
    read(fd, L, sizeof(float) * 6);
    close(fd);

    fd = open("category", O_WRONLY);
    int cat[5];
    for (int i = 0; i < 5; i++)
        cat[i] = i;
    for (int i = 1; i < 6; i++)
    {
        if (L[i] == avg)
            write(fd, &cat[0], sizeof(int));
        if (L[i] > avg + std_deviation)
            write(fd, &cat[1], sizeof(int));
        if (L[i] > avg && L[i] < avg + std_deviation)
            write(fd, &cat[2], sizeof(int));
        if (L[i] < avg && L[i] > avg - std_deviation)
            write(fd, &cat[3], sizeof(int));
        if (L[i] < avg - std_deviation)
            write(fd, &cat[4], sizeof(int));
    }
    close(fd);
}