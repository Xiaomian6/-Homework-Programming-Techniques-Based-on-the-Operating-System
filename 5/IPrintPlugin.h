#pragma once

class IPrintPlugin  // 基类虚函数
{
public:
	IPrintPlugin();           // 构造函数
	virtual ~IPrintPlugin();  // 析构函数

	virtual void Help() = 0;  // Help
	virtual void Print() = 0; // Print
	virtual int GetID() = 0;  // 功能ID
};
