#include "Render/RenderEngine.h"

// Creating Critical section interface
std::mutex RC_CriticalSection;
#define LOADINGCOMMAND_CRITICAL_SECTION std::scoped_lock<std::mutex> criticalSection (RC_CriticalSection);

// Function to run render thread
static unsigned RunThisThread(void* thisPtr)
{
	RenderThread* const self = (RenderThread*)thisPtr;
	self->Run();

	return 0;
}

RenderThread::RenderThread(RenderEngine* pRenderEngine) :
	m_pRenderEngine(pRenderEngine),
	m_nRenderThreadId(0),
	m_nCurrentFrame(0),
	m_nFrameFill(1),
	m_nFlush(0),
	m_pThread(nullptr)
{
	m_nMainThreadId = ::GetCurrentThreadId();

	m_Commands[0].Clear();
	m_Commands[1].Clear();
}

RenderThread::~RenderThread()
{

}

// Render Loop
void RenderThread::Run()
{
	m_nRenderThreadId = ::GetCurrentThreadId();

	while (true)
	{
		WaitForMainThreadSignal();

		RC_BeginFrame();

		ProcessCommands();

		SignalMainThread();

		m_pRenderEngine->Update();

		RC_EndFrame();

		if (m_pRenderEngine->GetQuit())
			break;
	}
}

void RenderThread::Start()
{
	if (!m_pThread)
	{
		m_pThread = std::unique_ptr<std::thread>(new std::thread(RunThisThread, this));
	}
}

bool RenderThread::IsRenderThread()
{
	return m_nRenderThreadId == ::GetCurrentThreadId();
}

// We have 2 buffers. One we fill, another - execute
void RenderThread::NextFrame()
{
	m_nCurrentFrame = (m_nCurrentFrame + 1) & 1;
	m_nFrameFill = (m_nFrameFill + 1) & 1;
}

bool RenderThread::CheckFlushCond()
{
	return *(int*)&m_nFlush != 0;
}

// Signal main thread, that he can continue his work
void RenderThread::SignalMainThread()
{
	m_nFlush = 0;
}

// Signal render thread, that he can continue his work
void RenderThread::SignalRenderThread()
{
	m_nFlush = 1;
}

// Process commands that render thread received from main thread
void RenderThread::ProcessCommands()
{
	assert(IsRenderThread());

	if (!CheckFlushCond())
		return;

	int n = 0;

	while (n < m_Commands[m_nCurrentFrame].Capacity())
	{
		byte* ptr = *m_Commands[m_nCurrentFrame] + n;
		n += sizeof(UINT32);
		UINT32 nCommandType = *((UINT32*)ptr);

		switch (nCommandType)
		{
			case eRC_Init:
			{
				m_pRenderEngine->RT_Init();
				break;
			}
			case eRC_SetupDefaultCamera:
			{
				m_pRenderEngine->RT_SetupDefaultCamera();
				break;
			}
			case eRC_SetupDefaultCompositor:
			{
				m_pRenderEngine->RT_SetupDefaultCompositor();
				break;
			}
			case eRC_LoadDefaultResources:
			{
				m_pRenderEngine->RT_LoadDefaultResources();
				break;
			}
			case eRC_LoadOgreHead:
			{
				m_pRenderEngine->RT_LoadOgreHead();
				break;
			}
			case eRC_UpdateActorPosition:
			{
				// Read command
				SceneObject* actor = ReadCommand<SceneObject*>(n);
				float x = ReadCommand<float>(n);
				float y = ReadCommand<float>(n);
				float z = ReadCommand<float>(n);
				m_pRenderEngine->RT_UpdateActorPosition(actor, Ogre::Vector3(x,y,z));
				break;
			}
			case eRC_UpdateActorScale:
			{
				SceneObject* actor = ReadCommand<SceneObject*>(n);
				float xScale = ReadCommand<float>(n);
				float yScale = ReadCommand<float>(n);
				float zScale = ReadCommand<float>(n);
				m_pRenderEngine->RT_UpdateActorScale(actor, Ogre::Vector3(xScale, yScale, zScale));
				break;
			}
			case eRC_SetupDefaultLight:
			{
				m_pRenderEngine->RT_SetupDefaultLight();
				break;
			}
			case eRC_OscillateCamera:
			{
				float time = ReadCommand<float>(n);
				m_pRenderEngine->RT_OscillateCamera(time);
				break;
			}
			case eRC_MoveCamera:
			{
				float x = ReadCommand<float>(n);
				float y = ReadCommand<float>(n);
				float z = ReadCommand<float>(n);
				m_pRenderEngine->RT_MoveCamera(Ogre::Vector3(x, y, z));
				break;
			}
			case eRC_CreateSceneObject:
			{
				uint32_t index = ReadCommand<uint32_t>(n);
				uint32_t len = ReadCommand<uint32_t>(n);

				Ogre::String str;
				str.resize(len);
				ReadMem(n, (byte*)str.c_str(), len);

				SceneObject* node = m_pRenderEngine->RT_CreateSceneObject(str, str);
				m_deliveryQueue[m_nFrameFill][index] = node;
				break; 
			}
		}
	}

	m_Commands[m_nCurrentFrame].Clear();
}

