#pragma once

class CRAII_DllHandle
{
public:
	CRAII_DllHandle(void)
		: Module_(0)
	{}

	CRAII_DllHandle(const tchar* dllFile)
	{
		Module_ = ::LoadLibrary(dllFile);
		if(!Module_)
		{
			DWORD dwErr = ::GetLastError();
		}
	}

	CRAII_DllHandle& operator=(CRAII_DllHandle& other)
	{
		Destroy();
		Module_ = other.Detach();
		return *this;
	}

	CRAII_DllHandle& operator=(const tchar* lpDllFile)
	{
		return CRAII_DllHandle::operator =(CRAII_DllHandle(lpDllFile));		
	}

	~CRAII_DllHandle(void)
	{
		Destroy();
	}

	HMODULE GetModule() const
	{
		return Module_;
	}

	FARPROC GetProcAddress(LPCSTR lpProcName) const
	{
		_ASSERT(Module_);
		return ::GetProcAddress(Module_, lpProcName);
	}

	HMODULE Detach()
	{
		HMODULE tmp = Module_;
		Module_ = 0;

		return tmp;
	}

	void Destroy()
	{
		if(0 != Module_)
		{
			::FreeLibrary(Module_);
			Module_ = 0;
		}
	}

protected:
	HMODULE Module_;
};

#include <comutil.h>
class CRAII_COMInit
{
public:
	CRAII_COMInit(void)
	{
		hr = CoInitialize(NULL);
	}

	CRAII_COMInit(DWORD flag)
	{
		hr = CoInitializeEx(NULL, flag);
	}

	~CRAII_COMInit(void)
	{
		if(SUCCEEDED(hr))
		{
			CoUninitialize();
		};
	}

protected:
	HRESULT	hr;
};

class CRAII_GlobalMemoryHandle
{
	/*
	* 1. HGLOBAL�ں˽ṹ����Lock����������������Χ�洢��
	* 2. ����MEM_FIXED��ȫ���ڴ棬��lock����ʼ��Ϊ0���Ҿ��ֵ�����ڴ��ַ��GlobalLock����ֵ���ھ����������������GlobalLock��
	* 3. ����MEM_MOVEABLE��ȫ���ڴ棬GlobalLock����lock������1��GlobalUnlock����lock������1��
	* 4. lock��������ʱ��GlobalFree����ʧ�ܣ�����Debugģʽ�»�ϵ㣻
	*/
public:
	CRAII_GlobalMemoryHandle(void)
		: Global_(0)
	{}

	CRAII_GlobalMemoryHandle(HGLOBAL hGlobal)
		: Global_(hGlobal)
	{}

	CRAII_GlobalMemoryHandle(SIZE_T dwBytes, UINT uFlags = 0)
	{
		Global_ = ::GlobalAlloc(uFlags, dwBytes);
		if(NULL == Global_)
		{
			DWORD dwErr = ::GetLastError();
		}
	}

	CRAII_GlobalMemoryHandle& operator=(CRAII_GlobalMemoryHandle& other)
	{
		Destroy();
		Global_ = other.Detach();
		return *this;
	}

	operator       HGLOBAL&()		{ return Global_; }
	operator const HGLOBAL&() const { return Global_; }

	~CRAII_GlobalMemoryHandle(void)
	{
		Destroy();
	}

	HGLOBAL GetGlobal() const
	{
		return Global_;
	}

	LPVOID Lock() const
	{
		_ASSERT(Global_);
		return ::GlobalLock(Global_);
	}

	template<typename T>
	T* Lock() const
	{
		return static<T*>(Lock());
	}

	BOOL Unlock() const
	{
		_ASSERT(Global_);
		return ::GlobalUnlock(Global_);
	}

	int GetLockCount() const
	{
		_ASSERT(Global_);
		UINT uiRet = ::GlobalFlags(Global_);
		return (GMEM_INVALID_HANDLE == uiRet) ? -1 : uiRet & GMEM_LOCKCOUNT;
	}

	HGLOBAL Detach()
	{
		HGLOBAL tmp = Global_;
		Global_ = 0;

		return tmp;
	}

	void Destroy()
	{
		if(NULL != Global_)
		{
			Global_ = ::GlobalFree(Global_);
			if(NULL != Global_)
			{
				_ASSERT(false);
				DWORD dwErr = ::GetLastError();
			}
		}
	}

protected:
	HGLOBAL Global_;
};