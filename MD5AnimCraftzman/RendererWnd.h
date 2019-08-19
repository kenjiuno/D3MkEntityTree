#pragma once


// CRendererWnd

class CRendererWnd : public CView
{
	// 
	struct WGL {
		// 
		HWND hWndAssoc;
		// 
		HDC hDC;
		// 
		HGLRC hRC;
		// 
		PIXELFORMATDESCRIPTOR pfd;

		// 
		WGL()
		{
			hWndAssoc = NULL; hDC = NULL; hRC = NULL;
		}
		// 
		~WGL()
		{
			Close();
		}
		// 
		void Close()
		{
			if (hRC == wglGetCurrentContext()) {
				wglMakeCurrent(NULL, NULL);
			}

			if (hRC != NULL) {
				wglDeleteContext(hRC);
				hRC = NULL;
			}

			if (hDC != NULL) {
				::ReleaseDC(hWndAssoc, hDC);
				hDC = NULL;
			}

			hWndAssoc = NULL;
		}
		// 
		bool Assoc(HWND hWnd)
		{
			Close();

			ZeroMemory(&pfd, sizeof(pfd));

			for (; ; ) {
				if (hWnd == NULL)
					break;
				hDC = ::GetDC(hWnd);
				if (hDC == NULL)
					break;

				pfd.nSize = sizeof(pfd);
				pfd.nVersion = 1;

				pfd.dwFlags = 0
					|PFD_DRAW_TO_WINDOW
					|PFD_SUPPORT_OPENGL
					|PFD_DOUBLEBUFFER
					;
				pfd.iPixelType = PFD_TYPE_RGBA;
				pfd.cColorBits = 32;
				pfd.cAlphaBits = 8;
				pfd.cDepthBits = 16;

				int i = ChoosePixelFormat(hDC, &pfd);
				if (i == 0)
					break;
				if (!SetPixelFormat(hDC, i, &pfd))
					break;

				hRC = wglCreateContext(hDC);
				if (hRC == NULL)
					break;
				if (!wglMakeCurrent(hDC, hRC))
					break;

				hWndAssoc = hWnd;
				return true;
			}
			if (hRC != NULL) wglDeleteContext(hRC), hRC = NULL;
			if (hDC != NULL) ::ReleaseDC(hWnd, hDC), hDC = NULL;
			return false;
		}
		// 
		bool Restore(bool fInit, HWND hWnd = NULL)
		{
			if (hWndAssoc == NULL) {
				if (!fInit) return false;
				if (!Assoc(hWnd)) return false;

				return true;
			}

			if (hRC == wglGetCurrentContext())
				return true;

			if (wglMakeCurrent(hDC, hRC))
				return true;

			return false;
		}
	};

	// 
	WGL wGL;

	// 
	DECLARE_DYNAMIC(CRendererWnd)

public:
	// 
	CRendererWnd();
	// 
	virtual ~CRendererWnd();
	// 
	bool Create(CWnd *pParentWnd, UINT nID, CCreateContext *pContext)
	{
		if (!CView::Create(
			AfxRegisterWndClass(
				0 |CS_HREDRAW |CS_VREDRAW,
				::LoadCursor(NULL, IDC_ARROW),
				NULL,
				::LoadIcon(NULL, IDI_EXCLAMATION)
				),
			"Renderer",
			0 |WS_CHILD |WS_VISIBLE,
			CRect(0, 0, 0, 0),
			pParentWnd,
			nID,
			pContext
			)
		) {
			return false;
		}
		return true;
	}
	// 
	void DoPaint();

	// 
	virtual void wReshape(int cx, int cy);
	// 
	virtual void wDisplay();

protected:
	// 
	bool Restore(bool fInit);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
};


