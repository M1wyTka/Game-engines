#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>

#include "GeneralDefines.h"
#include "MTQueue.h"
#include "function2/function2.hpp"
//#include "function2.hpp"

class RenderEngine;

class RenderThread
{
public:
	RenderThread(RenderEngine* pRenderEngine);
	~RenderThread();

	RenderThread(const RenderThread&) = delete;
	RenderThread& operator=(const RenderThread&) = delete;

	void Start();
	void Run();

	void RC_BeginFrame();
	void RC_EndFrame();

	void RC_LambdaAction(fu2::unique_function<void()> lambdaToAdd);
private:
	threadID m_nRenderThreadId;
	threadID m_nMainThreadId;

	volatile UINT32 m_nFlush;

	std::unique_ptr<std::thread> m_pThread;

	RenderEngine* m_pRenderEngine;

	class Node
	{	
	public:
		Node* next;
		fu2::unique_function<void()> f;
	};

	std::atomic<Node*> m_commandQueueHead{ nullptr };

	int m_nCurrentFrame;
	int m_nFrameFill;

	bool IsRenderThread();

	void RC_ProcessLambdaCommands();

	void NextFrame();

	inline bool CheckFlushCond();
	inline void SignalMainThread();
	inline void SignalRenderThread();
	void WaitForMainThreadSignal();
	void WaitForRenderThreadSignal();
	void SyncMainWithRender();
};
