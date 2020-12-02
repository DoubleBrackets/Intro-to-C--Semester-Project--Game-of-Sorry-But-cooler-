#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "GameWindow.h"



int main()
{
    int width = 480;
    int height = 270;

    GameWindow gameWindow(width, height);

    //Main game loop
    while (gameWindow.window.isOpen())
    {

        gameWindow.PollEvents();

        gameWindow.Render();
    }

    return 0;
}