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

    key = 9876; //Unique key is used for a perticular shared mem segment

    shmid = shmget(key, SHSIZE, IPC_CREAT | 0666); //shared memory created if doesn't exits
    if (shmid < 0)
    {
        printf("Error in getting shmid");
        return -1;
    }

    shm = shmat(shmid, NULL, 0); //attach current process to shared mem ret is shared memory address

    if (shm == (char *)-1)
    {
        printf("Error in shmat");
        return -1;
    }

    char pass[10];
    printf("Enter Password of length 10\n");
    scanf("%s", pass);
    memcpy(shm, pass, sizeof(pass));

    while (*shm != '*')
        sleep(1);

    int r = *(shm + 12);
    if (r == 0)
        printf("Password is Weak.\n");
    else
        printf("Password is Strong.\n");

    return 0;
}