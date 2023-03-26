# Linux环境下c的文件操作

## 一、文件的打开、关闭、读取和写入

</br>

### open

- 函数原型: `int open(const char *pathname, int flags, mode_t mode)`
- 返回值：文件标识符( > 2 )打开失败会返回 -1
- flags(文件打开后允许的操作方式)参数介绍:
    1. O_RDONLY: 只读
    2. O_WRONLY: 只写
    3. O_RDWR: 读写
    4. O_APPEND: 追加内容至文件末尾
    5. O_CREAT: 如果文件不存在就创建文件,此时需要mode参数来指定用户的权限,一般为mode设为*S_IRWXU*
    6. O_EXCL: 查看文件是否存在,如果存在则返回-1
    7. O_TRUNC: 将文件长度截断为0,可以理解为将文件清空
    8. flags可以组合使用,比如 *O_EXCL | O_CREAT* 就可以做到安全的打开一个文件。检测文件存在就返回,不存在就创建

- 示例:

    ``` C
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <stdio.h>
    /*
    * 文件描述符：0：stdin, 1:stdout, 2:stdout (标准输入,标准输出,标准错误)
    */
    int main(int argc, char const *argv[])
    {
        int fd = -1;
        char filename1[] = "test.txt";
        char filename2[] = "test2.txt";

        fd = open(filename1, O_RDWR);//出错的话文件描述符返回-1，成功的话返回 > 2的整数
        if(fd == -1){
            printf("打开文件错误,fd:%d\n", fd);
        }else{
            printf("打开文件成功, fd:%d\n", fd);
        }
        fd = open(filename2, O_RDWR | O_CREAT | O_EXCL, S_IRWXU); //安全的打开一个文件
        if(fd == -1){
            printf("该文件已经存在,fd:%d\n", fd);
            fd = open(filename2, O_RDWR);
            printf("fd:%d\n", fd);
        }else{
            printf("打开文件成功, fd:%d\n", fd);
        }
        close(fd); //关闭文件，让这个文件描述符能再次被占用

        return 0;
    }
    ```

</br>

### close

- 函数原型: `int close(int fd);`
- 参数: fd:文件描述符(一般是open函数的返回值);
- 作用: 关闭一个文件,并释放掉文件描述符,让它能够被再次占用
- 下面是一个检测你的系统支持的最大文件描述符是多少的示例

    ``` C
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <stdio.h>

    int main(void){
        int i = 0;
        int fd = 0;
        for(i = 0; fd >= 0; i++){
            fd = open("test.txt", O_CREAT);
        if(fd > 0){
            printf("fd: %d\n", fd);
        }else{
            printf("error,can't open file\n");
            return -1;
          }
        }
        return 0;
    }
    ```

</br>

### read

- 函数原型: `ssize_t read(int __fd, void *__buf, size_t __nbytes)`
- 返回值: *ssize_t*类型,若为 **-1**,意味着读取失败。若读取成功,则可以从*__buf*中取数据
- 参数:
    1. __fd: 要读取文件的文件标识符
    2. void *__buf: 读取到指定的缓冲区
    3. __nbytes: 读取的字节数,一般为指定缓冲区的大小
- 示例:

    ``` C
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <stdio.h>
    #define COUNT 10

    int main(int argc, char const *argv[])
    {
        int fd = -1;
        char buffer[COUNT];
        ssize_t size = -1;
        char filename[] = "test.txt";

        fd = open(filename, O_RDWR | O_CREAT | O_EXCL, S_IRWXU);

        if( fd == -1){
            printf("%s文件打开错误\n", filename);
            fd = open(filename, O_RDWR);
        }else{
            printf("%s文件打开成功%d\n", filename, fd);
        }

        while (size)
        {
            size = read(fd, buffer, COUNT);  //参数：文件标识符，缓冲区，字节数
            if(size == -1){
                close(fd);
                printf("文件读取错误\n");
                return -1;
            }else{
                if(size > 0){
                    printf("共读取字节%ld\n", size);
                    for( int i = 0; i < size; i++)
                        printf("%c", buffer[i]);
                    printf("\n");
                }else{
                    printf("文件读取完毕\n");
                }
            }
        }
    
        return 0;
    }
    ```

</br>

### write

- 函数原型: `ssize_t write (int __fd, const void *__buf, size_t __n)`
- 返回值: 返回成功写入的数量
- 参数:
    1. __fd: 要写入的文件的文件标识符
    2. __buf: 要写入的数据
    3. __n: 要写入的数量
- 示例:

    ``` C
    #include <stdio.h>
    #include <sys/stat.h>
    #include<sys/types.h>
    #include<fcntl.h>
    #include<unistd.h>
    #include<string.h>

    int main(int argc, char const *argv[])
    {
        int fd = -1;
        ssize_t size = -1;
        char buffer[] = "这是追加的数据";
        fd = open("test.txt", O_RDWR | O_APPEND);  //
        if(fd == -1){
            printf("文件打开错误, fd:%d\n",fd);
            return -1;
        }else{
            size = write(fd, buffer, strlen(buffer));
            printf("写入了%ld字节", size);
            close(fd);
        }
        return 0;
    }
    ```

**注意**: 对文件执行任意操作之前,需要先确保拥有对应的*权限*

</br>

## 二、lseek(指针偏移)

