================================================================================
    MICROSOFT �������: DBSchemaMaintainer ��Ŀ����
===============================================================================

Ӧ�ó�������Ϊ�������� DBSchemaMaintainer Ӧ�ó��򡣴�Ӧ�ó��򲻽���ʾʹ�� Microsoft ������Ļ���֪ʶ�����ҿ���Ϊ��дӦ�ó������㡣

���ļ�������� DBSchemaMaintainer Ӧ�ó���ĸ����ļ�������ժҪ��

DBSchemaMaintainer.vcproj
    ����ʹ��Ӧ�ó��������ɵ� VC++ ��Ŀ������Ŀ�ļ���
    �������й������ļ��� Visual C++ �汾����Ϣ���Լ��й�ʹ��Ӧ�ó�����ѡ���ƽ̨�����ú���Ŀ���ܵ���Ϣ��

DBSchemaMaintainer.h
    ����Ӧ�ó������Ҫͷ�ļ���������������Ŀ�ض���ͷ�ļ�(���� Resource.h)�������� CDBSchemaMaintainerApp Ӧ�ó����ࡣ

DBSchemaMaintainer.cpp
    ���ǰ���Ӧ�ó����� CDBSchemaMaintainerApp ����ҪӦ�ó���Դ�ļ���

DBSchemaMaintainer.rc
    ���ǳ���ʹ�õ����� Microsoft Windows ��Դ���б������� RES ��Ŀ¼�д洢��ͼ�ꡢλͼ�͹�ꡣ���ļ�����ֱ���� Microsoft Visual C++ �н��б༭����Ŀ��Դλ�� 2052 �С�

res\DBSchemaMaintainer.ico
    ��������Ӧ�ó���ͼ���ͼ���ļ�����ͼ���������Ҫ��Դ�ļ� DBSchemaMaintainer.rc �С�

res\DBSchemaMaintainer.rc2
    ���ļ����������� Microsoft Visual C++ �༭����Դ����Ӧ�ý���������Դ�༭���༭��������Դ���ڴ��ļ��С�

DBSchemaMaintainer.reg
    ����һ��ʾ�� .reg �ļ�������ʾ�˿�ܽ�Ϊ�����õ�ע�����õ����ࡣ���Խ�������
    ��������Ӧ�ó���һ��ʹ�õ� .reg �ļ����������Խ���ɾ����������Ĭ�ϵ� RegisterShellFileTypes ע�ᡣ


/////////////////////////////////////////////////////////////////////////////

��������ܴ���:
    ��Ŀ������׼ MFC ���档

MainFrm.h, MainFrm.cpp
    ��Щ�ļ���������� CMainFrame������������
    CMDIFrameWnd ���������� MDI ��ܹ��ܡ�

res\Toolbar.bmp
    ��λͼ�ļ�����Ϊ����������ƽ��ͼ��
    ��ʼ��������״̬������ CMainFrame ���й���ġ�ʹ����Դ�༭���༭�˹�����λͼ�������� DBSchemaMaintainer.rc �е� IDR_MAINFRAME TOOLBAR ��������ӹ�������ť��
/////////////////////////////////////////////////////////////////////////////

�����ӿ�ܴ���:

ChildFrm.h, ChildFrm.cpp
    ��Щ�ļ����岢ʵ�� CChildFrame �࣬����֧�� MDI Ӧ�ó����е��Ӵ��ڡ�

/////////////////////////////////////////////////////////////////////////////

Ӧ�ó����򵼴���һ���ĵ����ͺ�һ����ͼ:

DBSchemaMaintainerDoc.h��DBSchemaMaintainerDoc.cpp - �ĵ�
    ��Щ�ļ����� CDBSchemaMaintainerDoc �ࡣ�༭��Щ�ļ�������������ĵ����ݲ���ʵ���ļ�����ͼ���(ͨ�� CDBSchemaMaintainerDoc::Serialize)��
    �ĵ������������ַ���:
        �ļ���չ��:dbschema
        �ļ����� ID:DBSchemaMaintainer.DBSchemaDocument
        ����ܱ���:DBSchemaMaintainer
        �ĵ���������:DBSchema
        ɸѡ����:DBSchema Files (*.dbschema)
        �ļ����¼��:DBSchema
        �ļ�����ȫ��:DBSchemaMaintainer.DBSchemaDocument

DBSchemaTableView.h��DBSchemaTableView.cpp - �ĵ�����ͼ
    ��Щ�ļ����� CDBSchemaTableView �ࡣ
    CDBSchemaTableView �������ڲ鿴 CDBSchemaMaintainerDoc ����

res\DBSchemaMaintainerDoc.ico
    ����ͼ���ļ��������� CDBSchemaMaintainerDoc ��� MDI �Ӵ��ڵ�ͼ�ꡣ��ͼ���������Ҫ��Դ�ļ� DBSchemaMaintainer.rc �С�




/////////////////////////////////////////////////////////////////////////////

��������:

ActiveX �ؼ�
    Ӧ�ó��������ʹ�� ActiveX �ؼ���֧�֡�

��ӡ����ӡԤ��֧��
    Ӧ�ó�������ͨ���� MFC ����� CView ���еĳ�Ա���������������ڴ����ӡ����ӡ���úʹ�ӡԤ������Ĵ��롣

/////////////////////////////////////////////////////////////////////////////

������׼�ļ�:

StdAfx.h, StdAfx.cpp
    ��Щ�ļ�����������Ϊ DBSchemaMaintainer.pch ��Ԥ����ͷ(PCH)�ļ�����Ϊ StdAfx.obj ��Ԥ���������ļ���

Resource.h
    ���Ǳ�׼ͷ�ļ�������������Դ ID��
    Microsoft Visual C++ ����ȡ�����´��ļ���

DBSchemaMaintainer.manifest
	Windows XP ʹ��Ӧ�ó����嵥�ļ�����Ӧ�ó���	���ض��汾���г��򼯵������ԡ����س���ʹ�ô�	��Ϣ�ӳ��򼯻��������Ӧ���򼯻�	��Ӧ�ó������˽����Ϣ��Ӧ�ó����嵥������Ϊ	��Ӧ�ó����ִ���ļ���װ��ͬһ�ļ����е��ⲿ .manifest �ļ����������Ա����·�����	Ҳ��������Դ����ʽ�����ڸÿ�ִ���ļ��С�
/////////////////////////////////////////////////////////////////////////////

����ע��:

Ӧ�ó�����ʹ�á�TODO:��ָʾӦ��ӻ��Զ����Դ���벿�֡�

���Ӧ�ó����ڹ��� DLL ��ʹ�� MFC,����Ҫ���·��� MFC DLL�����Ӧ�ó������������ϵͳ���������ò�ͬ����Ҳ���������·�����Ӧ�ı��ػ���Դ MFC90XXX.DLL��
�й��������������ϸ��Ϣ������� MSDN �ĵ����й����·��� Visual C++ Ӧ�ó���Ĳ��֡�

/////////////////////////////////////////////////////////////////////////////