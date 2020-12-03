#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "GameWindow.h"
#include "ResourceManager.h"
#include "GameObject.h"

int main()
{
    int width = 1200;
    int height = 540;


    GameWindow gameWindow(width, height);
    ResourceManager resourceManager("e");
    GameObject board("game_board","board",0,height-400,4);
    //Main game loop
    while (gameWindow.window.isOpen())
    {
        gameWindow.PollEvents();

        gameWindow.Render();
    }

    return 0;
}