- 概念解释: lseek函数指定文件的偏移量,即从文件的那个位置开始操作
- 函数原型: `off_t lseek (int __fd, __off_t __offset, int __whence)`
- 返回值: off_t(long)类型, 执行成功返回文件当前的偏移量, 执行失败返回-1
- 参数:
    1. __fd: 指定文件的文件描述符
    2. __offset: 偏移量
    3. __whence:
        1. SEEK_SET: 代表offset参数为相对于文件开始的偏移量
        2. SEEK_CUR: 代表offset参数为相对于当前位置的偏移量
        3. SEEK_END: 代表offset参数为相当于文件结尾的,即将文件的长度扩展为length + offset, 空缺的用 \0 填充
- 示例:

    ``` C
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <stdio.h>
    //空洞文件: 对文件进行lseek操作超出文件末尾，末尾会用\0来填充
    int main(int argc, char const *argv[])
    {
        off_t offset = -1;
        offset = lseek(1, 0, SEEK_CUR); //判断标准输入(stdin)能否进行seek操作
        if(offset == -1){
            printf("STDIN can't seek\n");
            return -1;
        }else{
            printf("STDIN can seek\n");
        }
        return 0;
    }
    ```

</br>

## 三、stat(文件状态)

- 函数原型:`int stat (const char *path, struct stat *buf)`
- 返回值: 若为-1,则获取文件状态失败
- 参数:
    1. path: 文件路径
    2. buf: struct stat类型, 用来存储文件的状态
- 示例:

    ``` C
    #include <sys/types.h>
    #include<sys/stat.h>
    #include<unistd.h>
    #include<stdio.h>

    int main(int argc, char const *argv[])
    {
        struct stat st;  // 定义一个保存文件状态的结构体
        if( stat("test.txt", &st) == -1){
            printf("获取文件状态出错");
            return -1;
        }
        printf("设备ID: %ld\n", st.st_dev);
        printf("文件节点: %ld\n", st.st_ino);
        printf("文件模式: %d\n", st.st_mode);
        printf("文件硬链接数目: %ld\n", st.st_nlink);
        printf("文件所有者ID: %d\n", st.st_uid);
        printf("文件所有者的组ID: %d\n", st.st_gid);
        printf("设备ID(特殊设备): %ld\n", st.st_rdev);
        printf("文件大小: %ld\n", st.st_size);
        printf("文件所在文件系统块大小: %ld\n", st.st_blksize);
        printf("文件占用块数量: %ld\n", st.st_blocks);
        printf("文件最后访问时间: %ld\n", st.st_atime);
        printf("文件最后修改时间: %ld\n", st.st_mtime);
        printf("文件最后状态改变时间: %ld\n", st.st_ctime);
        return 0;
    }
    ```

</br>

## 四、mmap 和 munmap (内存映射)

</br>

### mmap

- 概念解释: 将一个文件的指定的内容映射到内存中,占用内存中的地址块,但是</br>
            不会占用内存空间。映射的内容可以用指针访问内存的方式去直接访问。速度更快。
- 函数原型: `void *mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset)`
- 返回值: 一个 *void*类型的指针,指向映射到内存空间的起始地址, 若等于(void*)-1,则意味着映射失败
- 参数:
    1. start: 指定映射到的内存的起始位置,通常为*NULL*,交给系统自行处理
    2. length: 映射到内存的数据的长度
    3. prot: 映射区的保护方式,也可以组合使用
        1. PROT_EXEC: 映射区域可执行
        2. PROT_READ: 映射区域可读
        3. PROT_WRITE: 映射区可写
        4. PROT_NONE: 映射区不能存取
    4. flags: 指定映射对象的类型(常用的两个)
        1. MAP_SHARED: 共享的映射去,可以被其他进程共享
        2. MAP_PRIVATE: 对映射区的操作不会影响原文件(相当于一份copy)
    5. fd: 文件的文件描述符
    6. offset: 指定文件的偏移量
- 示例:

    ``` C
    #include<stdio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <string.h>
    #define FILE_SIZE 80
    int main(int argc, char const *argv[])
    {
        int fd = -1;
        const char filename[] = "mmap_file.txt";
        char *ptr = NULL;
        char content[] = "this is what I write in memory";

        fd = open(filename, O_RDWR | O_EXCL | O_CREAT, S_IRWXU);
        if( fd == -1 ){
            printf("文件已经存在\n");
            fd = open(filename, O_RDWR);
        }

        lseek(fd, FILE_SIZE - 1, SEEK_SET);  //把文件大小扩展到 79 B
        write(fd, "a", 1);  //写入一个字符，扩展到80

        ptr = (char*)mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //进行映射

        if(ptr == (char*)-1){
            printf("映射失败\n");
            close(fd);
            return -1;
        }
        memcpy(ptr + 16, content, strlen(content));   //从起始位置偏移16写入数据
        munmap(ptr, FILE_SIZE);  //关闭映射
        close(fd);

        return 0;
    }
    ```

</br>

### munmap

- 功能: 清除内存映射
- 函数原型: `int munmap (void *__addr, size_t __len)`
- 参数:
    1. __addr: mmap函数的返回值
    2. __len: 要清除映射的长度

</br>

## 五、fcntl 和 ioctl

</br>

### fcntl

- 函数原型:
- 返回值:
- 参数:
- 示例:

</br>

### ioctl

- 函数原型:
- 返回值:
- 参数:
- 示例:

</br>

## Linux下文件操作总结

&emsp;Linux系统下进行文件操作需要文件标识符,文件标识符是系统连接用户空间和内核空间的枢纽 </br>
      当用户打开一个文件是,内核空间会创建相应的结构,并返回一个整型变量,即文件描述符。通常文 </br>
      件描述符只在同一个进程中有效,即不同进程可拥有相同的文件描述符,但是指向的文件不一定相</br>
      同。默认情况下文件描述符0,1,2分别被stdin,stdout,stderr占用。