// We process comands via byte* using std::vector as raw data.
template <class T>
T RenderThread::ReadCommand(int& nIndex)
{
	byte* Res = *m_Commands[m_nCurrentFrame] + nIndex;
	nIndex += sizeof(T);
	return *reinterpret_cast<const T*>(Res);
}

void RenderThread::ReadMem(int& nIndex, byte* dest, int sz)
{
	byte* Res = *m_Commands[m_nCurrentFrame] + nIndex;
	memcpy(dest, Res, sz);
	nIndex += sz;
}

byte* RenderThread::AddCommand(RenderCommand eRC, size_t nParamBytes)
{
	UINT32 cmdSize = sizeof(RenderCommand) + nParamBytes;
	byte* ptr = m_Commands[m_nFrameFill].Resize(m_Commands[m_nFrameFill].Capacity() * sizeof(byte) + cmdSize);
	AddDWORD(ptr, eRC);
	return ptr;
}

void RenderThread::AddDWORD(byte*& ptr, UINT32 nVal)
{
	*(UINT32*)ptr = nVal;
	ptr += sizeof(UINT32);
}

void RenderThread::AddFloat(byte*& ptr, const float fVal)
{
	*(float*)ptr = fVal;
	ptr += sizeof(float);
}

template <class T>
void RenderThread::AddWTF(byte*& ptr, T TVal)
{
	static_assert(std::is_trivially_copyable_v<T>);
	*(T*)ptr = TVal;
	ptr += sizeof(T);
}

inline void RenderThread::AddBytes(byte*& ptr, byte* copy, uint32_t sz)
{
	memcpy(ptr, copy, sz);
	ptr += sz;
}

void RenderThread::RC_Init()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_Init();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_Init, 0);
}

void RenderThread::RC_SetupDefaultCamera()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_SetupDefaultCamera();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_SetupDefaultCamera, 0);
}

void RenderThread::RC_SetupDefaultCompositor()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_SetupDefaultCompositor();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_SetupDefaultCompositor, 0);
}

void RenderThread::RC_LoadDefaultResources()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_LoadDefaultResources();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_LoadDefaultResources, 0);
}

void RenderThread::RC_LoadOgreHead()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_LoadOgreHead();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_LoadOgreHead, 0);
}

void RenderThread::RC_UpdateActorPosition(SceneObject* actor, Ogre::Vector3 pos)
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_UpdateActorPosition(actor, pos);
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	size_t total = sizeof(SceneObject*) + 3*sizeof(float);
	byte* p = AddCommand(eRC_UpdateActorPosition, total);
	AddWTF<SceneObject*>(p, actor);
	AddFloat(p, pos.x);
	AddFloat(p, pos.y);
	AddFloat(p, pos.z);
}

void RenderThread::RC_UpdateActorScale(SceneObject* actor, Ogre::Vector3 scale)
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_UpdateActorPosition(actor, scale);
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	size_t total = sizeof(SceneObject*) + 3 * sizeof(float);
	byte* p = AddCommand(eRC_UpdateActorScale, total);
	AddWTF<SceneObject*>(p, actor);
	AddFloat(p, scale.x);
	AddFloat(p, scale.y);
	AddFloat(p, scale .z);
}

void RenderThread::RC_SetupDefaultLight()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_SetupDefaultLight();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_SetupDefaultLight, 0);
}

void RenderThread::RC_OscillateCamera(float time)
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_OscillateCamera(time);
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_OscillateCamera, sizeof(float));
	AddFloat(p, time);
}

void RenderThread::RC_MoveCamera(Ogre::Vector3 tVector)
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_MoveCamera(tVector);
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_MoveCamera, 3*sizeof(float));
	AddFloat(p, tVector.x);
	AddFloat(p, tVector.y);
	AddFloat(p, tVector.z);
}

uint32_t RenderThread::RC_CreateSceneObject(const Ogre::String& meshName)
{
	LOADINGCOMMAND_CRITICAL_SECTION;

	static uint32_t index = 0; // TODO: redo to reusable value instead

	uint32_t len = meshName.size();
	byte* p = AddCommand(eRC_CreateSceneObject, sizeof(uint32_t) + sizeof(uint32_t) + len);
	AddWTF(p, index);
	AddWTF(p, len);
	AddBytes(p, (byte*)meshName.c_str(), len);
	return index++;
}

void RenderThread::RC_BeginFrame()
{

}

void RenderThread::RC_EndFrame()
{
	if (IsRenderThread())
		return;

	SyncMainWithRender();
}

void RenderThread::SyncMainWithRender()
{
	assert(!IsRenderThread());

	WaitForRenderThreadSignal();

	// Switch buffers
	{
		LOADINGCOMMAND_CRITICAL_SECTION;
		NextFrame();
		m_Commands[m_nFrameFill].Clear();
	}

	SignalRenderThread();
}

// Wait signal from main thread
void RenderThread::WaitForMainThreadSignal()
{
	assert(IsRenderThread());

	while (!m_nFlush)
	{
	}
}

// Wait signal from render thread
void RenderThread::WaitForRenderThreadSignal()
{
	assert(!IsRenderThread());

	while (m_nFlush)
	{
	}
}

std::map<uint32_t, void*>& RenderThread::GetDeliveryQueue() 
{
	return m_deliveryQueue[m_nFrameFill];
}