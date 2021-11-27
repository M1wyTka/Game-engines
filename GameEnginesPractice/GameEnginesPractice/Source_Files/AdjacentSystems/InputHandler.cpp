#include "AdjacentSystems/InputHandler.h"

#include "Ogre.h"

#include <algorithm>
#include <windows.h>

InputHandler::InputHandler(const std::string& strResourceRoot) : m_pMouseSensitivity(20)
{
	m_strMapFilePath = strResourceRoot + "actionmap.ini";
	std::replace(m_strMapFilePath.begin(), m_strMapFilePath.end(), '\\', '/');

	FillSymbolMap();
	FillCommandMap();
	FillCommandSymbolMap();
	
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

void InputHandler::MapCommand(std::string strCommand, size_t nCommand) 
{
	m_commandMap[strCommand] = nCommand;
}

//void InputHandler::MapCommandSymbol(std::string strCommand, size_t nCommand, std::string strDefaultSymbol)
//{
//	MapCommand(strCommand, nCommand);
//	m_commandSymbolMap[strCommand] = strDefaultSymbol;
//}

void InputHandler::MapCommandSymbol(std::string strCommand,std::string strDefaultSymbol)
{
	m_commandSymbolMap[strCommand] = strDefaultSymbol;
}

void InputHandler::LoadConfiguration()
{
	Ogre::ConfigFile cf;
	Ogre::String separator = ":=";
	cf.load(m_strMapFilePath, separator, true);

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

void InputHandler::MapInputEvent(std::size_t nSymbol, size_t nCommand)
{
	m_inputEventMap[nSymbol] = nCommand;
}


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

		m_bMouseButtonDown = GetKeyState(LMB) < 0;
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
	return Ogre::Vector2(0, 0);
}

void InputHandler::FillSymbolMap() 
{
	// Key name in file to enum mapping
	MapSymbol("a", A_KEY);
	MapSymbol("d", D_KEY);
	MapSymbol("w", W_KEY);
	MapSymbol("s", S_KEY);
	MapSymbol("q", Q_KEY);
}

void InputHandler::FillCommandMap() 
{
	// Name of command to enum mapping
	MapCommand("GoLeft", eIC_GoLeft);
	MapCommand("GoRight", eIC_GoRight);
	MapCommand("GoUp", eIC_GoUp);
	MapCommand("GoDown", eIC_GoDown);
	MapCommand("Shoot", eIC_Shoot);
}

void InputHandler::FillCommandSymbolMap()
{
	//MapCommandSymbol("GoLeft", "a");
	//MapCommandSymbol("GoRight", "d");
	//MapCommandSymbol("GoUp", "w");
	//MapCommandSymbol("GoDown", "s");
	//MapCommandSymbol("Shoot", "q");
}
