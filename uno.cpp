#include <iostream>
#include <vector>

enum cards {
    SKIP = 10,
    REVERSE = 11,
    PLUS2 = 12,
    WILD = 13,
    WILD4 = 14
};

using namespace std;

struct card{
    int color;
    int num;
};

struct player{
    vector<card> hand;
};

vector<card> makeDeck();

int main(int argc, char* argv[]){

    //Ensure correct usage
    if(argc != 2){
        cout << "Incorrect usage! Proper usage: ./uno.out <number of players>" << endl;
        return -1;
    }

    int numPlayers = stoi(argv[1]);

    vector<card> deck = makeDeck();

    /*for(int i=0; i<deck.size(); i++){
        cout << deck[i].color << "\t" << deck[i].num << endl;
    }*/

}

vector<card> makeDeck(){

    vector<card> deck;

    //Add first half of deck
    for(int i=0; i<4; i++){
        for(int j=0; j<14; j++){

            //make card
            card tmp;
            tmp.color = i;
            tmp.num = j;

            //append it to deck
            deck.insert(deck.end(), tmp);
        }
    }

    //Add second half of deck
    for(int i=0; i<4; i++){
        for(int j=1; j<15; j++){

            //make card
            card tmp;
            tmp.color = i;
            tmp.num = j;

            //append it to deck
            deck.insert(deck.end(), tmp);
        }
    }
    
    return deck;
}