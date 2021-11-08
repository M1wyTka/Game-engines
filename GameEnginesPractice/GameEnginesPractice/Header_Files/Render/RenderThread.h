#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>

#include "GeneralDefines.h"
#include "MTQueue.h"

class RenderEngine;

enum RenderCommand : UINT32
{
	eRC_Unknown = 0,
	eRC_Init,
	eRC_SetupDefaultCamera,
	eRC_SetupDefaultCompositor,
	eRC_LoadDefaultResources,
	eRC_LoadOgreHead,
	eRC_UpdateActorPosition,
	eRC_UpdateActorScale,
	eRC_SetupDefaultLight,
	eRC_OscillateCamera,
	eRC_MoveCamera,
	eRC_CreateSceneObject,
	eRC_BeginFrame,
	eRC_EndFrame
};

class RenderThread
{
public:
	RenderThread(RenderEngine* pRenderEngine);
	~RenderThread();

	RenderThread(const RenderThread&) = delete;
	RenderThread& operator=(const RenderThread&) = delete;

	void Start();
	void Run();

	void RC_Init();
	void RC_SetupDefaultCamera();
	void RC_SetupDefaultCompositor();
	void RC_LoadDefaultResources();
	void RC_LoadOgreHead();
	void RC_SetupDefaultLight();
	void RC_OscillateCamera(float time);
	void RC_UpdateActorPosition(SceneObject* actor, Ogre::Vector3 pos);
	void RC_UpdateActorScale(SceneObject* actor, Ogre::Vector3 pos);
	void RC_MoveCamera(Ogre::Vector3 pos);
	uint32_t RC_CreateSceneObject(const Ogre::String& meshName);
	void RC_BeginFrame();
	void RC_EndFrame();

	std::map<uint32_t, void*>& GetDeliveryQueue();
	void ReadMem(int& nIndex, byte* dest, int sz);

private:
	threadID m_nRenderThreadId;
	threadID m_nMainThreadId;

	volatile UINT32 m_nFlush;

	std::unique_ptr<std::thread> m_pThread;

	RenderEngine* m_pRenderEngine;

	MTQueue<byte> m_Commands[2];
	int m_nCurrentFrame;
	int m_nFrameFill;

	std::map<uint32_t, void*> m_deliveryQueue[2];

	template <class T>
	T ReadCommand(int& nIndex);

	inline byte* AddCommand(RenderCommand eRC, size_t nParamBytes);
	inline void AddDWORD(byte*& ptr, UINT32 nVal);
	inline void AddFloat(byte*& ptr, const float fVal);
	inline void AddBytes(byte*& ptr, byte* copy, uint32_t sz);

	template <class T>
	void AddWTF(byte*& ptr, T TVal);

	bool IsRenderThread();

	void ProcessCommands();
	void NextFrame();

	inline bool CheckFlushCond();
	inline void SignalMainThread();
	inline void SignalRenderThread();
	void WaitForMainThreadSignal();
	void WaitForRenderThreadSignal();
	void SyncMainWithRender();
};
