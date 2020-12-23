#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "GameWindow.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "GameManager.h"

#define TICK_RATE 50;
int main()
{
    int width = 480;
    int height = 250;
    const double tick_rate = TICK_RATE;

    //Creates window + resource manager objects
    GameWindow gameWindow(width, height);
    ResourceManager resourceManager("e");
    //Creates board graphic
    std::shared_ptr<GameObject> board = std::make_shared<GameObject>("game_board");
    board->SetTexture("board_tex",1);
    board->InitializeObject(board);
    board.reset();
    //Creates game manager object and initializes it
    GameManager gameManager;
    gameManager.InitGame();

    //Saves initial start time of loop
    long prevStartTime = GameObject::ConvertToMilli(std::chrono::high_resolution_clock::now());
    //Game loop started
    while (gameWindow.window.isOpen())
    {       
        //Game manager update
        
        //Gets current time
        long updateStartTime = GameObject::ConvertToMilli(std::chrono::high_resolution_clock::now());
        gameWindow.PollEvents();

        //Loops through gameobjects and updates them
        int l = GameObject::spriteList.size();
        for (int x = 0; x < l; x++)
        {
            std::shared_ptr<GameObject> g = GameObject::spriteList[x];
            g->UpdateGameObject(updateStartTime,updateStartTime - prevStartTime);
        }
        gameWindow.Render();
        //saves time to calculate frame length
        prevStartTime = updateStartTime;
        std::this_thread::sleep_for(std::chrono::milliseconds((long)(1 / tick_rate * 1000)));
    }
    return 0;
}



