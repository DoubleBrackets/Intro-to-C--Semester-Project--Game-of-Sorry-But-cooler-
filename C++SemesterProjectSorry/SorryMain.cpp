#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "GameWindow.h"
#include "ResourceManager.h"
#include "GameObject.h"

int main()
{
    int width = 480;
    int height = 200;
    const double tick_rate = 25;

    GameWindow gameWindow(width, height);
    ResourceManager resourceManager("e");
    GameObject board("game_board");
    board.SetTexture("board_tex",1);
    board.InitializeToRenderList();

    GameObject testplayer("testplayer");
    testplayer.SetTexture("redpiece_tex",1);
    testplayer.AddAnimation("walk",4, 0.2, sf::Vector2f(16, 16), 2, 2, sf::Vector2f(0, 0));
    testplayer.SetPosition(sf::Vector2f(64*3 ,100*3));
    testplayer.AddAnimation("idle", 1, 100, sf::Vector2f(16, 16), 1, 1, sf::Vector2f(0, 0));
    testplayer.StartAnimation("walk");
    testplayer.InitializeToRenderList();
    //Main game loop
    while (gameWindow.window.isOpen())
    {
        gameWindow.PollEvents();
        //Gets current time

        long updateStartTime = GameObject::ConvertToMilli(std::chrono::high_resolution_clock::now());
        //Loops through gameobjects and updates them
        int l = GameObject::spriteList.size();
        for (int x = 0; x < l; x++)
        {
            GameObject* g = &GameObject::spriteList[x];
            (*g).UpdateGameObject(updateStartTime);
        }
        gameWindow.Render();
        std::this_thread::sleep_for(std::chrono::milliseconds((long)(1 / tick_rate * 1000)));
    }

    return 0;
}