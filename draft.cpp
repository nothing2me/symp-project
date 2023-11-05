#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <set>

using namespace std;
string playerClassroom,
        playerDecision,
        playerLocation,
        playerLocationMessage;
const int MAX_HUNGER = 100;
const int MAX_STAMINA = 25;
void splashScreen();
void classPicker();
void gameIntro();
void playerUI(int playerActions, int hungerBar);
void diningHallMenu(int mealChoice);
void gameEndConditions(int finalScore, int playerActions, int hungerBar);
void movementSystem(int playerActions, int mealChoice, int hungerBar);


// ADD SO THAT IT ONLY USES STAMINA IF THE INPUT IS only w, a, s, d and that playerActions AND hungerBar wont decrease
// if an invalid input is entered
int main()
{
    srand(time(NULL));
    int finalScore, mealChoice = 0, actionCounter = 0, hungerBar, playerActions;
    hungerBar = MAX_HUNGER;
    playerActions = MAX_STAMINA;
    set<string> allowedDecisions = {"w", "a", "s", "d", "1", "2", "3", "backpack", "umbrella"};

    // Splash Screen
    splashScreen();
    // Class Pick Menu
    classPicker();
    // Start of the Game
    gameIntro();

    // Game loop continues while player still has stamina and hunger
    do{
        // Operations needed every iteration
        actionCounter++;
        finalScore = (hungerBar + playerActions) / 2;

        bool pauseHunger = false; // handles when hungerBar can be manipulated
        /*
          Decision tree func below is based on player movement choices, and playerActions.
          definitely need to change how the movement system will work, need to use a method that doesn't use player
          stamina to limit interactions
        */
        
        if (allowedDecisions.count(playerDecision)) {
            movementSystem(playerActions, mealChoice, hungerBar);
        } else { 
            ++playerActions;
            if (playerActions > 25)
                playerActions = MAX_STAMINA;
            else {
                cout << "That didnt seem to do anything.." << endl;
                pauseHunger = true; 
            }
        }

        // UI
        playerUI(playerActions, hungerBar);
        /*
         add 20 hunger if the character has been to the Dining Hall, sets playerLocation
         to plaza to indicate the player has completed their first goal
         of getting lunch, and that they now need to head to their designated class.
        */
         if (playerLocation == "Dining Hall" && hungerBar <= 80) {
            hungerBar += 20;
        } else if (playerLocation == "Dining Hall" && hungerBar >= 85) {
            hungerBar = MAX_HUNGER;
            playerLocation = "Plaza";
        }

        // -1 Stamina every turn
        --playerActions;
        // Remove hunger every 5 actions
        actionCounter = actionCounter % 5;
        if (actionCounter == 0 && pauseHunger == 0)
            hungerBar = hungerBar - 5;


        // Chance for player to be struck by lightning if they are outside
        if (playerLocation == "Outside" && actionCounter == 0) {
            int chanceOfLightning;
            chanceOfLightning = rand() % 30 + 1;
            if (chanceOfLightning != 5) {
                cout << "You hear thunder, be advised you should head inside." << endl;
            } else {
                cout << "You've been struck by lightning! You REALLY need to find some food.";
                hungerBar -= 20;
            }
        }

        // Warning that the player is running out of turns and hunger
        if (hungerBar <= 40) {
            cout << "You're feeling tired, Find food to refill your stamina." << endl;
        }

    } while (playerActions > 0 && hungerBar > 0); // end game loop when player runs out of actions/stamina or hunger bar

    // End text to show user stats, etc.
    gameEndConditions(finalScore, playerActions, hungerBar);

    return 0;
}

void splashScreen(){
    cout << "||==============||" << endl;
    cout << "|| Mediocracy's ||" << endl;
    cout << "||==============||==========================================================================================||" << endl;
    cout << "||  ____  ____                                                   _______             _                      ||" << endl;
    cout << "|| |_   ||   _|                                                 |_   __ \\           (_)                     ||" << endl;
    cout << "||   | |__| |    __   _    _ .--.     .--./)  .---.   _ .--.      | |__) |  ,--.    __    _ .--.    .--.    ||" << endl;
    cout << "||   |  __  |   [  | | |  [ `.-. |   / /'`\\; / /__\\ [ `/'`\\]      |  ___/  `'_\\ :  [  |  [ `.-. |  ( (`\\]   ||" << endl;
    cout << "||  _| |  | |_   | \\_/ |,  | | | |   \\ \\._// | \\__.,  | |        _| |_     // | |,  | |   | | | |   `'.'.   ||" << endl;
    cout << "|| |____||____|  '.__.'_/ [___||__]  .',__`   '.__.' [___]      |_____|    \\'-;__/ [___] [___||__] [\\__) )  ||" << endl;
    cout << "||                                  ( ( __))                                                                ||" << endl;
    cout << "||==========================================================================================================||" << endl;\
}

