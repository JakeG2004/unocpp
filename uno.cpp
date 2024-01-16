#include <iostream>
#include <vector>
#include <limits>
#include <stdlib.h> 

using namespace std;

enum vars {
    BACK = -1,
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
    int agentNum;
};

//Prototypes
vector<card> makeDeck();
vector<agent> makeAgents(int, int);
void distributeCards(vector<card>&, vector<card>&, vector<agent>&);
void printHand(agent&);
void printTopCard(vector<card>);
void printNums(vector<agent>);
void drawCard(agent&, vector<card>&, vector<card>&);
int playCard(agent&, vector<card>&, vector<card>&);
void playWild(int, agent&, vector<card>&);
void reversePlay(int&, vector<agent>&);
void plusCard(int, int, vector<agent>&, vector<card>&, vector<card>&);
void playerLoop(int&, int&, vector<agent>&, vector<card>&, vector<card>&);

void badInput();

int main(int argc, char* argv[]){

    bool win = false;
    srand(time(0));

    //Ensure correct usage
    if(argc != 2){
        cout << "Incorrect usage! Proper usage: ./uno.out <number of players>" << endl;
        return -1;
    }

    int numPlayers = stoi(argv[1]);
    //int numBots = stoi(argv[2]);

    //int numPlayers = 3;
    int numBots = 0;

    if(numPlayers + numBots > 10){
        cout << "Error: Too many agents to play the game. Max of 10." << endl;
        return -2;
    }

    //add players and bots to agent vector
    vector<agent> agents = makeAgents(numPlayers, numBots);

    //Establish discard
    vector<card> discard;

    //Establish deck
    vector<card> deck = makeDeck();

    //Distribute cards
    distributeCards(deck, discard, agents);

    int currentAgent = 0;
    int skip;

    //Establish deck
    while(!win){
        if(agents[currentAgent].type == PLAYER){
            playerLoop(currentAgent, skip, agents, deck, discard);
        } else {
            cout << "Bot" << endl;
        }

        //detect win
        if(agents[currentAgent].hand.size() == 0){
            win = true;
            cout << "Player " << agents[currentAgent].agentNum + 1 << " wins!" << endl;
            break;
        }


        currentAgent = (currentAgent + 1 + skip) % agents.size(); //go to next agent
    }

}

void playerLoop(int& currentAgent, int& skip, vector<agent>& agents, vector<card>& deck, vector<card>& discard){

    printTopCard(discard);

    int choice;
    bool repeatMenu = true;
    skip = 0;

    //keep prompting until exit condition is met
    while(repeatMenu){
        //Prompt player
        cout << "Player " << agents[currentAgent].agentNum + 1 << ", take your turn" << endl;
        cout << "1) View hand\n"
        << "2) View topcard\n"
        << "3) View # of cards in other hands\n"
        << "4) Draw a card\n"
        << "5) Play a card\n";

        //Handle bad input            
        while((cout << "> " && !(cin >> choice)) || choice < 1 || choice > 5)
            badInput();

        //Handle menu item
        switch(choice){
            case 1:
                printHand(agents[currentAgent]);
                break;
            case 2:
                printTopCard(discard);
                break;
            case 3:
                printNums(agents);
                break;
            case 4:
                drawCard(agents[currentAgent], deck, discard);
                repeatMenu = false;
                break;
            case 5:
                int tmp = playCard(agents[currentAgent], deck, discard);
                if(tmp != BACK)
                     repeatMenu = false;

                switch(tmp){
                    case SKIP:
                        skip = 1;
                        break;
                    case REVERSE:
                        reversePlay(currentAgent, agents);
                        break;
                    case PLUS2:
                        plusCard(2, currentAgent, agents, deck, discard);
                        break;
                    case WILD4:
                        plusCard(4, currentAgent, agents, deck, discard);
                        break;
                }
        }
    }

}

