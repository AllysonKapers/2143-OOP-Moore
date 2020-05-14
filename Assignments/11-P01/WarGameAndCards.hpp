#include "termio.h"
#include <algorithm> // std::random_shuffle
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string spade = "♠";
const string diamond = "♦";
const string heart = "♥";
const string club = "♣";

const string suits[4] = {"♠", "♦", "♣", "♥"};

// Black background        blue ,  red , blue , red
const string colors2[4] = {"&60", "&20", "&60", "&20"};

// Colored background      blue  , red  , blue , red
const string colors[4] = {"&16", "&12", "&16", "&12"};

// Card labels (could be "Iron Man" or "Charmander" or "Elf" ... anything)
const string ranks[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};


class Card {
private:
    int suitNum;     // value 0-3 : num index of suit
    int rank;        // 0-13      : num value of rank
    int number;      // 1-52      : unique value determines card
    int value;       // 1-14      : numeric value used for comparison
    string suitChar; // "♠", "♦", "♣", "♥"
    string rankChar; // Ace, 1 ,2 ,3 ... Q, K
    string color;    // Spade=blue, Diamond=red, etc.

public:
    friend ostream;

    Card(int);
    string Repr();
    bool operator<(const Card &);
    bool operator>(const Card &);
    bool operator==(const Card &);
    bool operator!=(const Card &);
    bool operator()(const Card &);

    int getValue(){ 
		return value; 
	};
};

/**
     * Public : Card
     *
     * Description:
     *      Represents a single card in a deck of cards to include a
     *      value along with rank and suit. We are assuming a standard
     *      card type in a deck of playing cards.
     *
     * Params:
     *      int :  value from 0 - 51 that represents a card in a deck.
     *
     * Returns:
     *      None
     */
Card::Card(int num) {
    number = num;
    suitNum = number / 13;
    suitChar = suits[suitNum];
    color = colors[suitNum];
    rank = number % 13;
    rankChar = ranks[rank];
}

/**
     * Public : Repr
     *
     * Description:
     *      Returns a string representation of the card class with
     *      colors embedded.
     *
     * Params:
     *      None
     *
     * Returns:
     *      [string] - representation of card
     */
string Card::Repr() {
    string s = "";
    s += color + "┌────┐&00 \n";
    s += color + "│";
    if (rank != 9) {
        s += color + " ";
    }
    s += color + rankChar + " " + suitChar + "│&00 \n";
    s += color + "└────┘&00 ";
    return s;
}

/**
 * Public : operator <<
 *
 * Description:
 *      Overload ostream and send the string representation "Repr"
 *      of the card.
 *
 *      We don't need the scope resolution operator (Card::) since
 *      this is a "friend" of card.
 *
 * Params:
 *      [ostream&] : os
 *      [Card]     : obj
 *
 * Returns:
 *      [ostream&]
 */
ostream &operator<<(ostream &os, Card obj) {

    os << obj.Repr();

    return os;
}

bool Card::operator<(const Card &rhs) {
    return this->rank < rhs.rank;
}

bool Card::operator>(const Card &rhs) {
    return this->rank > rhs.rank;
}

bool Card::operator==(const Card &rhs) {
    return this->rank == rhs.rank;
}

bool Card::operator!=(const Card &rhs) {
    return this->rank != rhs.rank;
}

bool Card::operator()(const Card &rhs) {
    return (this->rank < rhs.rank);
};

//Card Container Class

class CardContainer {
protected:
	Term::IO io;
	vector<Card *> Cards;
	int RandomIndex();
	struct CardCompare {
		bool operator()(Card *l, Card *r) {
			return *l < *r;
		}
	};

public:
	CardContainer(int);
	void Add(Card *);
	bool isEmpty();
	void Order();
	Card *Remove();
	void Reset();
	void Shuffle();
	int Size();
};

CardContainer::CardContainer(int numCards) {
	for (int i = 0; i < numCards; i++) {
		Cards.push_back(new Card(i));
	}
}

void CardContainer::Add(Card *card) {
	// check if card already in container?
	// allow go over 52 ??

	Cards.push_back(card);
	//currentCount = Cards.size();
}

bool CardContainer::isEmpty() {
	return Cards.empty();
}

