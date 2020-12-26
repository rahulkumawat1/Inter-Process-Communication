#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <wait.h>

float avg_fn(float *arr)
{
    float sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += arr[i];
    }
    sum /= 5;
    return sum;
}

float std_deviationdev_fn(float *arr, float mean)
{
    float t;
    float sum = 0;
    for (int i = 0; i < 5; i++)
    {
        t = arr[i] - mean;

        sum += (t * t);
    }
    sum /= 5;
    sum = sqrt(sum);
    return sum;
}

int main()
{
    int p1[2];
    int p2[2];
    int p3[2];

    pipe(p1);
    pipe(p2);
    pipe(p3);

    //p1[0]-read
    //p1[1]-write

    int pid = fork();
    if (pid == 0)
    {
        //Child process 1

        close(p1[1]);
        close(p2[0]);
        close(p3[1]);
        close(p3[0]);

        float temp[5];
        float avg;
        float std_deviation;

        read(p1[0], &temp, sizeof(temp));

        avg = avg_fn(temp);
        std_deviation = std_deviationdev_fn(temp, avg);

        write(p2[1], &avg, sizeof(avg));
        write(p2[1], &std_deviation, sizeof(std_deviation));

        close(p1[0]);
        close(p2[1]);
    }
    else
    {
        int pid2 = fork();
        if (pid2 == 0)
        {
            //Child process 2

            close(p2[1]);
            close(p1[1]);
            close(p3[0]);

            float temp[5];
            float avg;
            float std_deviation;
            int cat[5];

            read(p1[0], &temp, sizeof(temp));

            read(p2[0], &avg, sizeof(avg));
            read(p2[0], &std_deviation, sizeof(std_deviation));

            for (int i = 0; i < 5; i++)
            {
                if (temp[i] == avg)
                {
                    cat[i] = 0;
                }
                else if (temp[i] > (avg + std_deviation))
                {
                    cat[i] = 1;
                }
                else if (temp[i] > avg && temp[i] < (avg + std_deviation))
                {
                    cat[i] = 2;
                }
                else if (temp[i] < avg && temp[i] > (avg - std_deviation))
                {
                    cat[i] = 3;
                }
                else if (temp[i] < (avg - std_deviation))
                {
                    cat[i] = 4;
                }
            }

            write(p3[1], &cat, sizeof(cat));

            close(p2[0]);
            close(p3[1]);
            close(p1[0]);
        }
        else
        {
            //Parent process

            close(p2[0]);
            close(p2[1]);
            close(p1[0]);
            close(p3[1]);

            float temp[5];
            int cat[5];

            printf("Enter the temperatures in the range of 15 deg Celsius to 45 deg Celsius\n");
            for (int i = 0; i < 5; i++)
            {
                printf("Temperature at location L%d: ", i + 1);
                scanf("%f", &temp[i]);
            }
            write(p1[1], &temp, sizeof(temp));
            write(p1[1], &temp, sizeof(temp));

            read(p3[0], &cat, sizeof(cat));

            printf("\nRevised temperatures:\n");
            for (int i = 0; i < 5; i++)
            {
                if (cat[i] == 1)
                {
                    temp[i] -= 3;
                }
                else if (cat[i] == 2)
                {
                    temp[i] -= 1.5;
                }
                else if (cat[i] == 3)
                {
                    temp[i] += 2;
                }
                else if (cat[i] == 4)
                {
                    temp[i] += 2.5;
                }
                printf("L%d: %.2f deg Celsius\n", i + 1, temp[i]);
            }

            close(p3[0]);
            close(p1[1]);
        }
    }
    return 0;
}
