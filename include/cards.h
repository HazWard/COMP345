#pragma once

class Card {
    /**
        Card class is implemented as a node in a linked list. Implementation is barebones, with
        a display method for debugging and a single constructor.
    */
    // Use following line if you want to access private members with << operator 
    // friend std::ostream& operator<<(std::ostream&, const Card&);
    public:
        // Constants used to define card types, used in lieu of an enum
        static const int INFANTRY = 0;
        static const int CAVALRY = 1;
        static const int ARTILLERY = 2;
        
        int type;
        Card();
        Card(int t, Card *nextCard);
        Card *next;
};

class Deck{
    /*
    The deck class is implemented as a Linked List for simplicity and efficiency.
    Could also be done as a Vector (C++ equivalent of Java's ArrayList) for robustness.
    Has a display method for debugging and draw method that is to be passed as a param to
    the draw method of the hand class.
    */
    int numberOfCards;
    protected:
        Card *top;
    public:
        Deck(int numberOfCountries);
        Deck();
//        ~Deck();
        Card* draw();
        void display();
        int getNumberOfCards();
};

class Hand{
    /*
        Hand class implemented in the simplest way possible, simply a set of three counters
        that tracks the number of cards of each type. Display method used for debugging and
        draw method used to add cards. Exchange will check if you have 3 cards of the desired type
        and return true if the exchange operation was successful, false otherwise.
    */
    int inf, cav, art;
    public:
        bool exchange(int t);
        void draw(Card *c);
        void display(void);
};