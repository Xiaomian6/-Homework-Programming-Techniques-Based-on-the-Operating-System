#include "CPluginEnumerator.h"
#include <dirent.h>
#include <string.h>

CPluginEnumerator::CPluginEnumerator()     // 构造函数
{
}

CPluginEnumerator::~CPluginEnumerator()    // 析构函数
{
}

bool CPluginEnumerator::GetPluginNames(vector<string>& vstrPluginNames)
{
    DIR *dir = opendir("./plugin");    // 打开目录 存放插件的文件夹
    if(dir == 0)                       
		return false;
    
    for(;;)     // 循环遍历目录项
    {
		struct dirent *pentry = readdir(dir);   // 读取目录项
		if(pentry == 0)           // 无下一项目录退出
			break;

		if(strcmp(pentry->d_name, ".") == 0)   // 特殊文件. ..处理
			continue;

		if(strcmp(pentry->d_name, "..") == 0)
			continue;
		// 插件路径处理
		string str = "./plugin/";
		str += pentry->d_name;
		vstrPluginNames.push_back(str);   // 放入插件容器
    }

    closedir(dir);    // 关闭目录

    return true;
}

