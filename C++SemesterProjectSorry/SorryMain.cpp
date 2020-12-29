#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "GameWindow.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "GameManager.h"

#define TICK_RATE 50;

void InitBaseSprites();
void CreateExtraAnims();

int width = 480;
int height = 300;
int main()
{
    srand(time(NULL));

    const double tick_rate = TICK_RATE;
    GameWindow::res_Scale = 2;
    //Creates window + resource manager objects
    GameWindow gameWindow(width, height);
    ResourceManager resourceManager("e");

    //Creates game manager object and initializes it
    GameManager gameManager;
    //graphics
    InitBaseSprites();
    ConfirmButton::CreateIndicators();
    gameManager.InitGame();
    //Saves initial start time of loop
    long prevStartTime = GameObject::ConvertToMilli(std::chrono::high_resolution_clock::now());
    //Game loop started
    CreateExtraAnims();

    //testing
    GameManager::instance->MovePieceOutOfHome(0);
    GameManager::instance->MovePieceOutOfHome(1);
    while (gameWindow.window.isOpen())
    {   
        long updateStartTime = GameObject::ConvertToMilli(std::chrono::high_resolution_clock::now());
        //Game manager tries to start a new turn if possible
        if (!gameManager.gameEnded)
        {
            if (!gameManager.StartNewTurn())
            {
                std::string wintext = (gameManager.currentTurn == 0) ? "Red" : "Blue";
                wintext += " Team has Won";
                gameManager.turntext->SetText(wintext);
            }
        }

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
        long updateDuration = GameObject::ConvertToMilli(std::chrono::high_resolution_clock::now()) - updateStartTime;
        prevStartTime = updateStartTime;
        std::this_thread::sleep_for(std::chrono::milliseconds((long)(1 / tick_rate * 1000)-updateDuration));
    }
    
    return 0;
}



void InitBaseSprites()
{
    double scale = GameWindow::res_Scale;
    //Creates board graphic
    std::shared_ptr<GameObject> board = std::make_shared<GameObject>("game_board");
    board->SetTexture("board_tex", 1);
    board->InitializeObject(board);
    board.reset();
    //create homes
    std::shared_ptr<TeamHome> team0home = std::make_shared<TeamHome>("team0home", 0);
    team0home->InitializeObject(team0home);
    GameManager::instance->home0sprite = team0home;
    std::shared_ptr<TeamHome> team1home = std::make_shared<TeamHome>("team1home", 1);
    team1home->InitializeObject(team1home);
    GameManager::instance->home1sprite = team1home;
    //Confirm buttons
    std::shared_ptr<ConfirmButton> confirmbutton = std::make_shared<ConfirmButton>("confirmbutton");
    confirmbutton->InitializeObject(confirmbutton);
    //swap buttons
    std::shared_ptr<SwapButton> swapbutton = std::make_shared<SwapButton>("swapbutton");
    swapbutton->InitializeObject(swapbutton);
    //text
    std::shared_ptr<TextObject> currentcardtext = std::make_shared<TextObject>("currentcardtext");
    currentcardtext->SetText("current card description");
    currentcardtext->SetWraparound(25);
    currentcardtext->SetPosition(sf::Vector2f(10 * scale, height*scale - 75*scale));
    currentcardtext->InitializeObject(currentcardtext);
    GameManager::instance->currentcardtext = currentcardtext;

    std::shared_ptr<TextObject> storedcard0text = std::make_shared<TextObject>("storedcard0text");
    storedcard0text->SetText("No Stored Card");
    storedcard0text->SetWraparound(25);
    storedcard0text->SetPosition(sf::Vector2f(135 * scale, height * scale - 75 * scale));
    storedcard0text->InitializeObject(storedcard0text);
    GameManager::instance->storedcard0text = storedcard0text;

    std::shared_ptr<TextObject> storedcard1text = std::make_shared<TextObject>("storedcard1text");
    storedcard1text->SetText("No Stored Card");
    storedcard1text->SetWraparound(25);
    storedcard1text->SetPosition(sf::Vector2f(260 * scale, height * scale - 75 * scale));
    storedcard1text->InitializeObject(storedcard1text);
    GameManager::instance->storedcard1text = storedcard1text;

    std::shared_ptr<TextObject> turntext = std::make_shared<TextObject>("turntext");
    turntext->SetScale(1);
    turntext->SetText("CURRENT TURN");
    turntext->SetPosition(sf::Vector2f(width*scale/2-25*scale, 20*scale));
    turntext->InitializeObject(turntext);
    GameManager::instance->turntext = turntext;

    
}