void playerUI(int playerActions, int hungerBar){
    string staminaText = to_string(playerActions) + " Stamina"; // converts playerActions and adds it to string
    string hungerText = to_string(hungerBar) + " Hunger"; // Prev
    int textPadding = max(staminaText.length(), hungerText.length()); // Calculates text padding based on string length

    cout << "|| " << playerLocationMessage << endl;
    cout << "|]===============================[|" << endl;
    cout << "           Forward (w)             " << endl;
    cout << "   Left (a)            Right (d)   "<< endl;
    cout << "           Backward (s)            " << endl;
    cout << "                                   " << endl;
    cout<< setw(16 + textPadding / 2) << staminaText << setw(8 + textPadding / 2)   << endl;
    cout << setw(16 + textPadding / 2) << hungerText << setw(8 + textPadding / 2)  << endl;
    cout << "|]===============================[|" << endl;
    cout << "Current Location " << playerLocation << endl; // temp display location
    cin >> playerDecision;
}

void classPicker(){
    int playerClass;

    do{
        cout << "Please pick your class type:" << endl;
        cout << "Athlete Class (1) - Starts with protein bars" << endl;
        cout << "Geek Class    (2) - Starts with umbrella" << endl;

        if (cin >> playerClass && playerClass < 3 && playerClass > 0) {
            break;
        } else { // Clears the input if playerClass is something other than an int, such as a char or string
            cin.clear();
            cin.ignore();
            cout << "Invalid Input" << endl;
            continue;
        }
    } while(true);

    // Cases for each class, needs to include items into player inventory before break also
    switch (playerClass)
    {
        case 1:
            playerClassroom = "Dugan Wellness Center";
            playerLocation = "Library"; // Set starting location, used this one to test for Dining Hall func
            break;
        case 2:
            playerClassroom = "Science Center";
            playerLocation = "Library"; // Set starting location
            break;
        default: cout << "Invalid input" << endl <<endl;
    }
}

void gameIntro(){
    getline(cin, playerDecision);
    // Intro text
    cout << "You just finished playing a game of chess in the library and feel quite hungry, your next class begins in 45 minutes." << endl;
    cout << "Make it to the dining hall to sooth your hunger before you make it to your next class in the " << playerClassroom << "." << endl << endl;
    getline(cin, playerDecision);

    // Explain game mechanics
    cout << "Move either forward, backward, left or right." << endl;
    cout << "You can also type in actions such as examine, use, and store followed by the item name." << endl << endl;
    getline(cin, playerDecision);
}

void gameEndConditions(int finalScore, int playerActions, int hungerBar){
    if (playerActions == 0) {
        cout << "Game Over, you ran out of time." << endl;
        cout << "You finished with a total of " << finalScore << " points!";
    } else if (hungerBar == 0){
        cout << "Game over, you ran out of hunger." << endl;
        cout << "You finished with a total of " << finalScore << " points";
    }
}



void movementSystem(int playerActions, int mealChoice, int hungerBar2){
    if (playerLocation == "Library") { // Decision tree for the Library, starting location.
        playerLocationMessage = "Welcome to the library";
        if (playerDecision == "w" && playerActions == 23)
            playerLocationMessage = "You walk to the sliding doors, it seems to be raining outside.";
        else if (playerDecision == "w" && playerActions == 22) {
            playerLocationMessage = "Walking through the sliding doors, you enter the Breezeway";
        } else if (playerDecision == "w" && playerActions == 21) {
            playerLocation = "Breezeway";
        }
    } else if (playerLocation == "Breezeway") { // Decision tree for the Breezeway
        if (playerDecision == "w" && playerActions == 21)
            playerLocationMessage = "You are now in the Breezeway"; // need to fix playerLocation because it keeps getting set to library after here
        else if (playerDecision == "w" && playerActions == 20) {
            playerLocationMessage = "Walking straight, you're on a path in between starbucks and a statue.";
            playerLocation = "Outside";
        } else if (playerDecision == "a" && playerActions == 19) {
            playerLocationMessage = "You enter the hector garcia plaza thing, kinda cool ig. You see something shiny at your feet...";
            playerLocation = "Hector";
        } else
            playerLocationMessage = "error";
    } else if (playerLocation == "Hector") { // Decision tree for the Hector Plaza
        if (playerDecision == "w" && playerActions == 18) {
            playerLocationMessage = "You can pickup the item, or chose to move";
        }
    }

    // Dining Hall Menu
    diningHallMenu(mealChoice);
}

// need to add a decision path so player can access the Dining Hall location
void diningHallMenu(int mealChoice){
    // Dining Hall Menu
    do {
        if (playerLocation == "Dining Hall") {
            cout << "You've made it to the Dining Hall, here you can refill your hunger." << endl;
            cout << "Pick an option you'd enjoy." << endl;
            cout << "Meal (1): Burger and fries" << endl;
            cout << "Meal (2): Chicken nuggets and fries" << endl;
            cout << "Meal (3): Pizza and wings" << endl;
            cin >> mealChoice;

            switch (mealChoice) {
                case 1:
                    cout << "You chose the burger and fries." << endl;
                    break;
                case 2:
                    cout << "You chose the chicken nuggets and fries" << endl;
                    break;
                case 3:
                    cout << "You chose the pizza and wings." << endl;
                    break;
                default:
                    cout << "Invalid choice. Choose one of the listed options!" << endl;
                    continue;
            }
            cout << "After eating your food, you decide to head to your class in " << playerClassroom << "," << endl;
            cout << "walking outside from the Dining Hall you find yourself back at the plaza." << endl;
        }
        break;
    }while(true);
}
