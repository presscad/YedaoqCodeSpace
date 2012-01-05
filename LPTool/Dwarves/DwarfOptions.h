#pragma once
#include "Singlton.h"
#include <mytype.h>

class CDwarfOptions : public CSingleton<CDwarfOptions>
{
public:
	CDwarfOptions(void);
	~CDwarfOptions(void);

	int	Load();
	int Save();

public:
	tstring		DirCodecFile;		//�������ļ�Ŀ¼

	tstring		DirPackageFile;		//���ļ�Ŀ¼

	tstring		DirMediaFileA;		//ý���ļ�Ŀ¼
	tstring		DirMediaFileB;		//ý���ļ�Ŀ¼
	tstring		DirMediaFileC;		//ý���ļ�Ŀ¼

	tstring		DirDataBase;		//���ݿ�Ŀ¼
};
