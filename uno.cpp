#include <iostream>
#include <vector>

using namespace std;

enum vars {
    PLAYER = 0,
    BOT = 1,
    SKIP = 10,
    REVERSE = 11,
    PLUS2 = 12,
    WILD = 13,
    WILD4 = 14
};

struct card{
    int color;
    int num;
};

struct agent{
    vector<card> hand;
    int type;
};

vector<card> makeDeck();
vector<agent> makeAgents(int, int);
void distributeCards(vector<card>& deck, vector<agent>& agents);

int main(int argc, char* argv[]){

    //Ensure correct usage
    /*if(argc != 3){
        cout << "Incorrect usage! Proper usage: ./uno.out <number of players> <number of bots>" << endl;
        return -1;
    }*/

    //int numPlayers = stoi(argv[1]);
    //int numBots = stoi(argv[2]);

    int numPlayers = 2;
    int numBots = 1;

    if(numPlayers + numBots > 15){
        cout << "Error: Too many agents to play the game. Max of 15." << endl;
        return -2;
    }

    //add players and bots to agent vector
    vector<agent> agents = makeAgents(numPlayers, numBots);

    //Establish deck
    vector<card> deck = makeDeck();

    //Distribute cards
    distributeCards(deck, agents);

    for(int i=0; i<7; i++){
        cout << agents[0].hand[i].num << endl;
    }
}

void distributeCards(vector<card>& deck, vector<agent>& agents){
    //deal 7 cards to every agent
    for(int i=0; i<agents.size(); i++){
        while(agents[i].hand.size() < 7){
            int tmp = rand() % deck.size(); //choose random card from deck
            agents[i].hand.push_back(deck[tmp]); //put card into agent's hand
            deck.erase(deck.begin() + tmp); //erase card from deck
        }
    }
}

vector<agent> makeAgents(int numPlayers, int numBots){

    vector<agent> agents;

    for(int i=0; i<numPlayers + numBots; i++){

        //add players
        agent tmp;
        tmp.type = PLAYER;

        //add bots
        if(i >= numPlayers)
            tmp.type = BOT;

        agents.push_back(tmp);
    }

    return agents;
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
            deck.push_back(tmp);
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
            deck.push_back(tmp);
        }
    }
    
    return deck;
}