#include "WindowManagement/EditorWindow.h"
#include <filesystem>
#include <string>

EditorWindow::EditorWindow(RenderEngine* renderEngine) :
	m_bIsClosed(false),
	m_bIsFrozen(false),
	m_pCurrentSelection(nullptr),
	m_pRenderEngine(renderEngine)
{
	InitWindow();
}

EditorWindow::~EditorWindow()
{

}

void EditorWindow::ProcessSDLInput() 
{
	SDL_Event event;
	bool processed;
	SDL_PumpEvents();
	std::vector<SDL_Event> notProcessedEvents;
	while (SDL_PollEvent(&event))
	{
		processed = false;
		if (event.window.windowID == SDL_GetWindowID(m_SDLWindow))
		{
			//w->SetImguiContext();
			ImGui_ImplSDL2_ProcessEvent(&event);
			processed = true;
		}
		if (!processed) {
			notProcessedEvents.push_back(event);
		}
		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
			m_bIsClosed = true;
	}
	
	for (auto ev : notProcessedEvents) {
		SDL_PushEvent(&ev);
	}
}

void EditorWindow::InitWindow() 
{
	const char* glsl_version = "#version 130";

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	m_SDLWindow = SDL_CreateWindow("SDL Ogre Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 360, 720, window_flags);

	m_GLContext = SDL_GL_CreateContext(m_SDLWindow);
	SDL_GL_MakeCurrent(m_SDLWindow, m_GLContext);
	SDL_GL_SetSwapInterval(0.5);

	InitImGui();
}

void EditorWindow::InitImGui() 
{
	const char* glsl_version = "#version 130";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantCaptureKeyboard = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_SDLWindow, m_GLContext);
	bool f = ImGui_ImplOpenGL3_Init(glsl_version);
}

void EditorWindow::RenderGui()
{
	StartGuiUpdate();

	DisplayMenuBar();
	DisplayAllScripts();
	DisplaySelectionParameters();
	DisplayFreezeBtn();

	EndGuiUpdate();
}

void EditorWindow::StartGuiUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_SDLWindow);
	ImGui::NewFrame();
}

void EditorWindow::EndGuiUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorWindow::Update() 
{
	SDL_GL_MakeCurrent(m_SDLWindow, m_GLContext);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ProcessSDLInput();

	RenderGui();
	
	SDL_GL_SwapWindow(m_SDLWindow);
}

void EditorWindow::DisplayMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "CTRL+S")) {}
			if (ImGui::MenuItem("Load", "")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void EditorWindow::DisplaySelectionParameters()
{
	if (m_pCurrentSelection)
	{
		ImGui::Begin("Parameters");
		ImGui::Text(m_pCurrentSelection->getName().c_str());
	
		static float posVec[3] = { 0.f, 0.f, 0.f };
		posVec[0] = float(m_pCurrentSelection->getPosition().x);
		posVec[1] = float(m_pCurrentSelection->getPosition().y);
		posVec[2] = float(m_pCurrentSelection->getPosition().z);
		ImGui::InputFloat3("Position", posVec);
	
		static int rotVec[3] = { 0, 0, 0 };
		rotVec[0] = int(m_pCurrentSelection->getOrientation().getPitch().valueDegrees());
		rotVec[1] = int(m_pCurrentSelection->getOrientation().getYaw().valueDegrees());
		rotVec[2] = int(m_pCurrentSelection->getOrientation().getRoll().valueDegrees());
		ImGui::InputInt3("Rotation", rotVec);
		ImGui::End();
	
		m_pRenderEngine->GetRT()->RC_LambdaAction([&] {m_pCurrentSelection->setPosition(posVec[0], posVec[1], posVec[2]); });
		
	
		Ogre::Quaternion q = m_pCurrentSelection->getOrientation();
		int clampedX = std::clamp(rotVec[0], -180, 180);
		if (clampedX != int(m_pCurrentSelection->getOrientation().getPitch().valueDegrees()))
		{
			Ogre::Radian radX = Ogre::Radian(Ogre::Degree(clampedX));
			Ogre::Radian offsetX = radX - m_pCurrentSelection->getOrientation().getPitch();
			q.FromAngleAxis(offsetX, Ogre::Vector3::UNIT_X);
			q = q * m_pCurrentSelection->getOrientation();
			m_pRenderEngine->GetRT()->RC_LambdaAction([&] {m_pCurrentSelection->setOrientation(q); });
			return;
		}
	
		int clampedY = std::clamp(rotVec[1], -180, 180);
		if (clampedY != int(m_pCurrentSelection->getOrientation().getYaw().valueDegrees()))
		{
			Ogre::Radian radY = Ogre::Radian(Ogre::Degree(clampedY));
			Ogre::Radian offsetY = radY - m_pCurrentSelection->getOrientation().getYaw();
			q.FromAngleAxis(offsetY, Ogre::Vector3::UNIT_Y);
			q = q * m_pCurrentSelection->getOrientation();
			m_pRenderEngine->GetRT()->RC_LambdaAction([&] {m_pCurrentSelection->setOrientation(q); });
			return;
		}
	
		int clampedZ = std::clamp(rotVec[2], -180, 180);
		if (clampedZ != int(m_pCurrentSelection->getOrientation().getRoll().valueDegrees()))
		{
			Ogre::Radian radZ = Ogre::Radian(Ogre::Degree(clampedZ));
			Ogre::Radian offsetZ = radZ - m_pCurrentSelection->getOrientation().getRoll();
			q.FromAngleAxis(offsetZ, Ogre::Vector3::UNIT_Z);
			q = q * m_pCurrentSelection->getOrientation();
			m_pRenderEngine->GetRT()->RC_LambdaAction([&] {m_pCurrentSelection->setOrientation(q); });
			return;
		}
	}
}

void EditorWindow::DisplayAllScripts()
{
	ImGui::Begin("Scripts");
	std::string path = "D:\\MIPT\\Game-engines\\GameEnginesPractice\\GameEnginesPractice\\Scripts";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".lua")
		{
			std::string btnName = entry.path().filename().string();
			if (ImGui::Button(btnName.c_str()))
			{
				std::string command = "start notepad++ " + entry.path().string();
				std::system(command.c_str());
			}
		}
	}
	ImGui::End();
}

void EditorWindow::DisplayFreezeBtn()
{
	ImGui::Begin("lol");
	ImGui::Checkbox("F", &m_bIsFrozen);
	ImGui::End();
}
