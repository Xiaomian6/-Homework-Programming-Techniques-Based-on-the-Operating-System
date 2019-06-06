#include <iostream>
#include "CPluginController.h"
#include "CPluginEnumerator.h"
#include "IPrintPlugin.h"
#include "dlfcn.h"
using namespace std;

CPluginController::CPluginController(void)   // 构造函数
{
}

CPluginController::~CPluginController(void)  // 析构函数
{
}

bool CPluginController::InitializeController(void)   // 初始化插件句柄
{
	vector<string> vstrPluginNames;      // 插件容器

	CPluginEnumerator enumerator;        // 实例化插件枚举器
	if(!enumerator.GetPluginNames(vstrPluginNames))  // 注册目录里的插件
	{
		cout << "GetPluginNames error" << endl;
		return false;
	}

	for(unsigned int i=0 ; i<vstrPluginNames.size(); i++)
	{

		typedef int (*PLUGIN_CREATE)(IPrintPlugin**);
		PLUGIN_CREATE CreateProc; 

		IPrintPlugin *pPlugin = NULL;
		// 打开动态链接库
    	// RTLD_LAZY 动态加入动态链接库中的函数
		void* handle = dlopen(vstrPluginNames[i].c_str(), RTLD_LAZY); 

		if(handle != NULL) 
		{ 
			v_PluginHandle.push_back(handle);  // 放入动态链接库句柄容器
			// 映射动态链接库类中的 CreateObj()创建对象函数
			CreateProc = (PLUGIN_CREATE)dlsym(handle, "CreateObj"); 

			if(NULL != CreateProc) 
			{
				(CreateProc)(&pPlugin);  // CreateObj()创建对象函数

				if(pPlugin != NULL)
				{
					m_vpPlugin.push_back(pPlugin);  // 放入插件类容器
				}
			}
		}
	}

	return true;
}

bool CPluginController::ProcessRequest(int FunctionID)   // 功能ID输出命令处理进程
{
	for(unsigned int i = 0; i < m_vpPlugin.size(); i++)
	{
		if(m_vpPlugin[i]->GetID() == FunctionID)   // 在功能ID集合中比对
		{
			m_vpPlugin[i]->Print();    // 调用类的Print()函数，输出
			break;
		}
	}

	return true;
}

bool CPluginController::ProcessHelp(void)   // Help命令处理进程
{
	vector<string> vstrPluginNames;

	CPluginEnumerator enumerator;
	if(!enumerator.GetPluginNames(vstrPluginNames))
		return false;

	for(unsigned int i=0 ; i<vstrPluginNames.size(); i++)
	{
		typedef int (*PLUGIN_CREATE)(IPrintPlugin**);
		PLUGIN_CREATE CreateProc; 

		IPrintPlugin *pPlugin = NULL;
		// 打开动态链接库
    	// RTLD_LAZY 动态加入动态链接库中的函数
		void* handle = dlopen(vstrPluginNames[i].c_str(), RTLD_LAZY); 

		if(handle != NULL) 
		{ 
			// 映射动态链接库类中的 CreateObj()创建对象函数
			CreateProc = (PLUGIN_CREATE)dlsym(handle, "CreateObj"); 

			if(NULL != CreateProc) 
			{
				(CreateProc)(&pPlugin);

				if(pPlugin != NULL)
				{
					pPlugin->Help();  // 调用类Help()函数，输出
				}
			}

			dlclose(handle);    // 卸载句柄
		}
	}

	return true;
}

bool CPluginController::UninitializeController()     // 卸载插件
{
	for(unsigned int i = 0; i < v_PluginHandle.size(); i++)   // 遍历卸载插件句柄
	{
		dlclose(v_PluginHandle[i]);
	}

	return true;
}