void CreateExtraAnims()
{
    double scale = GameWindow::res_Scale;
    //extra animations
    //finish counter visual
    std::shared_ptr<GameObject> team0finish = std::make_shared<GameObject>("team0finish");
    team0finish->SetTexture("redhome_tex", 1);
    team0finish->AddAnimation("4", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 0), true);
    team0finish->AddAnimation("3", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 32), true);
    team0finish->AddAnimation("2", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 64), true);
    team0finish-> AddAnimation("1", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 96), true);
    team0finish->AddAnimation("0", 1, 1, sf::Vector2f(16, 32), 1, 1, sf::Vector2f(0, 128), true);
    team0finish->StartAnimation("0");
    team0finish->SetPosition(sf::Vector2f(7 * 16 * scale, (132 - 16 * 6) * scale));
    team0finish->InitializeObject(team0finish);

    GameManager::instance->finish0sprite = team0finish;

    std::shared_ptr<GameObject> team1finish = std::make_shared<GameObject>("team1finish");
    team1finish->SetTexture("bluehome_tex", 1);
    team1finish->AddAnimation("4", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 0), true);
    team1finish->AddAnimation("3", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 32), true);
    team1finish->AddAnimation("2", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 64), true);
    team1finish->AddAnimation("1", 6, 0.25, sf::Vector2f(16, 32), 6, 1, sf::Vector2f(0, 96), true);
    team1finish->AddAnimation("0", 1, 1, sf::Vector2f(16, 32), 1, 1, sf::Vector2f(0, 128), true);
    team1finish->StartAnimation("0");
    team1finish->SetPosition(sf::Vector2f(22 * 16 * scale, (132 - 16 * 6) * scale));
    team1finish->InitializeObject(team1finish);
    GameManager::instance->finish1sprite = team1finish;
 //summon graphics
    std::shared_ptr<GameObject> redsummon = std::make_shared<GameObject>("redsummonanim");
    redsummon->SetTexture("summonpiecered_tex", 1);
    redsummon->AddAnimation("default", 10, 0.05, sf::Vector2f(16, 190), 10, 1, sf::Vector2f(0, 0), false);
    redsummon->SetPosition(sf::Vector2f(9 * 16 * scale, -12 * scale));
    redsummon->StartAnimation("default");
    redsummon->SetAnimationFrame(9);
    redsummon->StopAnimation();
    redsummon->InitializeObject(redsummon);

    std::shared_ptr<GameObject> bluesummon = std::make_shared<GameObject>("bluesummonanim");
    bluesummon->SetTexture("summonpieceblue_tex", 1);
    bluesummon->AddAnimation("default", 10, 0.05, sf::Vector2f(16, 190), 10, 1, sf::Vector2f(0, 0), false);
    bluesummon->SetPosition(sf::Vector2f(24 * 16 * scale, -12 * scale));
    bluesummon->StartAnimation("default");
    bluesummon->SetAnimationFrame(9);
    bluesummon->StopAnimation();
    bluesummon->InitializeObject(bluesummon);

//end graphics
    std::shared_ptr<GameObject> redfinish = std::make_shared<GameObject>("redfinishanim");
    redfinish->SetTexture("summonpiecered_tex", 1);
    redfinish->AddAnimation("default", 10, 0.05, sf::Vector2f(16, 190), 10, 1, sf::Vector2f(0, 0), false);
    redfinish->SetPosition(sf::Vector2f(7 * 16 * scale, -12 * scale-6*16*scale));
    redfinish->StartAnimation("default");
    redfinish->SetAnimationFrame(9);
    redfinish->StopAnimation();
    redfinish->InitializeObject(redfinish);

    std::shared_ptr<GameObject> bluefinish = std::make_shared<GameObject>("bluefinishanim");
    bluefinish->SetTexture("summonpieceblue_tex", 1);
    bluefinish->AddAnimation("default", 10, 0.05, sf::Vector2f(16, 190), 10, 1, sf::Vector2f(0, 0), false);
    bluefinish->SetPosition(sf::Vector2f(22 * 16 * scale, -12 * scale-6*16*scale));
    bluefinish->StartAnimation("default");
    bluefinish->SetAnimationFrame(9);
    bluefinish->StopAnimation();
    bluefinish->InitializeObject(bluefinish);



}