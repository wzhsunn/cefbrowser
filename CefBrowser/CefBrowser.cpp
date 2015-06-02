
// CefBrowser.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CefBrowser.h"
#include "CefBrowserDlg.h"

#include "XClientApp.h"
#include "XUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCefBrowserApp

BEGIN_MESSAGE_MAP(CCefBrowserApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCefBrowserApp ����

CCefBrowserApp::CCefBrowserApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCefBrowserApp ����

CCefBrowserApp theApp;


// CCefBrowserApp ��ʼ��

BOOL CCefBrowserApp::InitInstance()
{
    {
        CefMainArgs main_args(::GetModuleHandle(NULL));
        CefRefPtr<XClientApp> app(new XClientApp);

        // Execute the secondary process, if any.
        int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
        if (exit_code >= 0)
            return exit_code;

        // Parse command line arguments. The passed in values are ignored on Windows.
        AppInitCommandLine(0, NULL);

        CefSettings settings;

        // Populate the settings based on command line arguments.
        AppGetSettings(settings);

        // Initialize CEF.
        CefInitialize(main_args, settings, app.get(), nullptr);

        // Init plugins, like Flash etc.
        InitWebPlugins();
    }

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	CCefBrowserDlg* pDlg = new CCefBrowserDlg;
    m_pMainWnd = pDlg;
    pDlg->Create(MAKEINTRESOURCE(IDD_CEFBROWSER_DIALOG), NULL);
    m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
    m_pMainWnd->UpdateWindow();

    {
        // Run the CEF message loop. This function will block until the application
        // recieves a WM_QUIT message.
        CefRunMessageLoop();

        CefShutdown();
    }

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CCefBrowserApp::ExitInstance()
{
    int nResult = CWinAppEx::ExitInstance();

    if(m_pMainWnd != NULL)
    {
        delete m_pMainWnd;
        m_pMainWnd = NULL;
    }

    return nResult;
}