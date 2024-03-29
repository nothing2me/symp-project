/*
 Just as a heads-up I know how bad this code looks
 I really just hacked it all together and made it work :p
 Sometimes inputs need to be entered twice for yes/no questions,
 I still cant figure out why :> - Noah
*/
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <set>
using namespace std;
// Function declarations
void splashScreen();
void gameIntro();
void printWetnessBar(int wetnessBar);
void diningHallMenu(int mealChoice);
void saveProgress(int playerActions, int hungerBar, int timeLeft);
void loadSaveFile(int& playerActions, int & hungerBar, int & timeLeft, int & playerClass);
//void vendingMachineMenu(bool& hasEnergy, int vendingChoice); // Didn't end up being used because of time
void gameEndResults(int finalScore, int playerActions, int hungerBar, int timeLeft);
void classPicker(int& playerClass, bool & hasUmbrella, bool & hasEnergy);
void secretMiniGame(bool& hasUmbrella, bool hasGuessed, int & finalScore, int randomNumber, int playerGuess, int counter);
void playerUI(bool hasUmbrella, bool hasEnergy, int playerActions, int hungerBar, int wetnessBar, int finalScore, int timeLeft);
int movementSystem(int& wetnessBar, int & playerActions, int& examScore, int & hungerBar, int & finalScore, int mealChoice, int vendingChoice, int actionCounter, int randomNumber, int playerGuess, int playerClass, int counter, bool hasGuessed, bool& hasUmbrella, bool& hasEnergy);
int finalExam(int& examScore,int& finalScore, int randomNumber, int playerGuess, int counter, bool &hasUmbrella, bool hasGuessed);
int useItems(bool& hasUmbrella, bool& hasEnergy, int& hungerBar, int& wetnessBar);
// Global declarations
ifstream saveFile("savefile.txt");
bool goalTwo = false, continueGame = true;
const int MAX_HUNGER = 100,
        MAX_MOISTURE = 100,
        MAX_STAMINA = 15,
        MAX_TIME = 60;
string playerClassroom,
        playerDecision,
        playerLocation,
        playerLocationMessage;

