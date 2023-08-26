#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<fcntl.h>

#define PORT 1500//端口号 
#define BACKLOG 5/*最大监听数*/ 
#define MAX_DATA 10000//接收到的数据最大程度 

int main(){
    printf("启动程序\n");
    int sockfd,new_fd;/*socket句柄和建立连接后的句柄*/
    struct sockaddr_in my_addr;/*本方地址信息结构体，下面有具体的属性赋值*/
    struct sockaddr_in their_addr;/*对方地址信息*/
    int sin_size;
    char buf[MAX_DATA];//储存接收数据 
    printf("建立套接字\n");
    sockfd=socket(AF_INET,SOCK_STREAM,0);//建立socket 
    if(sockfd==-1){
        printf("socket failed:%d",errno);
        return -1;
    }
    printf("设置套接字\n");
    my_addr.sin_family=AF_INET;/*该属性表示接收本机或其他机器传输*/
    my_addr.sin_port=htons(PORT);/*端口号*/
    my_addr.sin_addr.s_addr=htonl(INADDR_ANY);/*IP*/
    bzero(&(my_addr.sin_zero),8);/*将其他属性置0*/
    if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))<0){//绑定地址结构体和socket
        printf("bind error");
        return -1;
    }
    printf("开始监听\n");
    listen(sockfd,BACKLOG);//开启监听 ，第二个参数是最大监听数 
    while(1){//握手
        sin_size=sizeof(struct sockaddr_in);

        new_fd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小 
        if(new_fd==-1){
            printf("receive failed\n");
        }
        else{
            printf("receive success\n");
            send(new_fd,"Hello World! You can send normal messages\n",43,0);//发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可） 
            break;
        }
    }
    while (1)//接收消息并写文件
    {   
        int size=recv(new_fd,buf,MAX_DATA,0);//将接收到的数据打入buf
        // printf("Received:%s. I have got all of the states.\n",buf);

        if(size>0)
        {
            printf("size:%d\n",size);
            int receive_json_file=open("./test_json/in1.json",O_WRONLY|O_CREAT|O_TRUNC,S_IRWXO);//打开文件，只写，若无则创建
            lseek(receive_json_file,0,SEEK_SET);//将指针设置到文件开头
            int write_state=write(receive_json_file,buf,size);//将收到的内容写入到文件中
            if(write_state<0)//写状态检查
            {
                printf("write error!\n");
                exit(0);
            }
            system("python3 algorithm_wrapper.py");//运行一次电梯调度系统
            send(new_fd,"This is the response.\n",23,0);//发回内容，表示已经成功接收到
            // close(sockfd);//关闭socket
        }
        
        sleep(1);
    }
    close(sockfd);//关闭socket
    return 0;
} 