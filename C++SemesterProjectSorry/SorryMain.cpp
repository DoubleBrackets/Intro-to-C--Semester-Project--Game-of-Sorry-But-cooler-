#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "GameWindow.h"
#include "ResourceManager.h"
#include "GameObject.h"

#define TICK_RATE 50;
int main()
{
    int width = 480;
    int height = 250;
    const double tick_rate = TICK_RATE;

    GameWindow gameWindow(width, height);
    ResourceManager resourceManager("e");
    GameObject board("game_board");
    board.SetTexture("board_tex",1);
    board.InitializeObject();

    PlayerPiece* testplayer = new PlayerPiece(0,"testplayer");
    (*testplayer).SetPosition(sf::Vector2f(65 * 2.5, 159 * 2.5));
    //testplayer.velocity = sf::Vector2f(40,0);
    (*testplayer).InitializeObject();
    //Main game loop
    long prevStartTime = GameObject::ConvertToMilli(std::chrono::high_resolution_clock::now());
    while (gameWindow.window.isOpen())
    {       
        //Gets current time
        long updateStartTime = GameObject::ConvertToMilli(std::chrono::high_resolution_clock::now());
        gameWindow.PollEvents();

        //Loops through gameobjects and updates them
        int l = GameObject::spriteList.size();
        for (int x = 0; x < l; x++)
        {
            GameObject* g = GameObject::spriteList[x];
            g->UpdateGameObject(updateStartTime,updateStartTime - prevStartTime);
        }
        gameWindow.Render();
        //saves time to calculate frame length
        prevStartTime = updateStartTime;
        std::this_thread::sleep_for(std::chrono::milliseconds((long)(1 / tick_rate * 1000)));
    }

    return 0;
}