int main() {
    system("Color 03");
    srand(time(NULL));
    bool hasGuessed = false, hasUmbrella = false, hasEnergy = false;
    int randomNumber, playerClass, playerGuess, finalScore, examScore = 0, vendingChoice = 0, mealChoice = 0, actionCounter = 0, counter = 0, timeLeft = MAX_TIME, wetnessBar = MAX_MOISTURE, hungerBar = MAX_HUNGER, playerActions = MAX_STAMINA;
    set < string > allowedDecisions({
                                            "w",
                                            "a",
                                            "s",
                                            "d",
                                            "1",
                                            "2",
                                            "3",
                                            "Yes",
                                            "yes",
                                            "No",
                                            "no",
                                            "Leave",
                                            "leave",
                                            "backpack",
                                            "Use umbrella",
                                            "use umbrella",
                                            "Use Energy Bar",
                                            "use energy bar"
                                    });
    playerLocationMessage = "Welcome to the library! \n|| You can see a set of sliding doors in front of you, \n|| and a librarian working on a computer to your right."; // Starting message

    // Splash Screen
    splashScreen();
    // Class Pick Menu
    classPicker(playerClass, hasUmbrella, hasEnergy);
    // Start of the Game
    gameIntro();

    // Check for save file
    cout << "|| Do you already have a save file? Y/N" << "\n";
    cin >> playerDecision;
    if (playerDecision == "y" || playerDecision == "Y")
        loadSaveFile(playerActions, hungerBar, timeLeft, playerClass);
    else
        cout << "|| Creating save file..." << "\n";

    // Game loop continues while player still has stamina and hunger
    do {
        // Operations needed every iteration
        actionCounter++;

        bool pauseHunger = false; // handles when hungerBar can be manipulated
        bool pausePlayerActions = false;

        getline(cin, playerDecision);

        if (allowedDecisions.count(playerDecision)) { // Input validation
            examScore = movementSystem(wetnessBar, playerActions, finalScore, mealChoice, examScore, hungerBar, vendingChoice, randomNumber, playerGuess, actionCounter, counter, playerClass, hasGuessed, hasUmbrella, hasEnergy);
        } else { // Don't reduce hunger or stamina if an incorrect input is entered
            ++playerActions;
            if (playerActions > 15) {
                playerActions = MAX_STAMINA;
            } else if (playerLocationMessage == "Cant go that way") {
                pausePlayerActions = true;
            } else {
                cout << "\n" << "\n";
                pauseHunger = true;
            }
        }

        // Strike player with lightning
        if (playerLocation == "Outside") {
            int chanceOfLightning;
            chanceOfLightning = rand() % 25 + 1;

            if (chanceOfLightning != 5) {
                cout << "|| You hear thunder, be advised you should head inside.\n";
                wetnessBar -= 10;
            } else {
                cout << "|| You've been struck by lightning! You REALLY need to find some food.\n";
                hungerBar -= 40;
            }
        }
        // Keep playerActions from overflowing
        if (playerActions > 15) {
            playerActions = MAX_STAMINA;
        }

        // UI
        playerUI(hasUmbrella, hasEnergy, playerActions, hungerBar, wetnessBar, finalScore, timeLeft);

        // Add 20 hunger if player has been to the dining hall
        if (playerLocation == "Dining Hall Plaza") {
            hungerBar += 10;
        }

        // -1 Stamina every action
        if (!pausePlayerActions) {
            --playerActions;
            timeLeft -= 5;
        } else if (playerActions == 10){
            timeLeft += 5;
        }
        // Remove hunger every 5 actions
        actionCounter = actionCounter % 2;
        if (actionCounter == 0 && pauseHunger == 0)
            hungerBar -= 15;

        // Warning that the player is running out of turns and hunger
        if (hungerBar <= 40) {
            cout << " You're feeling tired, Find food to refill your stamina." << "\n";
        }

        // Player loses hunger if theyre too wet
        if (wetnessBar <= 50) {
            hungerBar -= 5;
        }

        if (playerDecision == "leave" || playerDecision == "Leave") {
            continueGame = false;
        }

        useItems(hasUmbrella, hasEnergy, hungerBar, wetnessBar);
        // End game loop when player runs out of actions/stamina or hunger bar
    } while (continueGame && timeLeft > 0 && (playerActions > 0 && hungerBar > 0));

    // Calculate final score
    finalScore = ((hungerBar + playerActions) / 2) + examScore;
    if (finalScore > 200)
        finalScore = 60;

    // Shows player results
    gameEndResults(finalScore, playerActions, hungerBar, timeLeft);

    // Save progress
    saveProgress(playerActions, hungerBar, timeLeft);

    return 0;
}

void splashScreen() {
    cout << "||==============||" << "\n";
    cout << "|| Mediocracy's ||" << "\n";
    cout << "||==============||==========================================================================================||" << "\n";
    cout << "||  ____  ____                                                   _______             _                      ||" << "\n";
    cout << "|| |_   ||   _|                                                 |_   __ \\           (_)                     ||" << "\n";
    cout << "||   | |__| |    __   _    _ .--.     .--./)  .---.   _ .--.      | |__) |  ,--.    __    _ .--.    .--.    ||" << "\n";
    cout << "||   |  __  |   [  | | |  [ `.-. |   / /'`\\; / /__\\ [ `/'`\\]      |  ___/  `'_\\ :  [  |  [ `.-. |  ( (`\\]   ||" << "\n";
    cout << "||  _| |  | |_   | \\_/ |,  | | | |   \\ \\._// | \\__.,  | |        _| |_     // | |,  | |   | | | |   `'.'.   ||" << "\n";
    cout << "|| |____||____|  '.__.'_/ [___||__]  .',__`   '.__.' [___]      |_____|    \\'-;__/ [___] [___||__] [\\__) )  ||" << "\n";
    cout << "||                                  ( ( __))                                                                ||" << "\n";
    cout << "||==========================================================================================================||" << "\n";\
}

