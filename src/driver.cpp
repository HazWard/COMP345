#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "../include/map.h"
#include "../include/player.h"
#include "../include/dice.h"
#include "../include/cards.h"

int main() 
{
    std::cout << "=============== PART 4 (PLAYER) - Driver ===============" << std::endl;
    Hand* testHand = new Hand();
    Node* testArr[1];
    testArr[0] = new Node(Country("Quebec", 1));
    std::vector<Node*> testContries(std::begin(testArr), std::end(testArr));
    Player* user = new Player(*testHand, testContries);
    
    std::cout << std::endl << "==== Player's hand ===="<< std::endl;
    user->hand->display();

    std::cout << std::endl << "==== Dice Rolling ===="<< std::endl;
    std::cout << "Roll of 1 die: " << user->roll(1) << std::endl;
    std::cout << "Roll of 2 die: " << user->roll(2) << std::endl;
    std::cout << "Roll of 3 die: " << user->roll(3) << std::endl;
    std::cout << std::endl;

    std::cout << std::endl << "==== Game Phases ===="<< std::endl;
    user->attack();
    user->fortify();
    user->reinforce();
    std::cout << std::endl;

    std::cout << std::endl << "==== Player's countries ===="<< std::endl;
    for(std::size_t i = 0; i < user->countries.size(); i++)
    {
        Country c =  user->countries[i];
        std::cout << c << std::endl;
    }
    return 0;
}