void CardContainer::Order() {
	sort(Cards.begin(), Cards.end(), CardCompare());
}

Card *CardContainer::Remove() {
	Card *temp = Cards.back();
	Cards.pop_back();
	return temp;
}

void CardContainer::Reset() {
	for (int i = 0; i < Cards.size() - 1; i++) {
		delete Cards[i];
		Cards[i] = new Card(i);
	}
}

void CardContainer::Shuffle() {
	for (int i = 0; i < Cards.size() - 1; i++) {
		int j = i + rand() % (Cards.size() - i);
		swap(Cards[i], Cards[j]);
	}
}

int CardContainer::Size() {
	return Cards.size();
}



class Deck : public CardContainer {
protected:
	int deckSize;

public:
	Deck(int);
	Card *Draw();
	void Print(int, bool);
};

Deck::Deck(int size) : CardContainer(size) {
	deckSize = size;
}

Card *Deck::Draw() {
	Card *temp = Cards.back();
	Cards.pop_back();
	return temp;
}

void Deck::Print(int columns = 1, bool clear = false) {
	int i = 0;
	vector<string> cards;

	if (clear) {
		io << Term::clear;
	}

	for (auto c : Cards) {
		cards.push_back(c->Repr());

		i++;
		if (i == columns) {
			io << Term::fuse(cards) << "\n";
			i = 0;
			cards.clear();
		}
	}

	// any cards left in the vector should be
	// printed out.
	if (cards.size() > 0) {
		io << Term::fuse(cards) << "\n";
	}
}



class Hand
{
public:
	Hand();
	//Returns a full deck of cards
	static Hand fullDeck();
	Card drawTop();
	Card drawBottom();
	//Returns the card from the top or bottom of the hand, removes that card
	void placeTop(Card c);
	void placeBottom(Card c);
	//Places the card on the top or bottom of the hand
	int Size();
	//Returns how many cards are in the hand
	bool isEmpty();
private:
	vector<Card> cards;
};

Hand::Hand()
{
}

Hand Hand::fullDeck()
{
	//Create a Hand with a full deck of cards
	Hand deck;

	const int NUM_SUITS = 4;
	const int NUM_FACES = 13;
	/*Suit suit[NUM_SUITS] = { clubs, diamonds, hearts, spades };
	Value face[NUM_FACES] = { ace, deuce, trey, four, five, six, seven, eight, nine, ten, jack, queen, king };*/

	for (int i = 0; i < NUM_SUITS; i++)
	{
		for (int j = 0; j < NUM_FACES; j++)
		{
			//deck.placeBottom(Card(face[j], suit[i]));
		}
	}

	return deck;
}

/*void CardContainer::Shuffle()
{
	srand(time(0) + Size());
	//Select a new random location for each card
	for (int i = 0; i < Size(); i++)
	{
		int newIndex = rand() % Size();
		Cards.insert(Cards.begin() + newIndex, Cards.at(i));
		Cards.erase(Cards.begin() + i);
	}
}*/


Card Hand::drawTop()
{
	Card drawnCard = cards.at(0);
	cards.erase(cards.begin());
	return drawnCard;
}

Card Hand::drawBottom()
{
	//Remove and return the last card
	Card card = cards.back();
	cards.erase(cards.end());
	return card;
}

void Hand::placeTop(Card c)
{
	//Insert the card at the beginning of the deck
	cards.insert(cards.begin(), c);
}

void Hand::placeBottom(Card c)
{
	cards.push_back(c);
}

bool Hand::isEmpty()
{
	//return cards.isEmpty();
	return Hand::isEmpty();
}

int Hand::Size()
{
	//return cards.Size();
	return Hand::Size();
}