void playerUI(bool hasUmbrella, bool hasEnergy, int playerActions, int hungerBar, int wetnessBar, int finalScore, int timeLeft) {
    string staminaText = to_string(playerActions) + " Stamina"; // converts playerActions and adds it to string
    string hungerText = to_string(hungerBar) + " Hunger";
    string timeText= to_string(timeLeft) + " Minutes Left";
    int textPadding = max(staminaText.length(), hungerText.length()); // Calculates text padding based on the length of a string

    cout << "|| " << playerLocationMessage << "\n"; // Displays game messages, what the player can see, npc interactions, etc.
    cout << "|]===============================[|" << "\n";
    cout << "           Forward (w)             " << "\n";
    cout << "   Left (a)            Right (d)   " << "\n";
    cout << "           Backward (s)            " << "\n";
    cout << "                                   " << "\n";
    cout << setw(18 + textPadding / 2) << timeText << setw(8 + textPadding / 2) << "\n";
    cout << setw(16 + textPadding / 2) << staminaText << setw(8 + textPadding / 2) << "\n";
    cout << setw(16 + textPadding / 2) << hungerText << setw(8 + textPadding / 2) << "\n";
    cout << "           Wetness Bar" << "\n"; // Cant decide whether to display hungerBar as an actual bar or number.
    cout << "           ";
    printWetnessBar(wetnessBar);
    cout << "\n";
    if (hasUmbrella || hasEnergy) {
        cout << "|]===============================[|\n";
        cout << "           Inventory:           \n";
        if (hasUmbrella && hasEnergy) {
            cout << "          -Umbrella           \n";
            cout << "          -Energy Bar          \n";
        } else if (hasEnergy) {
            cout << "          -Energy Bar          \n";
        } else if (hasUmbrella) {
            cout << "          -Umbrella           \n";
        }
    }
    cout << "|]===============================[|\n";
    cout << "|| Location:  " << playerLocation << "\n"; // temp display location
    cout << "||";
}

void printWetnessBar(int wetnessBar) {
    int half = wetnessBar / 10;
    for (int i = 0; i < half; i++) {
        cout << "*";
    }
}

void classPicker(int & playerClass, bool & hasUmbrella, bool & hasEnergy) {
    do {
        cout << "|| Please pick your class type:" << "\n";
        cout << "|| 1) Jock - Starts with energy bars" << "\n";
        cout << "|| 2) Geek - Starts with an umbrella" << "\n";

        if (cin >> playerClass && (playerClass == 1 || playerClass == 2)) {
            break;
        } else { // Clears the input if playerClass is something other than an int, such as a char or string
            cin.clear();
            while (cin.get() != '\n');
            cout << " That didnt seem to do anything..." << "\n";
            continue;
        }
    } while (true);

    // Cases for each class, needs to include items into player inventory before break also
    switch (playerClass) {
        case 1:
            playerClassroom = "Dugan Wellness Center"; // Classroom goal
            playerLocation = "Library"; // Starting location
            hasEnergy = true;
            break;
        case 2:
            playerClassroom = "Bayhall"; // Classroom goal
            playerLocation = "Library"; // Starting location
            hasUmbrella = true;
            break;
        default:
            cout << "Invalid input" << "\n" << "\n";
    }
}

void gameIntro() {
    getline(cin, playerDecision);
    // Intro text
    cout << "|| You just finished playing a game of chess in the library and feel quite hungry,\n";
    cout << "|| Make it to the dining hall before time runs out for your \n|| 'General College Knowledge Standardized Assessment Test' in " << playerClassroom << "!\n";
    cout << "|| You have 60 minutes to complete these objectives.\n";
    cout << "|| Press enter to continue.\n";
    getline(cin, playerDecision);

    // Explain game mechanics
    cout << "|| Move either forward, backward, left or right.\n";
    cout << "|| You can also type in actions such as use,\n|| depending on the items in your inventory.\n";
    cout << "|| Press enter to continue.\n";
    getline(cin, playerDecision);
}

