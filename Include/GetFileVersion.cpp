#include "stdafx.h"
#include "GetFileVersion.h"

/////////////////////////////////////////////////////////////////////////////


HRESULT STDAPICALLTYPE DllGetVersion( IN  HMODULE hModule, 
									  OUT DLLVERSIONINFO* lpDVI )
{
	if( hModule == NULL || 
		::IsBadReadPtr( lpDVI, sizeof( DLLVERSIONINFO* ) ) )
	{
		ASSERT_RETURN( S_FALSE );
	}

	CONST DWORD cbSize = lpDVI->cbSize;

	if(
#ifdef DLLVERSIONINFO2
		(
#endif
		cbSize != sizeof( DLLVERSIONINFO  )
#ifdef DLLVERSIONINFO2
		&& cbSize != sizeof( DLLVERSIONINFO2 ) ) 
#endif
		|| ::IsBadWritePtr( lpDVI, cbSize ) )
	{
		ASSERT_RETURN( S_FALSE );
	}

	::ZeroMemory( lpDVI, cbSize );
	lpDVI->cbSize = cbSize;
	
	CGetFileVersion fvi;
	if( fvi.Open( hModule ) )
	{
		VS_FIXEDFILEINFO vsffi = fvi.GetVSFFI();

		if( vsffi.dwFileType == VFT_DLL ||
			vsffi.dwFileType == VFT_STATIC_LIB )
		{
			switch( vsffi.dwFileOS )
			{			
			case VOS__WINDOWS32:
			case VOS_NT_WINDOWS32:
				lpDVI->dwPlatformID = DLLVER_PLATFORM_WINDOWS;
				break;
			case VOS_NT:
				lpDVI->dwPlatformID = DLLVER_PLATFORM_NT;
				break;			
			default:
				return ( S_FALSE );
			}
			
			lpDVI->dwMajorVersion = HIWORD( vsffi.dwFileVersionMS );
			lpDVI->dwMinorVersion = LOWORD( vsffi.dwFileVersionMS );
			lpDVI->dwBuildNumber  = HIWORD( vsffi.dwFileVersionLS );
			
		#ifdef DLLVERSIONINFO2

			if( cbSize == sizeof( DLLVERSIONINFO2 ) )
			{
				DLLVERSIONINFO2* lpDVI2 = (DLLVERSIONINFO2*)lpDVI;
				lpDVI2->ullVersion = MAKEDLLVERULL( 
					lpDVI->dwMajorVersion,
					lpDVI->dwMinorVersion,
					lpDVI->dwBuildNumber ,
					LOWORD( vsffi.dwFileVersionLS )
				);
			}

		#endif

			return ( S_OK );
		}
	#ifdef _DEBUG
		else
			ASSERT( 0 );
	#endif

		fvi.Close();
	}

	return ( S_FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// HIWORD( ffi.dwFileVersionMS ) - major
// LOWORD( ffi.dwFileVersionMS ) - minor
// HIWORD( ffi.dwFileVersionLS ) - build
// LOWORD( ffi.dwFileVersionLS ) - QFE
/////////////////////////////////////////////////////////////////////////////

CGetFileVersion::CGetFileVersion( void )
				: m_lpbyVIB( NULL )
{		
	Close();
}

CGetFileVersion::~CGetFileVersion( void )
{
	Close();
}

LPCTSTR CGetFileVersion::s_ppszStr[] = { 
	_T( "Comments" ),			_T( "CompanyName" ),      
	_T( "FileDescription" ),	_T( "FileVersion" ),
	_T( "InternalName" ),		_T( "LegalCopyright" ),
	_T( "LegalTrademarks" ),	_T( "OriginalFilename" ),
	_T( "PrivateBuild" ),		_T( "ProductName" ),
	_T( "ProductVersion" ),		_T( "SpecialBuild" ),
	_T( "OLESelfRegister" )
};

////////////////////////////////////////////////////////////////////////////////
// Implementation

BOOL CGetFileVersion::Open( IN HINSTANCE hInstance )
{
	if( hInstance == NULL )
		ASSERT_RETURN( FALSE );

	TCHAR szFileName[ MAX_PATH ] = { 0 };
	if( ::GetModuleFileName( hInstance, szFileName, MAX_PATH ) )
		return Open( szFileName );

	return FALSE;
};

BOOL CGetFileVersion::Open( IN LPCTSTR lpszFileName )
{
	if( lpszFileName == NULL )
		ASSERT_RETURN( FALSE );

	Close();
	if( !GetVersionInfo( lpszFileName ) || !QueryVersionTrans() )
		Close();

	return m_bValid;
};

BOOL CGetFileVersion::GetVersionInfo( IN LPCTSTR lpszFileName )
{
	DWORD dwDummy = 0;
	DWORD dwSize  = ::GetFileVersionInfoSize( 
		const_cast< LPTSTR >( lpszFileName ), &dwDummy // Set to 0
	);

	if ( dwSize > 0 )
	{		
		m_lpbyVIB = (LPBYTE)malloc( dwSize );

		if ( m_lpbyVIB != NULL && 
			::GetFileVersionInfo( const_cast< LPTSTR >( lpszFileName ), 
			0, dwSize, m_lpbyVIB ) )
		{
			UINT   uLen    = 0;
			LPVOID lpVSFFI = NULL;
			
			if ( ::VerQueryValue( m_lpbyVIB, _T( "\\" ), (LPVOID*)&lpVSFFI, &uLen ) )
			{
				::CopyMemory( &m_vsffi, lpVSFFI, sizeof( VS_FIXEDFILEINFO ) );
				m_bValid = ( m_vsffi.dwSignature == VS_FFI_SIGNATURE );
			}
		}
	}

	return m_bValid;
}

BOOL CGetFileVersion::QueryVersionTrans( void )
{
	if( m_bValid == FALSE )
		ASSERT_RETURN( FALSE );

	UINT   uLen  = 0;
	LPVOID lpBuf = NULL;
  
	if( ::VerQueryValue( m_lpbyVIB, _T( "\\VarFileInfo\\Translation" ), (LPVOID*)&lpBuf, &uLen ) )
	{
		m_lpdwTrans = (LPDWORD)lpBuf;
		m_nTransCnt = ( uLen / sizeof( DWORD ) );		
	}	
	return (BOOL)( m_lpdwTrans != NULL );
}

void CGetFileVersion::Close( void )
{
	m_nTransCnt  = 0;
	m_nTransCur  = 0;
	m_bValid	 = FALSE;	
	m_lpdwTrans  = NULL;
		
	::ZeroMemory( &m_vsffi, sizeof( VS_FIXEDFILEINFO ) );
	_free( m_lpbyVIB );
}

BOOL CGetFileVersion::QueryStringValue( IN  LPCTSTR lpszItem,
										 OUT LPTSTR  lpszValue, 
										 IN  INT     nBuf ) const
{
	if( m_bValid  == FALSE || lpszItem == NULL )
		ASSERT_RETURN( FALSE );
	
	if( lpszValue != NULL && nBuf <= 0 )
		ASSERT_RETURN( FALSE );

	::ZeroMemory( lpszValue, nBuf * sizeof( TCHAR ) );

	TCHAR szSFI[ MAX_PATH ] = { 0 };
	::wsprintf( szSFI, _T( "\\StringFileInfo\\%04X%04X\\%s" ), 
		GetCurLID(), GetCurCP(), lpszItem );

	BOOL   bRes    = FALSE;
	UINT   uLen    = 0;
	LPTSTR lpszBuf = NULL;

	if( ::VerQueryValue( m_lpbyVIB, (LPTSTR)szSFI, (LPVOID*)&lpszBuf, &uLen ) )
	{
		if( lpszValue != NULL && nBuf > 0 )
			bRes = (BOOL)( ::lstrcpyn( lpszValue, lpszBuf, nBuf ) != NULL );
		else
			bRes = TRUE;
	}
	
	return ( bRes );
}

BOOL CGetFileVersion::QueryStringValue( IN  INT    nIndex, 
										 OUT LPTSTR lpszValue,
										 IN  INT    nBuf ) const
{
	if( nIndex < VI_STR_COMMENTS || 
		nIndex > VI_STR_OLESELFREGISTER ) 
	{ 
		ASSERT_RETURN( FALSE );
	}	
	return QueryStringValue( s_ppszStr[ nIndex ], lpszValue, nBuf );
}

LPCTSTR CGetFileVersion::GetVerStringName( IN INT nIndex )
{ 
	if( nIndex < VI_STR_COMMENTS || 
		nIndex > VI_STR_OLESELFREGISTER ) 
	{ 
		ASSERT_RETURN( FALSE );
	}
	return (LPCTSTR)s_ppszStr[ nIndex ];
}

INT CGetFileVersion::FindTrans( IN LANGID wLID,
								 IN WORD   wCP ) const
{	
	if( m_bValid == FALSE )
		ASSERT_RETURN( -1 );

	for( UINT n = 0; n < m_nTransCnt; n++ )
	{
		if( LOWORD( m_lpdwTrans[ n ] ) == wLID && 
			HIWORD( m_lpdwTrans[ n ] ) == wCP  )
		{		
			return n;
		}
	}
	return -1;
}

BOOL CGetFileVersion::SetTrans( IN LANGID wLID /*LANG_NEUTRAL*/, 
								 IN WORD   wCP  /*WSLVI_CP_UNICODE*/ )
{	
	if( m_bValid == FALSE )
		ASSERT_RETURN( FALSE );

	if( GetCurLID() == wLID && GetCurCP() == wCP )
		return TRUE;

	INT nPos = FindTrans( wLID, wCP );
	if( nPos != -1 ) m_nTransCur = nPos;

	return ( m_nTransCur == (UINT)nPos );
}

DWORD CGetFileVersion::GetTransByIndex( IN UINT nIndex ) const
{
	if( m_bValid == FALSE || nIndex < 0 || nIndex > m_nTransCnt )
		ASSERT_RETURN( 0 );

	return m_lpdwTrans[ nIndex ];
}

BOOL CGetFileVersion::SetTransIndex( IN UINT nIndex /*0*/ )
{
	if( m_bValid == FALSE )
		ASSERT_RETURN( FALSE );

	if( m_nTransCur == nIndex )
		return TRUE;

	if( nIndex >= 0 && nIndex <= m_nTransCnt )
		m_nTransCur = nIndex;
	
	return ( m_nTransCur == nIndex );
}

/////////////////////////////////////////////////////////////////////////////
// Static members

// If the LID identifier is unknown, it returns a 
// default string ("Language Neutral"):

BOOL CGetFileVersion::GetLIDName( IN  WORD   wLID, 
								   OUT LPTSTR lpszName, 
								   IN  INT    nBuf )
{
	if( lpszName == NULL || nBuf <= 0 )
		ASSERT_RETURN( FALSE );

	return (BOOL)::VerLanguageName( wLID, lpszName, nBuf );
}

// If the CP identifier is unknown, it returns a 
// default string ("Unknown"):

BOOL CGetFileVersion::GetCPName( IN  WORD	   wCP,
								  OUT LPCTSTR* ppszName )
{
	if( ppszName == NULL )
		ASSERT_RETURN( FALSE );

	BOOL bRes = TRUE;	
	*ppszName  = NULL;

	switch ( wCP )
	{
		case VI_CP_ASCII:	 *ppszName = _T( "7-bit ASCII" );				break;
		case VI_CP_JAPAN:	 *ppszName = _T( "Japan (Shift ?JIS X-0208)" );break;
		case VI_CP_KOREA:	 *ppszName = _T( "Korea (Shift ?KSC 5601)" );	break;
		case VI_CP_TAIWAN:	 *ppszName = _T( "Taiwan (Big5)" );				break;
		case VI_CP_UNICODE:	 *ppszName = _T( "Unicode" );					break;
		case VI_CP_LATIN2:	 *ppszName = _T( "Latin-2 (Eastern European)" );break;
		case VI_CP_CYRILLIC: *ppszName = _T( "Cyrillic" );					break;
		case VI_CP_MULTILNG: *ppszName = _T( "Multilingual" );				break;
		case VI_CP_GREEK:	 *ppszName = _T( "Greek" );						break;
		case VI_CP_TURKISH:	 *ppszName = _T( "Turkish" );					break;
		case VI_CP_HEBREW:	 *ppszName = _T( "Hebrew" );					break;
		case VI_CP_ARABIC:	 *ppszName = _T( "Arabic" );					break;		
		default:			 *ppszName = _T( "Unknown" ); bRes = FALSE;		break;
	}
	return bRes;
}

BOOL __stdcall GetFileVersion( LPCTSTR strFile, LPTSTR pszVersion, int nVersionLen )
{
	TCHAR  szVersionBuffer[8192] = TEXT("");
	DWORD dwVerSize;
	DWORD dwHandle;

	dwVerSize = GetFileVersionInfoSize( strFile, &dwHandle );
	if( dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer)-1) )
		return FALSE;

	if( GetFileVersionInfo( strFile,0,dwVerSize,szVersionBuffer) )
	{
		VS_FIXEDFILEINFO * pInfo;
		unsigned int nInfoLen;

		if( VerQueryValue( szVersionBuffer, TEXT("\\"), (void**)&pInfo, &nInfoLen ) )
		{
			memset( pszVersion, 0, nVersionLen * sizeof(TCHAR));
			_stprintf_s( pszVersion, nVersionLen-1, TEXT("%d.%d.%d.%d"), 
				HIWORD( pInfo->dwFileVersionMS ), LOWORD( pInfo->dwFileVersionMS ), 
				HIWORD( pInfo->dwFileVersionLS ), LOWORD( pInfo->dwFileVersionLS ) );
			return TRUE;
		}
	}

	return FALSE;
}
unsigned __int64 __stdcall GetFileVersion(LPCTSTR fn)
{
	unsigned __int64 ret = 0;

	DWORD buff[4];
	VS_FIXEDFILEINFO* pvsf = (VS_FIXEDFILEINFO*)buff;
	DWORD d; // a variable that GetFileVersionInfoSize sets to zero (but why is it needed ?????????????????????????????? :)
	DWORD len = GetFileVersionInfoSize((TCHAR*)fn, &d);

	if(len)
	{
		TCHAR* b1 = new TCHAR[len];
		if(b1)
		{
			UINT uLen;
			if(GetFileVersionInfo((TCHAR*)fn, 0, len, b1) && VerQueryValue(b1, _T("\\"), (void**)&pvsf, &uLen))
			{
				ret = ((unsigned __int64)pvsf->dwFileVersionMS<<32) | pvsf->dwFileVersionLS;
			}

			delete [] b1;
		}
	}

	return ret;
}

unsigned __int64 __stdcall GetVersionByStringEx(LPCTSTR pVersion)
{
	DWORD wVersion[4] = {0,0,0,0};
	_stscanf(pVersion, _T("%d.%d.%d.%d"), wVersion, wVersion+1, wVersion+2, wVersion+3);

	LARGE_INTEGER tempVer;
	tempVer.HighPart = ((wVersion[0]&0xFFFF)<<16) | (wVersion[1]&0xFFFF);
	tempVer.LowPart  = ((wVersion[2]&0xFFFF)<<16) | (wVersion[3]&0xFFFF);

	return tempVer.QuadPart;	
}

/////////////////////////////////////////////////////////////////////////////