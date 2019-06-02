#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

using namespace std;

int outputLs(string str_name); // 输出指定文件信息函数

int main(int argc, char* argv[])
{
    string open_dir;
    string str2;

    switch (argc)
    {
    case 1:                 // 缺省文件名 默认当前目录
        open_dir = "./";
        break;

    case 2:                 // 带文件名 第一个参数
        str2 = (string)argv[1];
        outputLs(str2);     
        return 0;      
        break;

    default:
        cout << "Input error!" << endl;
        return 0;
        break;
    }

    DIR *dir = opendir(open_dir.c_str());    // 如果是当前目录
    
    if(dir == 0)
    {
        cout << "No this directory!" << endl;
        return 0;
    }

    for(;;)                                 // 循环遍历当前目录文件
    {
        struct dirent *pentry = readdir(dir);
        if(pentry == 0)
            break;

        if(strcmp(pentry->d_name, ".") == 0)   // 特殊文件. .. 处理
            continue;

        if(strcmp(pentry->d_name, "..") == 0)
            continue;

        string str1 = pentry->d_name;         
        //cout << str1 << endl;     // test
        outputLs(str1);
            
    }

    closedir(dir);

    return 0;
}

int outputLs(string str_name)
{
    struct stat st;
    int ret = stat(str_name.c_str(), &st);

    if(ret == -1)
    {
        cout << "No this file!" << endl;
        return 0;
    }
    
    char perms[11] = {0};   // 存储文件类型和访问权限

    switch(st.st_mode & S_IFMT)   // 判断文件类型
    {
        case S_IFLNK:
            perms[0] = 'l';   // 链接文档link
            break;
        case S_IFDIR:
            perms[0] = 'd';   // 目录
            break;
        case S_IFREG:
            perms[0] = '-';   // 文件
            break;
        case S_IFBLK:
            perms[0] = 'b';   // 可供储存的接口设备
            break;
        case S_IFCHR:
            perms[0] = 'c';   // 串行端口设备
            break;
        case S_IFSOCK:
            perms[0] = 's';   // sock文件
            break;
        case S_IFIFO:
            perms[0] = 'p';   // 命令管道文件
            break;
        default:
            perms[0] = '?';   // 未知
            break;
    }

    perms[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';    // 所有人权限
    perms[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
    
    perms[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';    // 所有组权限
    perms[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
    
    perms[7] = (st.st_mode & S_IROTH) ? 'r' : '-';    // 其他人权限
    perms[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';

    int linkNum = st.st_nlink;                        // 硬链接计数
    
    char* fileUser = getpwuid(st.st_uid)->pw_name;    // 文件所有者
    
    char* fileGrp = getgrgid(st.st_gid)->gr_name;     // 文件所属组
    
    int fileSize = (int)st.st_size;                   // 文件大小
    
    char* time = ctime(&st.st_mtime);                 // 修改时间
    char mtime[512] = {0};
    strncpy(mtime, time, strlen(time)-1);

    cout << perms << " " ;                            // 格式化输出
    cout << linkNum << " ";
    cout << fileUser << " ";
    cout << fileGrp << " ";
    cout << fileSize << " ";
    cout << mtime << " ";
    cout << str_name <<  endl; 

    return 0;
}