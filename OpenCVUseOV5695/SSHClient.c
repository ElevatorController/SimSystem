#include <sys/shm.h>
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
#include <arpa/inet.h>

#define DEST_PORT 1500//目标地址端口号 
#define DEST_IP "192.168.1.20"/*目标地址IP，这里设为个人电脑操作系统*/ 
#define MAX_DATA 100//接收到的数据最大程度 
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

int main(){

    //信号量相关操作（进程锁）

    int semmutex;
    int key2=ftok(PATHNAME,12);
    printf("创建semmutex\n");
    semmutex=semget(key2,1,IPC_CREAT|0666);//创建二进制信号量，0为占用，1为未占用
    if (semmutex < 0) {
        perror("shmget error (signal)");
        exit(1);
    }
    //初始化信号量
    printf("设置semmutex\n");
    set_semvalue(semmutex,1);
    //设置和打印时间相关的变量
    time_t t;



    int sockfd,new_fd;/*socket句柄和接受到连接后的句柄 */
    struct sockaddr_in dest_addr;/*目标地址信息*/
    char buf[MAX_DATA];//储存接收数据 
    char sendbuf[500000];//存储发送数据
    sockfd=socket(AF_INET,SOCK_STREAM,0);/*建立socket*/
    if(sockfd==-1){
        printf("socket failed:%d\n",errno);
    }


    //参数意义见服务器端 
    dest_addr.sin_family=AF_INET;/*该属性表示接收本机或其他机器传输*/
    dest_addr.sin_port=htons(DEST_PORT);/*端口号*/
    dest_addr.sin_addr.s_addr=inet_addr(DEST_IP);
    bzero(&(dest_addr.sin_zero),8);/*将其他属性置0*/
    
    //握手
    if(connect(sockfd,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr))==-1){//连接方法，传入句柄，目标地址和大小 
        printf("connect failed:%d\n",errno);//失败时可以打印errno 
        close(sockfd);
        return 0;
    } else{
        printf("connect success\n");
        
        recv(sockfd,buf,MAX_DATA,0);//将接收数据打入buf，参数分别是句柄，储存处，最大长度，其他信息（设为0即可）。 
        // send(sockfd,"Here is something messages about this elevator\n",48,0);
        printf("Received:%s\n",buf);
    }

    int times=500;//设置程序最多循环的次数，真实场景下可以不做循环次数的限制
    int i=0;
    while (1)
    {
        semaphore_p(semmutex);//信号量P操作
        int source_json_file=open("send.json",O_RDONLY);
        if(source_json_file<0)
        {
            printf("读文件出错\n");
            exit(0);
        }
        int size=lseek(source_json_file,0,SEEK_END);//获取文件大小
        lseek(source_json_file,0,SEEK_SET);//将指针放在文件开头
        int read_state=read(source_json_file,sendbuf,size);//读文件至指针sendbuf
        printf("size:%d\n",size);
        semaphore_v(semmutex);//信号量V操作
        if(read_state<0)//检查是否读成功
        {
            printf("read_error!\n");
            exit(0);
        }
        send(sockfd,sendbuf,size,0);//发送sendbuf中的内容
        recv(sockfd,buf,MAX_DATA,0);//接收内容并读进buf
        printf("Received:  %s\n",buf);//打印接收到的内容
        sleep(1);//每隔一秒钟发送一次
        i++;
        if(i>times)
        {
            break;
        }
    }
    
    del_semvalue(semmutex);//删除信号量
    close(sockfd);//关闭socket
    printf("已经关闭套接字\n");
    return 0;
} 
