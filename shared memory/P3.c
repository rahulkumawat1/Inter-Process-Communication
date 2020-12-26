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

    int alphanumeric = *(shm + 10);
    int specialchar = *(shm + 11);

    int r;
    if (alphanumeric >= specialchar)
        r = 0;
    else
        r = 1;
    memcpy(shm + 12, &r, sizeof(int));
    *shm = '*';
    return 0;
}