class WarGame
{
public:
	void play();
//these functions are private because they control game play and manipulation 
//by a player would cause the game to work incorrectly. 
private:
	WarGame(); 
	static void Battle();
	static int War();
	void printCards();
	static Deck D;
	static Hand player1, player2;
	static int Rounds;
	static const int NumOfRounds = 20; 


void play()
{
		int Rounds = 1;
		bool skipToEnd = false;

		
		player1 = Hand();
		player2 = Hand();
		Deal();

		//Compare each players hands until one is empty
		if (!player1.isEmpty() && !player2.isEmpty())
		{
			Battle();

			if (Rounds++ > NumOfRounds)
			{
				cout << "This game of War appears to be infinite." << endl;
			}
		}
        //ends game if one player has an empty hand
		else
		{
			cout << "End of game" << endl;
		}

		//Displays player who wins
		if(Rounds > NumOfRounds)
		{
			cout << "The war ends in a draw." << endl << endl;
		}
		else
		{   //determines winner of the game based in who has an empty hand
			cout << "Congratulations, ";
			if (player1.isEmpty())
			{
				cout << "Player 2 is the winner!" << endl << endl;
			}
			else
			{
				cout << "Player 1 is the winner" << endl << endl;
			}
		}

		//Game terminates
		exit(0);
}

void Deal(){
             
        D.Shuffle();    // Calls Decks shuffle method 
           
        while(!D.isEmpty()){ 

            // While the deck is not empty deal a card
            // to each player

            player1.drawTop();
            player2.drawTop();    

        }   
           
}


void Battle()
{
	Battle();
	Card pOneCard = player1.drawTop();
	Card pTwoCard = player2.drawTop();
    //checks if cards are the same then players enter war
	if (pOneCard.getValue() == pTwoCard.getValue())
	{   //assigns winner value to 
		int winner = War();

		//Give the cards to the winner of the round
		if (winner == 1)
		{
			player1.placeBottom(pOneCard);
			player1.placeBottom(pTwoCard);

			cout << "Player 1";
		}
		else if (winner == 2)
		{
			player2.placeBottom(pOneCard);
			player2.placeBottom(pTwoCard);

			cout << "Player 2";
		}
		cout << " won the battle!" << endl;
	}
	else if (pOneCard.getValue() > pTwoCard.getValue())
	{
		//Player 1 wins if card value is greater than player 2
		player1.placeBottom(pOneCard);
		player1.placeBottom(pTwoCard);
		cout << "Player 1 won this round." << endl;
	}
	else if (pOneCard.getValue() < pTwoCard.getValue())
	{
		//Player 2 wins if card value is greater than player 1
		player2.placeBottom(pOneCard);
		player2.placeBottom(pTwoCard);
		cout << "Player 2 won this round." << endl;
	}
}



//This functions "War" is the main driver for the War game. It goes past just 
//comparing cards and implements more specific game play. With tweaks the rest 
//of the functions could be used in more games that require card comparisons. 
int War()
{
	Hand warPile;
    //uses int variable to hold winner value, 
    //returns 1 when player1 wins and 2 when player2 wins
	int winner = 0;
    //loops until the the cards aren't the same
	while (player1.drawTop() == player2.drawTop())
	{
		cout << "WAR!!" << endl;

		//Handles if a player runs out of cards, to avoid error 
		if (player1.isEmpty())
		{
			cout << "Player 1 is out of cards." << endl;
			winner = 2;
		}
		else if (player2.isEmpty())
		{
			cout << "Player 2 is out of cards." << endl;
			winner = 1;
		}

		//Puts the extra three cards down to satisfy game requirements
		for (int i = 0; i < 3; i++)
		{
			//Save one card for the comparison
			if (player1.Size() > 1) warPile.placeBottom(player1.drawTop());
			if (player2.Size() > 1) warPile.placeBottom(player2.drawTop());
		}

		//Comparing cards 
		Card p1Card = player1.drawTop();
		Card p2Card = player2.drawTop();
		warPile.placeBottom(p1Card);
		warPile.placeBottom(p2Card);
	}

	//Give the cards to the winner
	if (player1.drawTop().getValue() > player2.drawTop().getValue() || winner == 1)
	{
		int pile_size = warPile.Size();
		for (int i = 0; i < pile_size; i++)
			player1.placeBottom(warPile.drawTop());

		winner = 1;
	}
	else if (player2.drawTop().getValue() > player1.drawTop().getValue() || winner == 2)
	{
		int pile_size = warPile.Size();
		for (int i = 0; i < pile_size; i++)
			player2.placeBottom(warPile.drawTop());

		winner = 2;
	}

	return winner;
}
};