#ifndef __ITaskbarList3_INTERFACE_DEFINED__   
#define __ITaskbarList3_INTERFACE_DEFINED__   

/* interface ITaskbarList3 */  
/* [object][uuid] */   
EXTERN_C const IID IID_ITaskbarList3;  

#if defined(__cplusplus) && !defined(CINTERFACE)   

typedef  
	enum TBPFLAG  
{     
	TBPF_NOPROGRESS = 0,  
	TBPF_INDETERMINATE  = 0x1,  
	TBPF_NORMAL = 0x2,  
	TBPF_ERROR  = 0x4,  
	TBPF_PAUSED = 0x8  
} TBPFLAG;  

typedef struct THUMBBUTTON *LPTHUMBBUTTON;  

MIDL_INTERFACE("EA1AFB91-9E28-4B86-90E9-9E9F8A5EEFAF")  
ITaskbarList3 : public ITaskbarList2  
{  
public:  
	virtual HRESULT STDMETHODCALLTYPE MarkFullscreenWindow(   
		/* [in] */ __RPC__in HWND hwnd,  
		/* [in] */ BOOL fFullscreen) = 0;  

	// ITaskbarList3 members   
	STDMETHOD (SetProgressValue)     (HWND hwnd, ULONGLONG ullCompleted, ULONGLONG ullTotal) PURE;  
	STDMETHOD (SetProgressState)     (HWND hwnd, TBPFLAG tbpFlags) PURE;  
	STDMETHOD (RegisterTab)          (HWND hwndTab,HWND hwndMDI) PURE;  
	STDMETHOD (UnregisterTab)        (HWND hwndTab) PURE;  
	STDMETHOD (SetTabOrder)          (HWND hwndTab, HWND hwndInsertBefore) PURE;  
	STDMETHOD (SetTabActive)         (HWND hwndTab, HWND hwndMDI, DWORD dwReserved) PURE;  
	STDMETHOD (ThumbBarAddButtons)   (HWND hwnd, UINT cButtons, LPTHUMBBUTTON pButton) PURE;  
	STDMETHOD (ThumbBarUpdateButtons)(HWND hwnd, UINT cButtons, LPTHUMBBUTTON pButton) PURE;  
	STDMETHOD (ThumbBarSetImageList) (HWND hwnd, HIMAGELIST himl) PURE;  
	STDMETHOD (SetOverlayIcon)       (HWND hwnd, HICON hIcon, LPCWSTR pszDescription) PURE;  
	STDMETHOD (SetThumbnailTooltip)  (HWND hwnd, LPCWSTR pszTip) PURE;  
	STDMETHOD (SetThumbnailClip)     (HWND hwnd, RECT *prcClip) PURE;  
};  

#else   /* C style interface */

#endif  /* C style interface */

#endif  /* __ITaskbarList3_INTERFACE_DEFINED__ */
