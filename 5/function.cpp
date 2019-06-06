#include <iostream>
#include "IPrintPlugin.h"

using namespace std;

class CPrintPlugin : public IPrintPlugin    // 继承基类
{
	public:
		CPrintPlugin()          
		{
			FUNC_ID = 1;
		}

		virtual ~CPrintPlugin()
		{
		}

		virtual void Print()
		{
			cout << "Hello World!" << endl;
		}

		virtual void Help()
		{
			cout << "Function ID " << FUNC_ID << " : This function will print hello world." << endl;
		}

		virtual int GetID(void)
		{
			return FUNC_ID;
		}
	private:
		int FUNC_ID;    // 功能ID
};

extern "C" void CreateObj(IPrintPlugin **ppPlugin)  // 创建对象
{
	static CPrintPlugin plugin;
	*ppPlugin = &plugin;
}
