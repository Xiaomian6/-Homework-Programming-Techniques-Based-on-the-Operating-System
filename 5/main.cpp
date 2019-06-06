#include <iostream>
#include "CPluginController.h"
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv)
{
	switch (argc)
	{
	case 2:
		if(strcmp(argv[1], "help") == 0)    // help参数处理
		{
			CPluginController pc;  // 实例化一个插件控制器
			pc.ProcessHelp();      // 调用Help命令处理进程
			return 0;              // 结束
		}else
		{
			int FunctionID = atoi(argv[1]); // 功能ID参数
			CPluginController pc;           // 实例化一个插件控制器
			pc.InitializeController();      // 初始化插件句柄
			pc.ProcessRequest(FunctionID);  // 调用功能ID输出命令处理进程
			pc.UninitializeController();    // 卸载插件
		}
		break;
	default:
		cout << "输入参数有误!" << endl;
		return 0;
		break;
	}

	return 0;
}