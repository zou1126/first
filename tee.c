//实现tee命令
//tee命令接受标准输入（stdin）的输入，并将收到的数据同时送往标准输出（stdout）和指定的文件中

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc,const char *argv[])
{
    if(argc<2)
    {
        printf("使用方法: %s 目标文件\n",argv[0]);
        exit(-1);
    }
    printf("目标文件是 %s\n",argv[1]);
    //设置一个缓冲区
    int ret;
    int fd;
    char buffer[108]={0};
    //标准输入
    if(ret=read(0,buffer,sizeof(buffer)-1),ret>0)
    {
        //创建一个文件，并判断这个文件是否存在，存在就打开这个文件，不存在就创建
        fd=open(argv[1],O_CREAT|O_EXCL|O_RDWR|O_APPEND,0777);
        if(fd==-1)
        {
            fd=open(argv[1],O_RDWR|O_APPEND);
        }
        //将标准输入写入到指定文件中
        int retnum=write(fd,buffer,sizeof(buffer)-1);
        if(retnum==-1)
        {
            perror("write");
            exit(-1);
        }
        //标准输出
        printf("%s",buffer);
        printf("读到了%d\n",ret);
    }
    size_t len=lseek(fd,0,SEEK_END);
    printf("文件的长度是%d\n",len);
    close(fd);
    return 0;
}