// Shows player results when the game ends
void gameEndResults(int finalScore, int playerActions, int hungerBar, int timeLeft) {
    if (playerActions == 0 || timeLeft == 0) {
        cout << "|| Game Over, you ran out of time.";
    } else if (hungerBar == 0) {
        cout << "|| Game over, you ran out of hunger.";
    }

    cout << "|| You finished with a total of " << finalScore << " points,\n";
    cout << "|| " << playerActions << " stamina, " << hungerBar << " hunger, and " << timeLeft << " minutes left to spare.\n";
    cout << "|| Thanks for playing! \n";
}

// Movement System and player interactions
int movementSystem(int& wetnessBar, int& playerActions, int& examScore, int& hungerBar, int& finalScore, int mealChoice, int vendingChoice, int randomNumber, int playerGuess, int counter, int actionCounter, int playerClass, bool hasGuessed, bool& hasUmbrella, bool& hasEnergy) {
    if (playerLocation == "Library"){
        // Decision tree
        if (playerDecision == "w" && playerActions == 14) {
            playerLocationMessage = "Walking into the breezeway, you can see: \n|| the O'Connor Plaza to your left \n|| a long, wide walkway infront of you \n|| and to your right, a map.";
            playerLocation = "Breezeway";
        } else if (playerDecision == "d" && playerActions == 14){
            playerLocationMessage = "You approach the libray's front desk, a welcoming face greets you. \n|| Hello and welcome to the library! How can I help you today? \n|| 1) Directions to class \n|| 2) Directions to the Dining Hall";
        } else if (playerDecision == "1" && playerActions == 13){
            if (playerClass == 2){
                playerLocationMessage = "You'll find your exam room in Bayhall, \n|| just straight out from the sliding doors and \n|| walk down the pathway, itll be on your left.\n|| Satisfied with their directions, you leave and enter the Breezeway";
            } else {
                playerLocationMessage = "You'll find your exam room in the Dugan, \n|| just straight out from the sliding doors and to your right, \n|| from there you'll find path way leading to both the Dining Hall and the Dugan.\n|| Satisfied with their directions, you leave and enter the Breezeway";
            }
            playerLocation = "Breezeway";
            ++playerActions;
        } else if (playerDecision == "2" && playerActions == 13){
            playerLocationMessage = "Just walk straight out the sliding doors \n|| and down the pathway, you be able to see its building from the end. \n|| From there you'll go straight and take a left into the plaza. \n|| Satisfied with their directions, you leave and enter the Breezeway\n";
            playerLocation = "Breezeway";
            ++playerActions;
        }
        else {
            playerLocationMessage = "Cant go that way...";
            playerActions += 1;
        }
    } else if (playerLocation == "Breezeway"){
        if (playerDecision == "w" && playerActions == 13){
            playerLocationMessage = "Going down a wide pathway you notice a large building \n|| labled 'Bayhall', it seems to be of interest. \n|| Would you like to go inside? yes/no";
            playerLocation = "Outside";
        } else if (playerDecision == "a" && playerActions == 13){
            playerLocationMessage = "Walking left you enter the O'Connor plaza, \n|| its decorated with all sorts of festive lights! \n|| You notice a hallway to your left leading towards the parking lot, \n|| and to your right you see a direct path to a building labeled 'Bayhall'.";
            playerLocation = "O'Connor Plaza";
        } else if (playerDecision == "d" && playerActions == 13) {
            playerLocationMessage = "Walking towards the UC, you notice two pathways \n|| one leading to the Dugan Gym \n|| the other leading to the Dining Hall \n|| Where would you like to go? \n|| 1) Dugan Gym \n|| 2) Dining Hall";
            playerLocation = "University Center Plaza";
        } else {
            playerLocationMessage = "Cant go that way...";
            playerActions += 1;
        }
    } else if (playerLocation == "Outside"){
        if (playerClass == 1){
            if ((playerDecision == "Yes" || playerDecision == "yes") && playerActions == 12) {
                playerLocationMessage = "The building seems closed for some reason... \n|| Leaving the building, you're able to see the Dining Hall from here! \n|| You just need to walk straight and turn left through the plaza.";
            } else if ((playerDecision == "No" || playerDecision == "no") && playerActions == 12) {
                playerLocationMessage = "You can see the Dining Hall from here! \n|| You just need to walk straight and turn left through the plaza.";
            } else if (playerDecision == "w" && playerActions == 11) {
                playerLocationMessage = "Walking down the path you find yourself in the plaza, \n|| To your left you see the Dining Hall.";
            } else if (playerDecision == "a" && playerActions == 10) {
                playerLocation = "Dining Hall";
                playerActions += 1;
            }else {
                playerLocationMessage = "Cant go that way...";
                playerActions += 1;
            }
        } else if (playerClass == 2) {
            if ((playerDecision == "Yes" || playerDecision == "yes") && playerActions == 12) {
                playerLocationMessage = "It looks to be where you'll need to take your exam! \n|| But its not yet time for it.  \n|| Leaving the building, you're able to see the Dining Hall from here! \n|| You just need to walk straight and turn left through the plaza.";
            } else if ((playerDecision == "No" || playerDecision == "no") && playerActions == 12) {
                playerLocationMessage = "You can see the Dining Hall from here! \n|| You just need to walk straight and turn left through the plaza.";
            } else if (playerDecision == "w" && playerActions == 11) {
                playerLocationMessage = "Walking down the path and through the plaza, \n|| you find yourself at the doors of the dining hall. \n|| Going inside, you find a row of ordering kiosks";
                playerLocation = "Dining Hall";
            } else {
                playerLocationMessage = "Cant go that way...";
                playerActions += 1;
            }
        }

    } else if (playerLocation == "Dining Hall Plaza"){
        if (playerClass == 1) {
            if (playerDecision == "d" && playerActions == 10) {
                playerLocationMessage = "Going up the steps to Bayhall, you walk inside and notice \n|| that this isn't your classroom!! \n|| Darting to the Dugan Gym, you make it to the classroom just in time. \n|| Are you ready to take the exam? yes/no ";\
                playerLocation = "Exam Classroom";
            } else if (playerDecision == "a" && playerActions == 10) {
                playerLocationMessage = "Going up the steps to the Dugan, you walk inside and notice \n|| a classroom labeled 'Exam Room' to your right, it looks to be your destination! \n|| Are you ready to take the exam? yes/no";;
                playerLocation = "Exam Classroom";

            }else {
                playerLocationMessage = "Cant go that way...";
                playerActions += 1;
            }
        }
        if (playerClass == 2) {
            if (playerDecision == "d" && playerActions == 10) {
                playerLocationMessage = "Going up the steps to Bayhall, you walk inside and notice a staircase in front of you leading \n|| towards a classroom labeled 'Exam Room', it looks to be your destination!\n|| Are you ready to take the exam? yes/no";
                playerLocation = "Exam Classroom";
            } else if (playerDecision == "a" && playerActions == 10) {
                playerLocationMessage = "Going up the steps to the Dugan, you walk inside and notice \n|| that this isn't your classroom!! \n|| Darting to the Bayhall, you make it to the classroom just in time. \n|| Are you ready to take the exam? yes/no";
                playerLocation = "Exam Classroom";
            }else {
                playerLocationMessage = "Cant go that way...";
                playerActions += 1;
            }
        }

    } else if (playerLocation == "O'Connor Plaza"){
        if (playerDecision == "a" && playerActions == 12){
            playerLocationMessage = "Heading towards the parking lot already? \n|| You haven't eaten or gone to your exam yet... \n|| Are you sure you want to leave? yes/no";
        } else if((playerDecision == "Yes" || playerDecision == "yes") && playerActions == 11){
            continueGame = false;
        } else if((playerDecision == "No" || playerDecision == "no") && playerActions == 11){
            playerLocationMessage = "Sounds like a good decision player, \n|| Walking back from the parking lot, you go through the plaza \n|| and make your way down the pathway to Bayhall. \n || Would you like to go inside? yes/no";
            playerLocation = "Outside";
            playerActions += 2;
        } else if (playerDecision == "d" && playerActions == 12){
            playerLocationMessage = "You go through the plaza, and make your way down the pathway to Bayhall. \n || Would you like to go inside? yes/no";
            playerLocation = "Outside";
            playerActions += 1;
        } else {
            playerLocationMessage = "Cant go that way...";
            playerActions += 1;
        }
    } else if (playerLocation == "University Center Plaza"){
        if (playerDecision == "1" && playerActions == 12){
            if (playerClass == 1){
                cout << "\n|| It looks to be where you'll need to take your exam! \n|| But its not yet time for it,  \n|| you still need to go to the Dining Hall.\n|| Your able to see the dining hall from here! \n|| Walking down the path and through the plaza, \n|| you find yourself at its doors.\n";
            } else if(playerClass == 2) {
                cout << "|| It seems to be closed today, how odd. \n|| Your able to see the dining hall from here! \n|| Walking down the path and through the plaza, \n|| you find yourself at its doors.\n";
            }
            playerLocation = "Dining Hall";
            --playerActions;
        } else if (playerDecision == "2" && playerActions == 12){
            playerLocationMessage = "Walking down the path and through the plaza, \n|| you find yourself at the doors of the dining hall. \n|| Enter? Yes / No";
        } else if ((playerDecision == "Yes" || playerDecision == "yes") && playerActions == 11){
            playerLocation = "Dining Hall";
        } else if ((playerDecision == "No" || playerDecision == "no") && playerActions == 11){
            cout << "|| Your hunger overwhelms you, and your arms and legs move on their own taking you directly to the order kiosks.";
            playerLocation = "Dining Hall";
        } else {
            playerLocationMessage = "Cant go that way...";
            playerActions += 1;
        }
    } else if (playerLocation == "Exam Classroom"){
        if(playerDecision == "No" || playerDecision == "no"){
            cout << "\n|| What else are you gonna do??? \n|| Go home?\n";
        }
        finalExam(examScore, finalScore, randomNumber, playerGuess, counter, hasUmbrella, hasGuessed);
        if (goalTwo) {
            playerLocationMessage = "You've finished your exam!";
        }
        continueGame = false;
    }

    // Dining Hall Menu
    diningHallMenu(mealChoice);

    // Vending Machine Menu
    //vendingMachineMenu(hasEnergy, vendingChoice);

    return examScore;
}

