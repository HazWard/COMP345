#include <iostream>
#include <string>
#include <random>
using namespace std;

//#include "cards.h"

const int INFANTRY = 0;
const int CAVALRY = 1;
const int ARTILLERY = 2;

class Card {
    /*
    */
    //Consider using an enum instead of the constants
    friend std::ostream& operator<<(std::ostream&, const Card&);
    public:
        int type;
        Card(int t, Card *nextCard);
        Card *next;
        void display(void);
};

Card::Card(int t, Card *nextCard){
    //TODO: Initialize the object and determine its type
    type = t;
    next = nextCard;
}

std::ostream& operator<< (std::ostream &strm, const Card &card) {
    switch(card.type) {
        case INFANTRY : return strm << "Infantry Card";
        case CAVALRY : return strm << "Cavalry Card";
        case ARTILLERY : return strm << "Artillery Card";
        default : return strm << "Card Error";
    }
}

class Deck{
    /*
    Implemented as a LinkedList but could also be done as a Vector (C++ equivalent of Java's ArrayList)
    */
    int numberOfCards;
    protected:
        Card *top;
    public:
        Deck(int numberOfCountries);
        ~Deck(void);
        Card* draw(void);
        void display(void);
};

Deck::Deck(int numberOfCountries){
    /**
        Constructor for the Deck class. TODO: talk about how this is a linked list, Initializes each
        card in the deck, making each type balanced, based on the modulo of 3 on the total number of cards.
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

    top = NULL;
    for(int i = 0; i < numberOfCards; i++){
        while(true){
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

Deck::~Deck(void){
    Card *temp = top;
    while(temp != NULL){
        temp = temp->next;
        delete top;
        top = temp;
    }
    temp = NULL;
}

Card* Deck::draw(void){
    Card *temp = top;
    top = top->next;
    int out = temp->type;
    return temp;
}

void Deck::display(void){
    int counter = 1;
    Card *temp = top;
    while(temp != NULL){
        cout << "Card number: " << counter << " Type: " << *temp << endl;
        temp = temp->next;
        counter++;
    }
    temp = NULL;
}

class Hand{
    /*
    */
    int inf, cav, art;
    public:
        bool exchange(int t);
        void draw(Card *c);
        void display(void);
};

bool Hand::exchange(int t){
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
    switch(c->type) {
        case INFANTRY : inf++; break;
        case CAVALRY : cav++; break;
        case ARTILLERY : art++; break;
        default : break;
    }
}

void Hand::display(void){
    cout << "This hand contains " << inf << " infantry cards, " << cav << " cavalry cards, and "
    << art << " artillery cards." << endl;
}

int main(){
    Deck *d = new Deck(9);
    d->display();
    Hand *h = new Hand();
    h->draw(d->draw());
    h->display();
    h->draw(d->draw());
    h->draw(d->draw());
    h->display();
    cout << (h->exchange(INFANTRY) == 1 ? "True" : "False") << " " << (h->exchange(INFANTRY) == 1 ? "True" : "False") << endl;
    return 0;
}