void distributeCards(vector<card>& deck, vector<card>& discard, vector<agent>& agents){
    //deal 7 cards to every agent
    for(int i=0; i<agents.size(); i++){
        //add 7 cards to each agent
        while(agents[i].hand.size() < 7){
            drawCard(agents[i], deck, discard);
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
        
        tmp.agentNum = i;
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

void printHand(agent& agent){
    cout << "\nID:\tColor:\tValue:" << endl;
    for(int i=0; i<agent.hand.size(); i++){

        //Convert value to text
        string tmp = to_string(agent.hand[i].num);
        switch(agent.hand[i].num){
            case SKIP:
                tmp = "Skip";
                break;
            case REVERSE:
                tmp = "Reverse";
                break;
            case PLUS2:
                tmp = "Plus 2";
                break;
            case WILD:
                tmp = "Wild";
                break;
            case WILD4:
                tmp = "Wild +4";
                break;
        }
        cout << i << "\t" << agent.hand[i].color << "\t" << tmp << endl;
    }
}

void printTopCard(vector<card> discard){
    card topCard = discard[discard.size() - 1];
    //Convert value to text
    string tmp = to_string(topCard.num);
    switch(topCard.num){
        case SKIP:
            tmp = "Skip";
            break;
        case REVERSE:
            tmp = "Reverse";
            break;
        case PLUS2:
            tmp = "Plus 2";
            break;
        case WILD:
            tmp = "Wild";
            break;
        case WILD4:
            tmp = "Wild +4";
            break;
    }
    cout << "\nThe topcard is a " << topCard.color << " " << tmp << endl << endl;;
}

void printNums(vector<agent> agents){
    cout << "\nPlayer:\tCards:" << endl; 
    for(int i=0; i<agents.size(); i++){
        cout << i + 1 << "\t" << agents[i].hand.size() << endl;
    }
    cout << endl;
}

void drawCard(agent& agent, vector<card>& deck, vector<card>& discard){

    //reshuffle if deck is empty
    if(deck.size() == 0){
        deck = discard;
        discard = {};
        discard.push_back(deck[deck.size() - 1]);
        deck.pop_back();
    }

    int tmp = rand() % deck.size(); //choose random card from deck
    agent.hand.push_back(deck[tmp]); //Set it as topcard
    deck.erase(deck.begin() + tmp); //erase card from deck
}

int playCard(agent& agent, vector<card>& deck, vector<card>& discard){
    printHand(agent);
    cout << agent.hand.size() << "\t" << "Back" << endl;
    card topCard = discard[discard.size() - 1];
    int choice;

    //Handle bad input
    while((cout << "The ID of the card you wish to play: " && !(cin >> choice)) || choice < 0 || choice > agent.hand.size()){
        badInput();
    }

    //In the event of "back"
    if(choice == agent.hand.size())
        return BACK;

    //Failure state
    while((agent.hand[choice].color != topCard.color && agent.hand[choice].num != topCard.num) && agent.hand[choice].num < WILD){
        cout << "This card cannot be played" << endl;
        cout << "Select a different card: ";
        cin >> choice;
    }

    int returnValue = agent.hand[choice].num;

    //In the event of a wild
    if(agent.hand[choice].num >= WILD){
        playWild(choice, agent, discard);
        return returnValue;
    }

    //update discard and hand accordingly
    discard.push_back(agent.hand[choice]);
    agent.hand.erase(agent.hand.begin() + choice);
    return returnValue;
}

void playWild(int choice, agent& agent, vector<card>& discard){
    int colorChoice;
    card tmp;

    //Apple new color
    cout << "1) Red\n"
    << "2) Yellow\n"
    << "3) Green\n"
    << "4) Blue\n";

    //Handle bad input
    while((cout << "Enter the number for the color you want: " && !(cin >> colorChoice)) || colorChoice < 1 || colorChoice > 4)
        badInput();

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

    //push new color to discard
    tmp.num = agent.hand[choice].num;
    discard.push_back(tmp);
    agent.hand.erase(agent.hand.begin() + choice);
}

void reversePlay(int& currentAgent, vector<agent>& agents){
    agent tmp;

    for(int i=0; i<agents.size() / 2; i++){
        tmp = agents[i];
        agents[i] = agents[agents.size() - 1 - i];
        agents[agents.size() - 1 - i] = tmp;

        currentAgent = (agents.size() - 1) - currentAgent;
    }
}

void plusCard(int n, int currentAgent, vector<agent>& agents, vector<card>& deck, vector<card>& discard){
    int nextAgent = (currentAgent + 1) % agents.size(); //go to next agent

    for(int i=0; i<n; i++){
        drawCard(agents[nextAgent], deck, discard);
    }
}








void badInput(){
    cin.clear(); //clear bad input flag
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
    cout << "Invalid input; please re-enter.\n";
}