void diningHallMenu(int mealChoice) {
    do {
        if (playerLocation == "Dining Hall") {
            cout << "|| You've made it to the Dining Hall!" << "\n";
            cout << "|| Pick an option you'd enjoy." << "\n";
            cout << "|| 1) - Burger and fries" << "\n";
            cout << "|| 2) - Tofu and udon" << "\n";
            cout << "|| 3) - Chicken curry and tureric rice" << "\n";
            cin >> mealChoice;

            switch (mealChoice) {
                case 1:
                    cout << "|| You chose the burger and fries.\n";
                    break;
                case 2:
                    cout << "|| You chose the Tofu and udon\n";
                    break;
                case 3:
                    cout << "|| You chose the Stir fry beef and rice\n";
                    break;
                default:
                    cout << "|| Invalid choice. Choose one of the listed options!\n";
                    continue;
            }
            cout << "|| After eating your food, you decide to head to your class in " << playerClassroom << ",\n";
            playerLocationMessage = "Walking outside from the Dining Hall you find yourself back at the plaza. \n|| Walking forward, you can see the Dugan gym to your left \n|| and Bayhall to your right.";
            playerLocation = "Dining Hall Plaza";
        }
        break;
    } while (true);
}

// Vending Machine interaction, cutting this out for time purposes
/*void vendingMachineMenu(bool & hasEnergy, int vendingChoice) {
    do {
        if (playerLocation == "Vending Machine") {
            cout << "|| You've stumbled upon a vending machine..." << "\n";
            cout << "|| Pick an option you'd enjoy." << "\n";
            cout << "|| Meal (1): Energy Bar" << "\n";
            cout << "|| Meal (2): M&Ms" << "\n";
            cout << "|| Meal (3): Honey Bun microwaved for 10 minutes" << "\n";
            cin >> vendingChoice;

            switch (vendingChoice) {
                case 1:
                    cout << "|| You chose the Energy bar" << "\n";
                    hasEnergy = true;
                    break;
                case 2:
                    cout << "|| You chose the M&Ms" << "\n";
                    break;
                case 3:
                    cout << "|| You chose the honey bun" << "\n";
                    break;
                default:
                    cout << "Invalid choice. Choose one of the listed options!" << "\n";
                    continue;
            }
            cout << "|| walking away from the vending machine you find yourself back in the" << playerLocation << "." << "\n";
        }
        break;
    } while (true);
}*/

