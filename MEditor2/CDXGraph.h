//
// CDXGraph.h
//

#ifndef __H_CDXGraph__
#define __H_CDXGraph__

// Filter graph notification to the specified window
#define WM_GRAPHNOTIFY  (WM_USER+20)
#define SLIDERHEIGHT 20

#include "amstream.h"
#include "dshow.h"
//#include "qedit.h"

class CDXGraph
{
private:
	IGraphBuilder *     mGraph;  
	IMediaControl *		mMediaControl;
	IMediaEventEx *		mEvent;
	IBasicVideo *		mBasicVideo;
	IBasicAudio *		mBasicAudio;
	IVideoWindow  *		mVideoWindow;
	IBaseFilter  *	mVideoFilter;
	IMediaSeeking *		mSeeking;
	IVideoFrameStep *mFrameStep;

	DWORD				mObjectTableEntry; 

public:
	CDXGraph();
	virtual ~CDXGraph();

public:
	virtual bool Create(void);
	virtual void Release(void);
	virtual bool Attach(IGraphBuilder * inGraphBuilder);

	IGraphBuilder * GetGraph(void); // Not outstanding reference count
	IMediaEventEx * GetEventHandle(void);

	HRESULT FindDeviceFilter(IBaseFilter ** ppSrcFilter,GUID deviceGUID);
	bool AddFilters(const GUID deviceGUID);
	bool ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);
	void DisconnectFilters(IPin * inOutputPin);

	bool SetDisplayWindow(HWND inWindow,bool ChangeWindowSize = 1);
	bool SetNotifyWindow(HWND inWindow);
	void MoveVideoWindow(HWND inWindow);
	bool MoveVideoWindow(long inLeft = 0, long inTop = 0);
	bool ResizeVideoWindow(long inLeft, long inTop, long inWidth, long inHeight,bool KeepAspect = 0);
	void HandleEvent(WPARAM inWParam, LPARAM inLParam);
	bool GetVideoRect(RECT *rc);
	bool ShowSlider(bool Show = 1);
	bool GetFrameStepInterface(void);
	bool StepOneFrame(void);

	bool Run(void);        // Control filter graph
	bool Stop(void);
	bool Pause(void);
	bool IsRunning(void);  // Filter graph status
	bool IsStopped(void);
	bool IsPaused(void);

	bool SetFullScreen(BOOL inEnabled);
	bool GetFullScreen(void);
	bool OnTop(BOOL inEnabled = TRUE);

	// IMediaSeeking
	bool GetCurrentPosition(double * outPosition);
	bool GetStopPosition(double * outPosition);
	bool SetCurrentPosition(double inPosition);
	bool SetStartStopPosition(double inStart, double inStop);
	bool GetDuration(double * outDuration);
	bool SetPlaybackRate(double inRate);

	// Attention: range from -10000 to 0, and 0 is FULL_VOLUME.
	bool SetAudioVolume(long inVolume);
	long GetAudioVolume(void);
	// Attention: range from -10000(left) to 10000(right), and 0 is both.
	bool SetAudioBalance(long inBalance);
	long GetAudioBalance(void);
#ifdef UNICODE
	bool RenderFile(LPCWSTR inFile);
#else
	bool RenderFile(const char * inFile);
#endif
	bool QueryInterfaces(void);
	bool SnapshotBitmap(const char * outFile);

//private:
//	void AddToObjectTable(void) ;
//	void RemoveFromObjectTable(void);
	
};

#endif // __H_CDXGraph__