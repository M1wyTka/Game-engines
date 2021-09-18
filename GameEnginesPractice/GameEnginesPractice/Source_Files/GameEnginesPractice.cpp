// GameEnginesPractice.cpp : Defines the entry point for the application.
//

#include "GameEnginesPractice.h"
#include "Game.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Game* pGame = new Game();
    pGame->Run();

    return 0;
}