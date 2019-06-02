#include <fcntl.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>

using namespace std;

int main()
{
    int fd = open("test1.txt", O_RDWR | O_APPEND);    // 打开文件,设置O_APPEND
    if(fd == -1)
    {
        cout << "open error" << endl;
        return 0;
    }

    off_t CurrenPosition;
    CurrenPosition = lseek(fd, 0, SEEK_CUR);      // 打开文件后，获取当前文件偏移量
    cout << "After open:" << CurrenPosition << endl;

    if(lseek(fd, 5, SEEK_CUR) == -1)              // 改变当前偏移量，+5
    {
        cout << "lseek error" << endl;
        return 0;
    }

    CurrenPosition = lseek(fd, 0, SEEK_CUR);      // 获取当前偏移量，判断是否可以改变偏移量
    cout << "After change:" << CurrenPosition << endl; 


    CurrenPosition = lseek(fd, 0, SEEK_CUR);  // 获取写入前的偏移量
    cout << "Before write:" << CurrenPosition << endl; 

    char buf[10];
    for(int i=0;i < 10;i++)
    {
        buf[i] = 't';
    }
    if(write(fd, buf, 10) != 10)                // 写入10个数据
    {
        cout << "Write error!" << endl;
        return 0;
    }

    CurrenPosition = lseek(fd, 0, SEEK_CUR);  // 获取写入后的偏移量
    cout << "After write:" << CurrenPosition << endl; 

    if(lseek(fd, 5, SEEK_SET) == -1)         // 改变当前偏移量为5
    {
        cout << "lseek error" << endl;
        return 0;
    }

    CurrenPosition = lseek(fd, 0, SEEK_CUR);  // 获取读取前偏移量
    cout << "Before read:" << CurrenPosition << endl; 

    if(read(fd, buf, 3) == -1)                // 读取3个字符
    {
        cout << "Read error!" << endl;
        return 0;
    }
    for(int i=0;i<3;i++)                      // 输出3个字符
    {
        cout << buf[i] ;
    }
    cout << endl;

    CurrenPosition = lseek(fd, 0, SEEK_CUR);  // 获取读取后偏移量
    cout << "After read:" << CurrenPosition << endl; 

    if(close(fd) == -1)                       // 关闭文件
    {
        cout << "close error" << endl;
    }

    return 0;
}