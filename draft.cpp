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
void secretMiniGame(int randomNumber, int playerGuess, int counter, int finalScore, bool hasGuessed);
void vendingMachineMenu(int vendingChoice);
void printWetnessBar(int wetnessBar);
void playerUI(int playerActions, int hungerBar, int wetnessBar);
void diningHallMenu(int mealChoice);
void gameEndResults(int finalScore, int playerActions, int hungerBar);
void movementSystem(int playerActions, int mealChoice, int vendingChoice, int randomNumber, int playerGuess, int counter, int finalScore, bool hasGuessed);

int main()
{
    srand(time(NULL));
    bool hasGuessed = false;
    int randomNumber, playerGuess, counter = 0, finalScore, wetnessBar = 100, vendingChoice = 0, mealChoice = 0, actionCounter = 0, hungerBar = MAX_HUNGER, playerActions = MAX_STAMINA;
    set<string> allowedDecisions = {"w", "a", "s", "d", "1", "2", "3", "Yes", "yes", "backpack", "umbrella"};


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
          Decision tree func movementSystem  is based on player movement choices, and playerActions.
          definitely need to change how the movement system will work, need to use a method that doesn't use player
          stamina to limit interactions
        */

        if (allowedDecisions.count(playerDecision)) { // Input validation
            movementSystem(playerActions, mealChoice, vendingChoice, randomNumber, playerGuess, counter, finalScore, hasGuessed);
        } else { // Don't reduce hunger or stamina if an incorrect input is entered
            ++playerActions;
            if (playerActions > 25)
                playerActions = MAX_STAMINA;
            else {
                cout << "That didnt seem to do anything.." << endl;
                pauseHunger = true;
            }
        }
        // Number guessing game
        // secretMiniGame(randomNumber, playerGuess, counter, hasGuessed);

        // UI
        playerUI(playerActions, hungerBar, wetnessBar);

        // Add 20 hunger if player has been to the dining hall
        if (playerLocation == "Dining Hall" && hungerBar <= 80) {
            hungerBar += 20;
        } else if (playerLocation == "Dining Hall" && hungerBar >= 85) {
            hungerBar = MAX_HUNGER;
        }

        // -1 Stamina every action
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
                --wetnessBar;
            } else {
                cout << "You've been struck by lightning! You REALLY need to find some food.";
                hungerBar -= 20;
            }
        }

        // Warning that the player is running out of turns and hunger
        if (hungerBar <= 40) {
            cout << "You're feeling tired, Find food to refill your stamina." << endl;
        }

    } while (playerActions > 0 && hungerBar > 0); // End game loop when player runs out of actions/stamina or hunger bar

    // End text to show user stats, etc.
    gameEndResults(finalScore, playerActions, hungerBar);

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

void playerUI(int playerActions, int hungerBar, int wetnessBar){
    string staminaText = to_string(playerActions) + " Stamina"; // converts playerActions and adds it to string
    string hungerText = to_string(hungerBar) + " Hunger";
    int textPadding = max(staminaText.length(), hungerText.length()); // Calculates text padding based on the length of a string

    cout << "|| " << playerLocationMessage << endl; // Displays game messages, what the player can see, npc interactions, etc.
    cout << "|]===============================[|" << endl;
    cout << "           Forward (w)             " << endl;
    cout << "   Left (a)            Right (d)   "<< endl;
    cout << "           Backward (s)            " << endl;
    cout << "                                   " << endl;
    cout<< setw(17 + textPadding / 2) << staminaText << setw(8 + textPadding / 2)   << endl;
    cout << setw(17 + textPadding / 2) << hungerText << setw(8 + textPadding / 2)  << endl;
    cout << "            Wetness Bar" << endl; // Cant decide whether to display hungerBar as an actual bar or number.
    cout << "            ";
    printWetnessBar(wetnessBar);
    cout << endl;

    cout << "|]===============================[|" << endl;
    cout << "Current Location " << playerLocation << endl; // temp display location
    cin >> playerDecision;
}


void printWetnessBar(int wetnessBar) {
    int half = wetnessBar / 10;
    for (int i = 0; i < half; i++) {
    cout << "*";
    }
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
            while (cin.get() != '\n');
            cout << "Invalid Input" << endl;
            continue;
        }
    } while(true);

    // Cases for each class, needs to include items into player inventory before break also
    switch (playerClass)
    {
        case 1:
            playerClassroom = "Dugan Wellness Center"; // Classroom goal
            playerLocation = "Library"; // Starting location
            break;
        case 2:
            playerClassroom = "Science Center"; // Classroom goal
            playerLocation = "Library"; // Starting location
            break;
        default: cout << "Invalid input" << endl <<endl;
    }
}

