#include "../include/cards.h"

#include <iostream>
#include <random>
using namespace std;



//Constants used to define card types, used in lieu of an enum
static const int INFANTRY = 0;
static const int CAVALRY = 1;
static const int ARTILLERY = 2;

//Default constructor
Card::Card() : type(3), next(NULL) {}

Card::Card(int t, Card *nextCard){
    //Constructor for Card class
    type = t;
    next = nextCard;
}

std::ostream& operator<< (std::ostream &strm, const Card &card) {
    //Overwritten implementation of the << operator, used as the C++ equivalent of toString
    switch(card.type) {
        case INFANTRY : return strm << "Infantry Card";
        case CAVALRY : return strm << "Cavalry Card";
        case ARTILLERY : return strm << "Artillery Card";
        default : return strm << "Card Error";
    }
}

Deck::Deck(int numberOfCountries){
    /**
        Constructor for the Deck class. Number of cards is equal to the number of countries passed as
        argument. Determines the number for each type of card by finding modulo 3, balancing in favour
        of lower tier cards. Then iterates the number of cards and creates a random deck based on the
        correct number of cards of each type.
    */
    this->numberOfCards = numberOfCountries;

    //Using this code to generate random numbers, used for determining the type of each card
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 2); // define the range

    int cardType[3];
    cardType[0] = (numberOfCards % 3 >= 1 ? numberOfCards / 3 + 1 : numberOfCards / 3);
    cardType[1] = (numberOfCards % 3 == 2 ? numberOfCards / 3 + 1 : numberOfCards / 3);
    cardType[2] = numberOfCards / 3;

    //setting the top card to null, so that the first card created will have its next point to null
    top = NULL;
    for(int i = 0; i < numberOfCards; i++){
        while(true){ //will loop until generates a random value equal to a card type that still needs to be created
            int r = distr(eng);
            //int r = rand() % static_cast<int>(3); //non distributed random generator
            if(cardType[r] > 0){
                top = new Card(r, top);
                cardType[r]--;
                break;
            }
        }
    }
}

//Default constructor
Deck::Deck() : numberOfCards(0), top(NULL) {}

int Deck::getNumberOfCards() { return numberOfCards; }

Deck::~Deck(){
    /**
        Destructor for the Deck class, iterates through the linked list and destroys every node.
    */
    Card *temp = top;
    while(temp != NULL){
        temp = temp->next;
        delete top;
        top = temp;
    }
    temp = NULL;
}

Card* Deck::draw(){
    /**
        Draw method removes the first node from the linked list and returns the pointer to that node.
        Used in conjuction with the draw method of the Hand class, with the return value of this method
        being passed as the argument to the draw method of Hand.
    */
    if(this->numberOfCards > 0){
        Card *temp = top;
        top = top->next;
        return temp;
    }
    return NULL;
}

void Deck::display(){
    /**
        Used for debugging. Iterates through the linked list and prints each card.
     */
    int counter = 1;
    int infCounter = 0;
    int cavCounter = 0;
    int artCounter = 0;

    Card *temp = top;
    while(temp != NULL){
        cout << "Card number: " << counter << " Type: " << *temp << endl;
        switch(temp->type) {
            case INFANTRY : infCounter++; break;
            case CAVALRY : cavCounter++; break;
            case ARTILLERY : artCounter++; break;
            default : break;
        }
        temp = temp->next;
        counter++;
    }

    cout << "This deck contains " << infCounter << " infantry cards, " << cavCounter << " cavalry cards, and "
    << artCounter << " artillery cards." << endl;

    //Setting temp pointer to NULL to avoid weird stuff
    temp = NULL;
}

bool Hand::exchange(int t){
    /**
        Used when a player wants to exchange 3 cards of a certain type for additional units.
        Returns true if successful, false otherwise.
    */
    switch(t) {
        case INFANTRY : 
            if(inf >= 3){
                inf = inf - 3;
                return true;
            }
            return false;
        case CAVALRY : 
            if(cav >= 3){
                cav = cav - 3;
                return true;
            }
            return false;
        case ARTILLERY :
            if(art >= 3){
                art = art - 3;
                return true;
            }
            return false;
        default : return false;
    }
}

void Hand::draw(Card *c){
    /**
        Draw method of Hand is passed a pointer to a card object and increments the counter
        corresponding to that card type. Deletes the object when done.
    */
    switch(c->type) {
        case INFANTRY : inf++; break;
        case CAVALRY : cav++; break;
        case ARTILLERY : art++; break;
        default : break;
    }
    delete c;
}

void Hand::display(void){
    cout << "This hand contains " << inf << " infantry cards, " << cav << " cavalry cards, and "
    << art << " artillery cards." << endl;
}

int Hand::getTotalCards()
{
    return inf + art + cav;
}
