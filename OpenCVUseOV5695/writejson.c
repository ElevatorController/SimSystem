


#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <time.h>
#include <fcntl.h>
#include "cJSON.h"

#define PATHNAME "."

/* union semun is defined by including <sys/sem.h> */
/* according to X/OPEN we have to define it ourselves */
union semun {
    int val; /* value for SETVAL */
    struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* array for GETALL, SETALL */
    /* Linux specific part: */
    struct seminfo *__buf; /* buffer for IPC_INFO */
};

static int set_semvalue(int sem_id,int value)
{
    // 用于初始化信号量，在使用信号量前必须这样做
    union semun sem_union;
 
    sem_union.val = value;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
    {
        perror("semctl");
        exit(1);
        return 0;
    }
    return 1;
}
static void del_semvalue(int sem_id)
{
    // 删除信号量
    union semun sem_union;
 
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
    {
        fprintf(stderr, "Failed to delete semaphore\n");
    }
}
static int semaphore_p(int sem_id)
{
    // 对信号量做减1操作，即等待P（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;//P()
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        fprintf(stderr, "semaphore_p failed\n");
        return 0;
    }
 
    return 1;
}
 
static int semaphore_v(int sem_id)
{
    // 这是一个释放操作，它使信号量变为可用，即发送信号V（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; // V()
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        fprintf(stderr, "semaphore_v failed\n");
        return 0;
    }
    return 1;
}


int main(int argc,char* argv[]) {

    int people_num=0;
    sscanf(argv[1],"%d",&people_num);
    //信号量相关操作
        //信号量相关操作（进程锁）

    int semmutex;
    int key2=ftok(PATHNAME,12);
    printf("创建semmutex\n");
    semmutex=semget(key2,1,IPC_CREAT|0666);//创建二进制信号量，0为占用，1为未占用
    if (semmutex < 0) {
        perror("shmget error (signal)");
        exit(1);
    }
    // //初始化信号量
    // printf("设置semmutex\n");
    // set_semvalue(semmutex,1);
    // 读文件
    semaphore_p(semmutex);
    printf("打开源文件\n");
    int source_file=open("send.json",O_RDONLY);
    printf("设置指针\n");
    char message[500000];
    int size=lseek(source_file,0,SEEK_END);
    printf("读文件\n");
    lseek(source_file,0,SEEK_SET);
    read(source_file,message,size);
    printf("%s",message);
    printf("关闭文件\n");
    close(source_file);
    semaphore_v(semmutex);


    // JSON处理
    cJSON* cjson_test = NULL;
    cJSON* cjson_People=NULL;
    cJSON* cjson_Time=NULL;
    int People_array_size = 0;

    cjson_test = cJSON_Parse(message);
    cjson_Time=cJSON_GetObjectItem(cjson_test,"Time");
    cjson_People = cJSON_GetObjectItem(cjson_test, "People");
    People_array_size = cJSON_GetArraySize(cjson_People);
    cjson_Time->valueint=cjson_Time->valueint+1;
    cjson_Time->valuedouble=cjson_Time->valuedouble+1;
    //加入摄像头看到的人
    for(int i=0;i<people_num;i++)
    {
        cJSON* person1 = cJSON_CreateObject();
        cJSON_AddNumberToObject(person1, "come_time", cjson_Time->valueint);
        cJSON_AddNumberToObject(person1, "from_floor", 5);
        cJSON_AddNumberToObject(person1, "to_floor", rand()%12+1);
        cJSON_AddNumberToObject(person1, "current_floor", 5);
        cJSON_AddNumberToObject(person1, "in_which_elevator", 0);
        cJSON_AddFalseToObject(person1, "is_out");
        cJSON_AddItemToArray(cjson_People, person1);
    }
    //加入另外虚拟的人
    int other_random_num=rand()%4;
    for(int i=0;i<other_random_num;i++)
    {
        int floor=rand()%12+1;
        cJSON* person2 = cJSON_CreateObject();
        cJSON_AddNumberToObject(person2, "come_time", cjson_Time->valueint);
        cJSON_AddNumberToObject(person2, "from_floor", floor);
        cJSON_AddNumberToObject(person2, "to_floor", rand()%12+1);
        cJSON_AddNumberToObject(person2, "current_floor", floor);
        cJSON_AddNumberToObject(person2, "in_which_elevator", 0);
        cJSON_AddFalseToObject(person2, "is_out");
        cJSON_AddItemToArray(cjson_People, person2);
    }
    
    

    // 将JSON对象转换为字符串并打印输出
    char* jsonString = cJSON_Print(cjson_test);
    printf("%s\n", jsonString);

    //写文件
    semaphore_p(semmutex);
    printf("打开源文件\n");
    int target_file=open("send.json",O_WRONLY|O_CREAT|O_TRUNC,S_IRWXO);
    printf("读文件\n");
    lseek(target_file,0,SEEK_SET);
    write(target_file,jsonString,strlen(jsonString));
    fsync(target_file);
    printf("关闭文件\n");
    close(target_file);
    semaphore_v(semmutex);

    // 释放内存
    free(jsonString);
    cJSON_Delete(cjson_test);

    return 0;
}