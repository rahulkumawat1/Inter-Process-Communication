#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

double variance(float a[], int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
    double mean = (double)sum / (double)n;

    double sqDiff = 0;
    for (int i = 0; i < n; i++)
        sqDiff += (a[i] - mean) * (a[i] - mean);
    return sqDiff / n;
}

double standardDeviation(float arr[], int n)
{
    return sqrt(variance(arr, n));
}

int main(int argc, char *argv[])
{
    float L[6];
    int fd = open("temp", O_RDONLY);
    read(fd, L, sizeof(float) * 6);
    close(fd);

    float avg, std_deviation;
    avg = (L[1] + L[2] + L[3] + L[4] + L[5]) / 5.0;
    std_deviation = standardDeviation(L + 1, 5);

    mkfifo("avg_std", 0777);
    fd = open("avg_std", O_WRONLY);
    write(fd, &avg, sizeof(float));
    write(fd, &std_deviation, sizeof(float));
    close(fd);

    fd = open("temp", O_WRONLY);
    write(fd, L, sizeof(float) * 6);
    close(fd);
}