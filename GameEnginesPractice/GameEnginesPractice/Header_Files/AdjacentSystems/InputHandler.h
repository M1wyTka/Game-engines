#pragma once
#include "Input.h"

#include <string>
#include <unordered_map>
#include <bitset>

#include "Ogre.h"

#include "SDL.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"


class InputHandler
{
public:
	InputHandler(const std::string& strResourceRoot);
	~InputHandler();

	void Update();

	bool GetQuit() const { return m_bIsQuit;  };
	const std::bitset<eIC_Max>& GetInputState() const;
	bool IsCommandActive(EInputCommand inputCommand) const;

	Ogre::Vector2 MousePos() const;
	Ogre::Vector2 DeltaMousePos() const;
	Ogre::Vector2 DeltaDownMousePos() const;
	float GetMouseSensitivity() const { return m_pMouseSensitivity; }

private:
	void LoadConfiguration();

	bool IsKeyDown(size_t vk_key);

	void FillSymbolMap();
	void FillCommandMap();
	void FillCommandSymbolMap();

	void MapSymbol(std::string strSymbol, size_t nSymbol);
	void MapInputEvent(std::size_t nSymbol, size_t nCommand);
	void MapCommand(std::string strCommand, size_t nCommand);
	void MapCommandSymbol(std::string strCommand, std::string strDefaultSymbol);
	//void MapCommandSymbol(std::string strCommand, size_t nCommand, std::string strDefaultSymbol);
	void Remap();

	void ReadMappedButtonInput();
	void ReadMouseInput();

	bool m_bIsQuit;

	std::string m_strMapFilePath;

	typedef std::unordered_map<std::string, size_t> TCommandMap;
	typedef std::unordered_map<std::string, size_t> TSymbolMap;
	typedef std::unordered_map<size_t, size_t> TInputEventMap;
	typedef std::unordered_map<std::string, std::string> TCommandSymbolMap;

	TCommandMap m_commandMap;
	TSymbolMap m_symbolMap;
	TInputEventMap m_inputEventMap;
	TCommandSymbolMap m_commandSymbolMap;

	std::bitset<eIC_Max> m_InputState;
	bool m_bMouseButtonDown;

	POINT m_pMousePoint;
	Ogre::Vector2 m_pCurMousePos;
	Ogre::Vector2 m_pPrevMousePos;
	float m_pMouseSensitivity;
};

