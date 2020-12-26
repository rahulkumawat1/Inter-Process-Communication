#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct MSG
{
    long type;
    char data[100];
} message;

void main()
{

    key_t key;
    int msgid;

    key = ftok("progfile", 65);            //unique key is return
    msgid = msgget(key, 0666 | IPC_CREAT); //message queue is created and id is returned

    int i;

    float avg;
    float marks[5];

    printf("MARKS of 5 students out of 50:\n");
    for (int i = 0; i < 5; i++)
    {
        scanf("%f", &marks[i]);
    }

    for (int i = 1; i <= 5; i += 1)
    {
        message.type = 10;
        sprintf(message.data, "%f", marks[i - 1]); //instead of printing terminal output str is stored in message.data

        msgsnd(msgid, &message, sizeof(message.data), 0); //sending marks
    }

    msgrcv(msgid, &message, sizeof(message.data), 100, 0); //recieving avg
    printf("Avg of marks is : %s \n", message.data);

    sscanf(message.data, "%f", &avg);

    for (i = 1; i <= 5; i += 1)
    {
        msgrcv(msgid, &message, sizeof(message.data), 100, 0); //recieving grades
        printf("Grade of Student %d is : %s \n", i, message.data);
    }
}