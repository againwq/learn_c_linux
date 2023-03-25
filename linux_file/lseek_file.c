#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
//空洞文件: 对文件进行lseek操作超出文件末尾，末尾会用\0来填充
/**
 * SEEK_SET: 相对于文件开始处进行偏移
 * SEEK_CUR: 相对于当前位置开始偏移
 * SEEK_END: 相对于文件末尾偏移
*/
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
