#include "AdjacentSystems/InputHandler.h"

#include "Ogre.h"

#include <algorithm>
#include <windows.h>

InputHandler::InputHandler(const std::string& strResourceRoot) : m_pMouseSensitivity(20)
{
	m_strMapFilePath = strResourceRoot + "actionmap.ini";
	std::replace(m_strMapFilePath.begin(), m_strMapFilePath.end(), '\\', '/');

	MapSymbol("a", 'A');
	MapSymbol("d", 'D');
	MapSymbol("w", 'W');
	MapSymbol("s", 'S');
	MapSymbol("q", 'Q');

	MapSymbol("left", VK_LEFT);
	MapSymbol("right", VK_RIGHT);
	MapSymbol("up", VK_UP);
	MapSymbol("down", VK_DOWN);
	MapSymbol("shoot", 0x51); // q

	MapCommandSymbol("GoLeft", eIC_GoLeft, "a");
	MapCommandSymbol("GoRight", eIC_GoRight, "d");
	MapCommandSymbol("GoUp", eIC_GoUp, "w");
	MapCommandSymbol("GoDown", eIC_GoDown, "s");
	MapCommandSymbol("Shoot", eIC_Shoot, "q");

	LoadConfiguration();

	Remap();
}

InputHandler::~InputHandler()
{
}

bool InputHandler::IsKeyDown(size_t vk_key)
{
	if (GetAsyncKeyState(vk_key) & 0x8000)
		return true;

	return false;
}

void InputHandler::MapSymbol(std::string strSymbol, size_t nSymbol)
{
	m_symbolMap[strSymbol] = nSymbol;
}

void InputHandler::MapInputEvent(std::size_t nSymbol, size_t nCommand)
{
	m_inputEventMap[nSymbol] = nCommand;
}

void InputHandler::MapCommandSymbol(std::string strCommand, size_t nCommand, std::string strDefaultSymbol)
{
	m_commandMap[strCommand] = nCommand;
	m_commandSymbolMap[strCommand] = strDefaultSymbol;
}

void InputHandler::LoadConfiguration()
{
	Ogre::ConfigFile cf;
	cf.load(m_strMapFilePath);

	Ogre::ConfigFile::SectionIterator secIter = cf.getSectionIterator();

	Ogre::String strInputType, strCommand, strSymbol;
	while (secIter.hasMoreElements())
	{
		strInputType = secIter.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = secIter.getNext();

		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			strCommand = i->first;
			strSymbol = i->second;

			m_commandSymbolMap[strCommand] = strSymbol;
		}
	}
}

void InputHandler::Remap()
{
	for (auto& it : m_commandSymbolMap)
	{
		MapInputEvent(m_symbolMap[it.second], m_commandMap[it.first]);
	}
}

// We used int as return type just for demonstration. It should be done another way
void InputHandler::Update()
{
	for (auto& it : m_inputEventMap)
	{
		m_InputState.set(it.second, IsKeyDown(it.first));
	}

	if (m_pWinHandle)
	{
		m_pPrevMousePos = m_pCurMousePos;

		GetCursorPos(&m_pMousePoint);
		ScreenToClient(m_pWinHandle, &m_pMousePoint);

		float x = float(m_pMousePoint.x);
		float y = float(m_pMousePoint.y);
		m_pCurMousePos = Ogre::Vector2(x, y);

		m_bMouseButtonDown = GetKeyState(VK_LBUTTON) < 0;

	}
}

void InputHandler::SetWinHandle(HWND window)
{
	m_pWinHandle = window;
	GetCursorPos(&m_pMousePoint);
	ScreenToClient(m_pWinHandle, &m_pMousePoint);

	float x = float(m_pMousePoint.x);
	float y = float(m_pMousePoint.y);
	m_pCurMousePos = Ogre::Vector2(x, y);
}

const std::bitset<eIC_Max>& InputHandler::GetInputState() const
{
	return m_InputState;
}

bool InputHandler::IsCommandActive(EInputCommand inputCommand) const
{
	return m_InputState.test(inputCommand);
}

Ogre::Vector2 InputHandler::MousePos() const
{
	return m_pCurMousePos;
}

Ogre::Vector2 InputHandler::DeltaMousePos() const
{
	Ogre::Vector2 diff = m_pCurMousePos - m_pPrevMousePos;
	return Ogre::Vector2(diff.x, -diff.y);
}

Ogre::Vector2 InputHandler::DeltaDownMousePos() const
{
	if (m_bMouseButtonDown) 
	{
		return DeltaMousePos();
	}
	else
		return Ogre::Vector2(0, 0);
}