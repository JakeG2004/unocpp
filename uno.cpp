#include <iostream>
#include <vector>
#include <limits>
#include <stdlib.h> 

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
    string color;
    int num;
};

struct agent{
    vector<card> hand;
    int type;
};

//Prototypes
vector<card> makeDeck();
vector<agent> makeAgents(int, int);
void distributeCards(vector<card>&, vector<card>&, vector<agent>&);
void printHand(int, vector<agent>);
void printTopCard(vector<card>);
void printNums(vector<agent>);
void drawCard(int, vector<agent>&, vector<card>&);
void playCard(int, vector<agent>&, vector<card>&, vector<card>&);

void badInput();

int main(int argc, char* argv[]){

    bool win = false;
    srand(time(0));

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

    //Establish discard
    vector<card> discard;

    //Distribute cards
    distributeCards(deck, discard, agents);

    int currentAgent = 0;

    //Establish deck
    while(!win){
        if(agents[currentAgent].type == PLAYER){
            printTopCard(discard);

            int choice;

            //Prompt player
            cout << "Player " << currentAgent + 1 << ", take your turn" << endl;
            cout << "1) View hand\n"
            << "2) View topcard\n"
            << "3) View # of cards in other hands\n"
            << "4) Draw a card\n"
            << "5) Play a card\n";

            //Handle bad input
            while((cout << "> " && !(cin >> choice)) || choice < 1 || choice > 5)
                badInput();

            switch(choice){
                case 1:
                    printHand(currentAgent, agents);
                    break;
                case 2:
                    printTopCard(discard);
                    break;
                case 3:
                    printNums(agents);
                    break;
                case 4:
                    drawCard(currentAgent, agents, deck);
                    break;
                case 5:
                    playCard(currentAgent, agents, deck, discard);
                    break;

            
            //clear the screen
            //cout << "\x1B[2J\x1B[H";

            }

            

        } else {
            cout << "Bot" << endl;
        }


        currentAgent = (currentAgent + 1) % agents.size(); //go to next agent
    }

}

void distributeCards(vector<card>& deck, vector<card>& discard, vector<agent>& agents){
    //deal 7 cards to every agent
    for(int i=0; i<agents.size(); i++){
        //add 7 cards to each agent
        while(agents[i].hand.size() < 7){
            drawCard(i, agents, deck);
        }
    }
    //Set topcard
    int tmp = rand() % deck.size(); //get random card
    discard.push_back(deck[tmp]); //Set it as topcard
    do{
        tmp = rand() % deck.size(); //get random card
        discard[0] = deck[tmp]; //Set it as topcard
    } while (deck[tmp].num > 9);

    deck.erase(deck.begin() + tmp); //erase card from deck
    
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
            switch(i){
                case 0:
                    tmp.color = "Red";
                    break;
                case 1:
                    tmp.color = "Yellow";
                    break;
                case 2:
                    tmp.color = "Blue";
                    break;
                case 3:
                    tmp.color = "Green";
                    break;
            }
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
            switch(i){
                case 0:
                    tmp.color = "Red";
                    break;
                case 1:
                    tmp.color = "Yellow";
                    break;
                case 2:
                    tmp.color = "Blue";
                    break;
                case 3:
                    tmp.color = "Green";
                    break;
            }
            tmp.num = j;

            //append it to deck
            deck.push_back(tmp);
        }
    }
    
    return deck;
}

void printHand(int current, vector<agent> agents){
    cout << "\nColor:\tValue:\tID:" << endl;
    for(int i=0; i<agents[current].hand.size(); i++){
        cout << agents[current].hand[i].color << "\t" << agents[current].hand[i].num << "\t" << i << endl;
    }
    cout << endl;
}

void printTopCard(vector<card> discard){
    card topCard = discard[discard.size() - 1];
    cout << "\nThe topcard is a " << topCard.color << " " << topCard.num << endl << endl;;
}

void printNums(vector<agent> agents){
    cout << "\nPlayer:\tCards:" << endl; 
    for(int i=0; i<agents.size(); i++){
        cout << i + 1 << "\t" << agents[i].hand.size() << endl;
    }
    cout << endl;
}

void drawCard(int currentAgent, vector<agent>& agents, vector<card>& deck){
    int tmp = rand() % deck.size(); //choose random card from deck
    agents[currentAgent].hand.push_back(deck[tmp]); //Set it as topcard
    deck.erase(deck.begin() + tmp); //erase card from deck
}

void playCard(int currentAgent, vector<agent>& agents, vector<card>& deck, vector<card>& discard){
    printHand(currentAgent, agents);

    int choice;

    //Handle bad input
    while((cout << "The ID of the card you wish to play: " && !(cin >> choice)) || choice < 0 || choice > agents[currentAgent].hand.size() - 1)
        badInput();

    card topCard = discard[discard.size() - 1];

    //Failure state
    if((agents[currentAgent].hand[choice].color != topCard.color && agents[currentAgent].hand[choice].num != topCard.num) && agents[currentAgent].hand[choice].num < WILD){
        cout << "This card cannot be played" << endl;
        return;
    }

    //In the event of a wild
    if(agents[currentAgent].hand[choice].num >= WILD){
        int colorChoice;

        cout << "1) Red\n"
        << "2) Yellow\n"
        << "3) Green\n"
        << "4) Blue\n";

        //Handle bad input
        while((cout << "Enter the number for the color you want: " && !(cin >> colorChoice)) || colorChoice < 1 || colorChoice > 4)
            badInput();

        card tmp;
        switch(colorChoice){
            case 1:
                tmp.color = "Red";
                break;
            case 2:
                tmp.color = "Yellow";
                break;
            case 3:
                tmp.color = "Green";
                break;
            case 4:
                tmp.color = "Blue";
                break;
        }

        tmp.num = agents[currentAgent].hand[choice].num;
        discard.push_back(tmp);
        agents[currentAgent].hand.erase(agents[currentAgent].hand.begin() + choice);
        return;
    }

    //update discard and hand accordingly
    discard.push_back(agents[currentAgent].hand[choice]);
    agents[currentAgent].hand.erase(agents[currentAgent].hand.begin() + choice);
    
}














void badInput(){
    cin.clear(); //clear bad input flag
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
    cout << "Invalid input; please re-enter.\n";
}