void secretMiniGame(bool & hasUmbrella, bool hasGuessed, int & finalScore, int randomNumber, int playerGuess, int counter) {
    randomNumber = rand() % 100 + 1;
    int bonusAmt;
    cout << "|| Guess the random number: ";
    do {
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

        if (playerGuess == randomNumber) {
            cout << "|| Congratulations! \n|| You found the secret number!" << "\n";
            /* if (counter <= 10) { // this game was originally meant to be played before you took the exam, but due to time constraints were not doi
                 if (hasUmbrella) {
                     cout << "|| You've already got an umbrella, \n|| most I can offer you is some bonus points instead" << "\n";
                     finalScore += 15;
                 } else {
                     cout << "|| You've won an umbrella!" << "\n";
                     hasUmbrella = true; // Set hasUmbrella to true here
                 }
            */
            hasGuessed = true;
        }

        if (playerGuess > randomNumber) {
            cout << "|| Lower, try again: ";
        } else if (playerGuess < randomNumber) {
            cout << "|| Higher, try again: ";
        }
    } while (!hasGuessed);
    if (counter <= 10) {
        bonusAmt = 10;
        finalScore += 10;
    } else {
        bonusAmt = 5;
        finalScore += 5;
    }

    cout << "|| It took you " << counter << " attempts and" << "\n";
    cout << "|| scored you " << bonusAmt << " bonus points." << "\n";
}

