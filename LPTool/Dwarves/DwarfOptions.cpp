#include "StdAfx.h"
#include "DwarfOptions.h"

CDwarfOptions::CDwarfOptions(void)
{
	DirDataBase = TEXT("D:\\LocalPlayer\\trunk\\database");
	DirCodecFile = TEXT("D:\\LocalPlayer\\trunk\\codec");
	DirPackageFile = TEXT("D:\\LocalPlayer\\trunk\\CodecPackage");
	DirMediaFileA = TEXT("\\\\10.1.10.253\\����ý���\\����ͨ����");
}

CDwarfOptions::~CDwarfOptions(void)
{

}


int	CDwarfOptions::Load()
{
	return 1;
}

int CDwarfOptions::Save()
{
	return 1;
}