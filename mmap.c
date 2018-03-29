//内存映射方式实现文件拷贝
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
    int bd=open("bainiangudu.txt",O_RDWR);
    if(bd==-1)
    {
        perror("open");
        exit(-1);
    }
    size_t len=lseek(bd,0,SEEK_END);
     //MAP_SHARED才会对文件进行修改
    //请确保前面open的时候文件是以可写的方式打开
    void *addr=mmap(0,len,PROT_WRITE,MAP_SHARED,bd,0);
    //检查映射的返回值
    if(addr==MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }
    //映射完就可以关闭文件，不影响已经映射的内存
    close(bd);
    int ret;
    //创建一个文件，并判断这个文件是否存在，存在就打开这个文件，不存在就创建，并且可以将新的内容写入到创建的文件的末尾
    int fd=open("abc.txt",O_RDWR|O_CREAT|O_EXCL|O_APPEND,0777);
    if(fd==-1)
    {
        fd=open("abc.txt",O_RDWR|O_APPEND);
    }
    int retnum=write(fd,addr,len);
    if(retnum==-1)
        {
            perror("write");
            exit(-1);
        }
    
    size_t lenth=lseek(fd,0,SEEK_END);
    printf("长度是%d\n",lenth);
    close(fd);
     //取消映射
     munmap(addr,len);
    return 0;
}