void gameIntro(){
    getline(cin, playerDecision);
    // Intro text
    cout << "You just finished playing a game of chess in the library and feel quite hungry, your next class begins in 45 minutes." << endl;
    cout << "Make it to the dining hall to sooth your hunger before you make it to your next class in the " << playerClassroom << "." << endl;
    cout << "                                       Press enter to continue." << endl << endl;
    getline(cin, playerDecision);

    // Explain game mechanics
    cout << "Move either forward, backward, left or right." << endl;
    cout << "You can also type in actions such as examine, use, and store followed by the item name." << endl << endl;
    getline(cin, playerDecision);
}

// Conditions to make game end
void gameEndResults(int finalScore, int playerActions, int hungerBar){
    if (playerActions == 0) {
        cout << "Game Over, you ran out of time." << endl;
        cout << "You finished with a total of " << finalScore << " points!";
    } else if (hungerBar == 0){
        cout << "Game over, you ran out of hunger." << endl;
        cout << "You finished with a total of " << finalScore << " points";
    }
}

// Movement System and player interactions
void movementSystem(int playerActions, int mealChoice, int vendingChoice, int randomNumber, int playerGuess, int counter, int finalScore, bool hasGuessed){
    if (playerLocation == "Library") { // Decision tree for the Library, starting location.
        playerLocationMessage = "Welcome to the library";
        if (playerDecision == "w" && playerActions == 24)
            playerLocationMessage = "You walk to the sliding doors, it seems to be raining outside.";
        else if (playerDecision == "w" && playerActions == 23) {
            playerLocationMessage = "Walking through the sliding doors, you enter the Breezeway";
        } else if (playerDecision == "w" && playerActions == 22) {
            playerLocation = "Breezeway";
        } else if (playerDecision == "d" && playerActions == 23){
            playerLocationMessage = "You approach a man dressed in dark age clothing, do you wish to talk to him? Yes/No \n    Lets see if you can guess the secret number...";
        } else if (playerDecision == "Yes" || playerDecision == "yes" && playerActions == 22){
            secretMiniGame(randomNumber, playerGuess, counter, finalScore, hasGuessed);
        }
    } else if (playerLocation == "Breezeway") { // Decision tree for the Breezeway
        if (playerDecision == "w" && playerActions == 22)
            playerLocationMessage = "You are now in the Breezeway"; // need to fix playerLocation because it keeps getting set to library after here
        else if (playerDecision == "w" && playerActions == 21) {
            playerLocationMessage = "Walking straight, you're on a path in between starbucks and a statue.";
            playerLocation = "Outside";
        } else if (playerDecision == "a" && playerActions == 20) {
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

    // Vending Machine Menu
    vendingMachineMenu(vendingChoice);
}

// Need to add a decision path so player can access the Dining Hall location
void diningHallMenu(int mealChoice){
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
            playerLocation = "Plaza";
        }
        break;
    }while(true);
}

// Vending Machine interaction, need to add decision path that leads to vending machine
void vendingMachineMenu(int vendingChoice){
    do {
        if (playerLocation == "Vending Machine") {
            cout << "You've stumbled upon a vending machine..." << endl;
            cout << "Pick an option you'd enjoy." << endl;
            cout << "Meal (1): Protein Bar" << endl;
            cout << "Meal (2): M&Ms" << endl;
            cout << "Meal (3): Honey Bun microwaved for 10 minutes" << endl;
            cin >> vendingChoice;

            switch (vendingChoice) {
                case 1:
                    cout << "You chose the protein bar" << endl;
                    break;
                case 2:
                    cout << "You chose the M&Ms" << endl;
                    break;
                case 3:
                    cout << "You chose the honey bun" << endl;
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

void secretMiniGame(int randomNumber, int playerGuess, int counter, int finalScore, bool hasGuessed){
    randomNumber = rand() % 100 + 1;
    int bonusAmt;
    cout << "Guess the random number: ";
    do{
        ++counter;
        do {
            if (cin >> playerGuess) {
                if (playerGuess > 0) { // Check for correct input within a range
                    break;
                } else {
                    cout << "|| Invalid, enter a number: ";
                }
            } else {
                cout << "|| Invalid, enter a number: ";
                cin.clear();
                while (cin.get() != '\n');
            }
        } while (true);

        if(playerGuess == randomNumber){
            cout << "|| Congratulations! \n || You found the secret number!" << endl;
            hasGuessed = true;
        } else if(playerGuess > randomNumber){
            cout << "|| Lower, try again: ";
        } else if(playerGuess < randomNumber){
            cout << "|| Higher, try again: ";
        }
    }while(!hasGuessed);

    if (counter <= 4){
        finalScore += 10;
        bonusAmt = 10;
    } else if(counter >= 5 && counter <= 10){
        finalScore += 5;
        bonusAmt = 5;
    }

    cout << "|| It took you " << counter << " attempts!" << endl;
    cout << "|| That scored you " << bonusAmt << " points!" << endl;
}
