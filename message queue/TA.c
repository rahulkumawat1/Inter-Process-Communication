#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct MSG
{
    long type;
    char data[100];
} message;

char getGrade(float mark, float avg)
{
    if (mark <= 19)
        return 'C';
    else if (mark < 29)
        return 'B';
    else if (mark < 39)
        return 'A';
    else
        return 'S';
}

void main()
{
    int i = 0;
    key_t key;
    int msgid;

    float marks[5];
    float avg = 0;

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    for (i = 1; i <= 5; i += 1)
    {
        msgrcv(msgid, &message, sizeof(message.data), 10, 0); //MARKS recieved
        printf("Mark of Student %d is : %s \n", i, message.data);
        sscanf(message.data, "%f", marks + i - 1);
        avg += marks[i - 1];
    }
    avg /= 5;

    message.type = 100;
    sprintf(message.data, "%f", avg);
    msgsnd(msgid, &message, sizeof(message.data), 0); //avg sent
    printf("Avg Sent to CI\n");

    for (int i = 1; i <= 5; i += 1)
    {
        message.type = 100;
        sprintf(message.data, "%c", getGrade(marks[i - 1], avg));
        msgsnd(msgid, &message, sizeof(message.data), 0); //marks sent to CI
    }
    printf("Grades Sent to CI\n");

    for (int i = 1; i <= 5; i += 1)
    {
        message.type = i;
        sprintf(message.data, "%f", marks[i - 1]);
        msgsnd(msgid, &message, sizeof(message.data), 0); //marks sent to ST
    }
    printf("Marks sent to ST");
    //sleep(1);
    //msgctl(msgid, IPC_RMID, NULL);
}