void loadSaveFile(int & playerActions, int & hungerBar, int & timeLeft, int & playerClass) {
    ifstream saveFile("savefile.txt");

    if (!saveFile.is_open()) {
        cout << "No save found." << "\n";
    } else {

        saveFile.ignore(numeric_limits < streamsize > ::max(), ':'); // Ignore until the ':' character
        saveFile >> playerActions;

        saveFile.ignore(numeric_limits < streamsize > ::max(), ':'); // Ignore until the ':' character
        saveFile >> hungerBar;

        saveFile.ignore(numeric_limits < streamsize > ::max(), ':'); // Ignore until the ':' character
        saveFile >> timeLeft;

        saveFile.ignore(numeric_limits < streamsize > ::max(), ':'); // Ignore until the ':' character
        saveFile >> playerLocation;

        saveFile.ignore(numeric_limits < streamsize > ::max(), ':'); // Ignore until the ':' character
        saveFile >> playerLocationMessage;

        saveFile.ignore(numeric_limits < streamsize > ::max(), ':'); // Ignore until the ':' character
        saveFile >> playerClass;

        saveFile.close();
    }
}
void saveProgress(int playerActions, int hungerBar, int timeLeft) {
    // Save progress
    ofstream outputFile("savefile.txt");
    if (!outputFile.is_open()) {
        cout << "Unable to open save file." << "\n";
    } else {
        // Write data to the file
        outputFile << "PActions  : " << playerActions << "\n";
        outputFile << "HBar      : " << hungerBar << "\n";
        outputFile << "MLeft     : " << timeLeft << "\n";
        outputFile << "PLocation : " << playerLocation << "\n";
        outputFile << "PLMessage : " << playerLocationMessage << "\n";
        outputFile.flush();
        outputFile.close();
        cout << "|| File saved successfully" << "\n";
    }
}

