#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHSIZE 100

int main(int argc, char *argv[])
{
    int shmid;
    key_t key;
    char *shm;
    char *s;

    key = 9876;

    shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        printf("Error in getting shmid");
        return -1;
    }

    shm = shmat(shmid, NULL, 0);

    if (shm == (char *)-1)
    {
        printf("Error in shmat");
        return -1;
    }

    char pass[10];
    s = shm;
    for (int i = 0; i < 10; i++)
    {
        pass[i] = *s;
        s++;
    }
    int alphanumeric = 0, specialchar = 0;
    for (int i = 0; i < 10; i++)
    {
        if ((pass[i] >= '0' && pass[i] <= '9') || (pass[i] >= 'a' && pass[i] <= 'z') || (pass[i] >= 'A' && pass[i] <= 'Z'))
        {
            alphanumeric++;
        }
        else
        {
            specialchar++;
        }
    }
    memcpy(shm + 10, &alphanumeric, sizeof(int));
    memcpy(shm + 11, &specialchar, sizeof(int));
    return 0;
}