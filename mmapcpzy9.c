//内存映射方式实现文件拷贝
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

char *getaddr(const char* pathname,size_t *filelen)
{
    int bd=open(pathname,O_RDONLY);
    if(bd<0)
    {
        perror("open");
        return NULL;
    }
    *filelen=lseek(bd,0,SEEK_END);

     //MAP_SHARED才会对文件进行修改
    //请确保前面open的时候文件是以可写的方式打开
    void *addr=mmap(0,*filelen,PROT_READ,MAP_PRIVATE,bd,0);

    //检查映射的返回值
    if(addr==MAP_FAILED)
    {
        perror("mmap addr");
        return NULL;
    }

    //映射完就可以关闭文件，不影响已经映射的内存
    close(bd);
    return addr;
}

int main(int argc,const char* argv[])
{
    if(argc<3)
    {
        printf("使用方法: %s 源文件 目标文件",argv[0]);
        exit(-1);
    }
    printf("源文件是 %s\n",argv[1]);
    printf("目标文件是 %s\n",argv[2]);

    size_t len;
    char *addr=getaddr(argv[1],&len);
    if(addr==NULL)
    {
        exit(-1);
    }

    //创建一个文件，并判断这个文件是否存在，存在就打开这个文件，不存在就创建，并且可以将新的内容写入到创建的文件的末尾
    int fd=open(argv[2],O_RDWR|O_CREAT|O_EXCL,0777);
    if(fd==-1)
    {
        fd=open(argv[2],O_RDWR);
    }

    //新文件需要先撑大再映射，否则无法写入，产生总线错误
    ftruncate(fd,len);
    void *addrdest=mmap(0,len,PROT_WRITE,MAP_SHARED,fd,0);
    if(addrdest==MAP_FAILED)
    {
        perror("mmap addrdest");
        exit(-1);
    }

    //既然两块内存都准备好了，就可以直接对拷
    memcpy(addrdest,addr,len);

    //输出目标文件的长度，测试有没有拷入
    size_t lenth=lseek(fd,0,SEEK_END);
    printf("长度是%d\n",lenth);
    close(fd);

     //取消映射
     munmap(addr,len);
     munmap(addrdest,lenth);
    return 0;
}