int finalExam(int& examScore, int& finalScore, int randomNumber, int playerGuess, int counter, bool & hasUmbrella, bool hasGuessed) {
    char userAnswer;
    int arraySize = 10;
    examScore = 0;
    cout << "|| Welcome to your 'General College Knowledge Standardized Assessment Test' \n|| Please enter your answers in uppercase, such as A, B, C, T or F. \n|| Enter your answers in lowercase\n";
    // Array for Q&A combinations
    string questions[arraySize] = {
            "|| How many years does it take to graduate with a bachelor's degree? \n|| A) 4 years. B) 8 years. C) 2 years.\n",
            "|| How many credit hours do you have to have to be considered a full-time student? \n|| A) 10 Hours. B) 15 hours. C) 12 hours.\n",
            "|| What building is Chic-Fil-A located in?  \n|| A) University Center. B) Island Hall. C) Bay Hall. \n",
            "|| Which of the following subjects is a core curriculum? \n|| A) English. B) History. C) Both \n",
            "|| Which letter grade corresponds to 80-89? \n|| A) C. B) B. C) A.\n",
            "|| You must take 2 history classes as well as a political science class in order to graduate. T/F?\n",
            "|| Attending your classes is not mandatory and you can go when you want. T/F? \n",
            "|| You can get to any of your classes by walking. T/F?\n",
            "|| If you get caught cheating, you could risk receiving a 0 on the assignment or worse. T/F?\n",
            "|| With a meal plan, you can get unlimited meals all year long. T/F?\n"
    };

    char answers[arraySize] = {
            'a',
            'c',
            'a',
            'c',
            'b',
            't',
            'f',
            't',
            't',
            't'
    };
    // Input validation and question prompt loop
    for (int i = 0; i < arraySize; ++i) {
        cout << questions[i];
        bool validInput = false;
        while (!validInput) {
            string userInput;
            cin >> userInput;
            userAnswer = tolower(userInput[0]);  // Take the first character from the input

            if (userInput.length() == 1 && (userAnswer == 'a' || userAnswer == 'b' || userAnswer == 'c' || userAnswer == 't' || userAnswer == 'f')) {
                validInput = true;
            } else {
                cout << "|| Invalid input. Please enter A, B, C, T, or F: ";
            }
        }

        if (userAnswer == answers[i]) {
            cout << "|| You got the answer correct!\n";
            examScore += 10;
        } else {
            cout << "|| Incorrect, the answer is " << answers[i] << "\n";
        }
    }
    cout << "|| Exam Grade: " << examScore << "\n";

    if (examScore >= 70) {
        cout << "|| Congrats on passing the exam! You've won!!\n";
        cout << "|| A man with long grey hair walks toward you, \n|| 'Would you like to play a game?' he asks... yes/no\n";
        cin >> playerDecision;
        if (playerDecision == "Yes" || playerDecision == "yes") {
            secretMiniGame(hasUmbrella, hasGuessed, finalScore, randomNumber, playerGuess, counter);
        } else
            cout << "|| Alright then, I was gonna give you bonus points for playing but NoOoOo you dont wanna play my game. Hmph.\n";
    } else {
        cout << "|| Wow. You failed the exam, better luck next time...\n";
    }
    goalTwo = true;
    return examScore;
}

int useItems(bool& hasUmbrella, bool& hasEnergy, int& hungerBar, int& wetnessBar) {
    if (hasUmbrella && playerLocation == "Outside" && (playerDecision == "Use umbrella" || playerDecision == "use umbrella")) {
        cout << "|| You've equipped an umbrella.\n";
        wetnessBar += 10;
    } else if ((playerDecision == "Use umbrella" || playerDecision == "use umbrella") && !hasUmbrella){
        cout << "|| You dont have an umbrella...\n";
    }

    if (hasEnergy && (playerDecision == "Use Energy bar" || playerDecision == "use energy bar")) {
        cout << " You've eaten a Energy bar.\n";
        hungerBar += 15;
        hasEnergy = false;
    } else if ((playerDecision == "Use Energy Bar" || playerDecision == "use energy bar") && !hasEnergy){
        cout << "|| You dont have a Energy bar...\n";
    }
    return 0;
}
