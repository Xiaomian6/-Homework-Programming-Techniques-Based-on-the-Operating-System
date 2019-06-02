#include <fcntl.h>
#include <iostream>
#include <dirent.h>
#include <string.h>

using namespace std;

void recursiveRead(string str_name,int level);  // 递归读取函数

int main(int argc,char* argv[])
{
    string open_dir;
    switch (argc)
    {
    case 1:                // 缺省目录参数，默认 ./ 目录
        open_dir = ".";
        break;
    case 2:                // 带参数：目录的路径
        open_dir = (string)argv[1];
        break;
    default:               // 参数格式输入不正确，错误处理
        cout << "Input error!" << endl;
        return 0;
        break;
    }

    DIR *dir = opendir(open_dir.c_str());                // 打开目录
    if(dir == 0)       
    {
        cout << "No this directory!" << endl;
        return 0;
    }

    cout << "|" << endl;
    for(;;)                                            // 循环读取目录
    {
        struct dirent *pentry = readdir(dir);
        if(pentry == 0)                                // 空指针退出循环
            break;

        if(strcmp(pentry->d_name,".") == 0)            // . .. 特殊文件处理
            continue;

        if(strcmp(pentry->d_name,"..") == 0)
            continue;

        if(pentry->d_type == 4)                        // 如果是目录类型
        {
            string str = open_dir;
            str += "/";
            str += pentry->d_name;
            cout << "|-" ;
            cout << pentry->d_name << "(目录)" << endl;   
            recursiveRead(str, 1);                     // 递归读取该目录下级目录
        }

        if(pentry->d_type != 4)                        // 不是目录类型，输出文件名
        {
            cout << "|-" ;
            cout << pentry->d_name << endl;
        }

    }

    closedir(dir);                                     // 关闭

    return 0;
}

void recursiveRead(string str_name,int level)
{
    DIR *dir = opendir(str_name.c_str());                   // 打开目录
    if(dir == 0)       
    {
        cout << "RecursiveRead error!" << endl;
    }

    cout << "|";
    for(int i=0;i<level;i++)
    {
        cout << "  ";
    }
    cout << "|" << endl;
    
    for(;;)                                                 // 循环读取目录
    {
        struct dirent *pentry = readdir(dir);
        if(pentry == 0)                                     // 空指针退出循环
            break;

        if(strcmp(pentry->d_name,".") == 0)                 // . .. 特殊文件处理
            continue;

        if(strcmp(pentry->d_name,"..") == 0)
            continue;

        cout << "|";
        for(int i=0;i<level;i++)
        {
            cout << "  ";
        }

        if(pentry->d_type == 4)                            // 如果是目录类型
        {
            string str = str_name;
            str += "/";
            str += pentry->d_name;
            cout << "|-" ;
            cout << pentry->d_name << "(目录)" << endl;   
            recursiveRead(str, level+1);                     // 递归读取该目录下级目录
        }

        if(pentry->d_type != 4)                        // 不是目录类型，输出文件名
        {
            cout << "|-" ;
            cout << pentry->d_name << endl;
        }

    }

    closedir(dir);                                      // 关闭
}

