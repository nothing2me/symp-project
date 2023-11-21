#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <set>

using namespace std;
string playerClassroom,
        playerDecision,
        playerLocation,
        playerLocationMessage;
ifstream saveFile("savefile.txt");
const int MAX_HUNGER = 100;
const int MAX_STAMINA = 25;
void splashScreen();
void classPicker(int& playerClass, bool& hasUmbrella, bool& hasProtein);
void gameIntro();
void secretMiniGame(bool& hasUmbrella, bool hasGuessed, int randomNumber, int playerGuess, int counter);
void vendingMachineMenu(int vendingChoice);
void printWetnessBar(int wetnessBar);
void playerUI(bool hasUmbrella, bool hasProtein,int playerActions, int hungerBar, int wetnessBar, int finalScore);
void diningHallMenu(int mealChoice);
void saveProgress(int playerActions, int hungerBar);
void loadSaveFile(int& playerActions, int& hungerBar);
void gameEndResults(int finalScore, int playerActions, int hungerBar);
int movementSystem(int& wetnessBar, int& playerActions, int& examScore, int finalScore, int correctAnswers,  int mealChoice, int vendingChoice,int actionCounter, int hungerBar, int randomNumber, int playerGuess, int playerClass, int counter, bool hasGuessed, bool& hasUmbrella);
int finalExam(int& examScore);
int useItems(bool& hasUmbrella, bool& hasProtien, int& hungerBar, int& wetnessBar);
int lightningChance(int& wetnessBar, int& hungerBar ,int actionCounter);
int endGame = 1;
bool goalOne = false, goalTwo = false;

int main()
{
    system("Color 03");
    srand(time(NULL));
    bool hasGuessed = false, hasUmbrella = false, hasProtein = false;
    int randomNumber, playerClass, playerGuess, correctAnswers = 100, examScore = 0, counter = 0, finalScore, wetnessBar = 100, vendingChoice = 0, mealChoice = 0, actionCounter = 0, hungerBar = MAX_HUNGER, playerActions = MAX_STAMINA;
    set<string> allowedDecisions = {"w", "a", "s", "d", "1", "2", "3", "Yes", "yes", "backpack","Use umbrella", "use umbrella", "leave"};
    playerLocationMessage = "Welcome to the library, you can see a mysterious man standing forward and to your right";

    // Splash Screen
    splashScreen();
    // Class Pick Menu
    classPicker(playerClass, hasUmbrella, hasProtein);
    // Start of the Game
    gameIntro();
    // Load Save

    cout << "|| Do you already have a save file? y/n" << endl;
    cin >> playerDecision;
    if(playerDecision == "y" || playerDecision == "Y")
        loadSaveFile(playerActions, hungerBar);
    else
        cout << "|| Creating save file..." << endl;

    // Game loop continues while player still has stamina and hunger
    do{
        // Operations needed every iteration
        actionCounter++;
        finalScore = (hungerBar + playerActions + examScore) / 3;

        bool pauseHunger = false; // handles when hungerBar can be manipulated
        bool pausePlayerActions = false;
        /*
          Decision tree func movementSystem  is based on player movement choices, and playerActions.
          definitely need to change how the movement system will work, need to use a method that doesn't use player
          stamina to limit interactions
        */
        getline(cin, playerDecision);

        if (allowedDecisions.count(playerDecision)) { // Input validation
            movementSystem(wetnessBar, playerActions, finalScore, mealChoice, examScore, vendingChoice, correctAnswers, randomNumber, playerGuess, actionCounter, hungerBar, counter, playerClass, hasGuessed, hasUmbrella);
        } else { // Don't reduce hunger or stamina if an incorrect input is entered
            ++playerActions;
            if (playerActions > 25) {
                playerActions = MAX_STAMINA;
            }else if(playerLocationMessage == "Cant go that way"){
                pausePlayerActions = true;
            }
            else {
                cout << "|| That didnt seem to do anything..." << endl;
                pauseHunger = true;
            }
        }
        // Keep playerActions from overflowing
        if (playerActions > 25) {
            playerActions = MAX_STAMINA;
        }

        // UI
        playerUI(hasUmbrella,  hasProtein, playerActions, hungerBar, wetnessBar, finalScore);

        // Add 20 hunger if player has been to the dining hall
        if (playerLocation == "Dining Hall" && hungerBar <= 80) {
            hungerBar += 20;
            goalOne = true;
        } else if (playerLocation == "Dining Hall" && hungerBar >= 85) {
            hungerBar = MAX_HUNGER;
            goalOne = true;
        }

        // -1 Stamina every action
        if(!pausePlayerActions) {
            --playerActions;
        }
        // Remove hunger every 5 actions
        actionCounter = actionCounter % 5;
        if (actionCounter == 0 && pauseHunger == 0)
            hungerBar = hungerBar - 5;

        // Warning that the player is running out of turns and hunger
        if (hungerBar <= 40) {
            cout << "|| You're feeling tired, Find food to refill your stamina." << endl;
        }

        // Player loses hunger if theyre too wet
        if(wetnessBar < 50){
            hungerBar -= 5;
        }

        if (playerDecision == "leave" || playerDecision == "Leave"){
            endGame = 0;
        }


        useItems(hasUmbrella, hasProtein, hungerBar, wetnessBar);

    } while (endGame > 0 && (playerActions > 0 && hungerBar > 0));
    // End game loop when player runs out of actions/stamina or hunger bar

    // Shows player results, score
    gameEndResults(finalScore, playerActions, hungerBar);

    // Save progress
    saveProgress(playerActions, hungerBar);



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

void playerUI(bool hasUmbrella, bool hasProtein, int playerActions, int hungerBar, int wetnessBar, int finalScore){
    string staminaText = to_string(playerActions) + " Stamina"; // converts playerActions and adds it to string
    string hungerText = to_string(hungerBar) + " Hunger";
    int textPadding = max(staminaText.length(), hungerText.length()); // Calculates text padding based on the length of a string

    cout << "|| " << playerLocationMessage << endl; // Displays game messages, what the player can see, npc interactions, etc.
    cout << "|]===============================[|" << endl;
    cout << "           Forward (w)             " << endl;
    cout << "   Left (a)            Right (d)   "<< endl;
    cout << "           Backward (s)            " << endl;
    cout << "                                   " << endl;
    if(hasUmbrella || hasProtein) {
        cout << "           Inventory:           " << endl;
        if (hasUmbrella && hasProtein) {
            cout << "           -Umbrella           " << endl;
            cout << "           -Protein Bar          " << endl;
        } else if (hasProtein) {
            cout << "           -Protein Bar          " << endl;
        } else if (hasUmbrella) {
            cout << "           -Umbrella           " << endl;
        }
    }
    cout<< setw(16 + textPadding / 2) << staminaText << setw(8 + textPadding / 2)   << endl;
    cout << setw(16 + textPadding / 2) << hungerText << setw(8 + textPadding / 2)  << endl;
    cout << "           Wetness Bar" << endl; // Cant decide whether to display hungerBar as an actual bar or number.
    cout << "           ";
    printWetnessBar(wetnessBar);
    cout << endl;
    cout << "|]===============================[|" << endl;
    cout << "|| Location:  " << playerLocation << endl; // temp display location
    cout << "||";
}


void printWetnessBar(int wetnessBar) {
    int half = wetnessBar / 10;
    for (int i = 0; i < half; i++) {
    cout << "*";
    }
}

void classPicker(int& playerClass, bool& hasUmbrella, bool& hasProtein){
    do{
        cout << "|| Please pick your class type:" << endl;
        cout << "|| Athlete Class (1) - Starts with protein bars" << endl;
        cout << "|| Geek Class (2) - Starts with umbrella" << endl;

        if (cin >> playerClass && playerClass < 3 && playerClass > 0) {
            break;
        } else { // Clears the input if playerClass is something other than an int, such as a char or string
            cin.clear();
            while (cin.get() != '\n');
            cout << " That didnt seem to do anything..." << endl;
            continue;
        }
    } while(true);

    // Cases for each class, needs to include items into player inventory before break also
    switch (playerClass)
    {
        case 1:
            playerClassroom = "Dugan Wellness Center"; // Classroom goal
            playerLocation = "Library"; // Starting location
            hasProtein = true;
            break;
        case 2:
            playerClassroom = "Bayhall"; // Classroom goal
            playerLocation = "Library"; // Starting location
            hasUmbrella = true;
            break;
        default: cout << "Invalid input" << endl <<endl;
    }
}

void gameIntro(){
    getline(cin, playerDecision);
    // Intro text
    cout << "|| You just finished playing a game of chess in the library and feel quite hungry, your next class begins in 45 minutes." << endl;
    cout << "|| Make it to the dining hall to sooth your hunger before you make it to your next class in the " << playerClassroom << "." << endl;
    cout << "|| Press enter to continue." << endl << endl;
    getline(cin, playerDecision);

    // Explain game mechanics
    cout << "|| Move either forward, backward, left or right." << endl;
    cout << "|| You can also type in actions such as examine, use, and store followed by the item name."<< endl;
    cout << "|| Press enter to continue." << endl << endl;
    getline(cin, playerDecision);
}

// Shows player results when the game ends
void gameEndResults(int finalScore, int playerActions, int hungerBar){
    if (playerActions == 0) {
        cout << "|| Game Over, you ran out of time." << endl;
    } else if (hungerBar == 0){
        cout << "Game over, you ran out of hunger." << endl;
    }
    cout << " You finished with a total of " << finalScore << " points" << endl;
    cout << "|| With " << playerActions << " stamina and " << hungerBar << " hunger left." << endl;
    cout << "|| Good Job! " << endl;
}

// Movement System and player interactions
int movementSystem(int& wetnessBar, int& playerActions, int& examScore, int finalScore, int mealChoice, int correctAnswers,  int vendingChoice, int randomNumber, int playerGuess, int counter, int actionCounter, int hungerBar, int playerClass, bool hasGuessed, bool& hasUmbrella){
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
            secretMiniGame(hasUmbrella, hasGuessed, randomNumber, playerGuess, counter);
        } else if (playerDecision == "w" && playerActions == 21){
            playerLocationMessage = "Good job playing the game and good luck out there, It seems to be raining.";
            playerLocation = "Breezeway";
            ++playerActions;
        }else{
                playerLocationMessage = "Cant go that way...";
                playerActions += 1;
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
        } else{
                playerLocationMessage = "Cant go that way...";
                playerActions += 1;
        }
    } else if (playerLocation == "Hector") { // Decision tree for the Hector Plaza
        if (playerDecision == "w" && playerActions == 18) {
            playerLocationMessage = "You can pickup the item, or chose to move";
        } else{
                playerLocationMessage = "Cant go that way...";
                playerActions += 1;
        }
    } else if (playerLocation == "Classroom") {
        finalExam(examScore);
        if (goalTwo){
            playerLocationMessage = "You've finished your exam!";
        }
        endGame = 0;
    } else if (playerLocation == "Outside"){
        playerLocationMessage = "Walking forward, you can see a  building labled 'Bayhall' infront of you and to the left. \n || You also notice the Faculty building to your right.";
        if (playerDecision == "w" && playerActions == 19){
            if (playerClass == 2){
                playerLocationMessage = "It seems to be where you need to take your exam! \n || Would you like to go inside? y/n";
            } else {
                playerLocationMessage = "The building doesnt seem to be anything of interest, but you can see some vending machines through a window. \n || Would you like to go inside? y/n";
            }
        }else if (playerDecision == "yes" || playerDecision == "Yes" && playerActions  == 17){
            playerLocationMessage = "Entering Bayhall";
            playerLocation = "Bayhall";
        } else {
            playerLocationMessage = "Cant go that way...";
            ++playerActions;
        }
    } else if (playerLocation == "Bayhall"){
        playerLocationMessage = "You find yourself in front of the doors. Entering, you now see two stretches of hallway, \n || One in front of you leading towards a stairset, and to your right where you see a row of \n || vending machines";
        if(playerDecision == "w" && playerActions == 16){
            if(goalOne) {
                playerLocationMessage = "Walking up the stairs, you eventually find yourself infront of the testing room. \n || Are you ready to take the exam? yes/no";
                if (playerDecision == "yes" || playerDecision == "Yes" && playerActions == 15)
                    playerLocationMessage = "Good luck!";
                else
                    playerLocationMessage = "Too bad.";
                playerLocation = "Classroom";
            } else{
                playerLocationMessage = "Not quite time to go to your exam, you walk back downstairs and out the front doors \n || You can see the what looks to be the dining hall from here, along with the dugan gym.";
                playerLocation = "Outside";
            }
        }
    }

    if (playerLocation == "Outside") {
        int chanceOfLightning;
        chanceOfLightning = rand() % 25 + 1;

        if (chanceOfLightning != 5) {
            cout << "|| You hear thunder, be advised you should head inside." << endl;
            wetnessBar -= 10;
        } else {
            cout << "|| You've been struck by lightning! You REALLY need to find some food.";
            hungerBar -= 40;
        }
        return 0;
    }
    // Dining Hall Menu
    diningHallMenu(mealChoice);

    // Vending Machine Menu
    vendingMachineMenu(vendingChoice);

    return 0;
}

// Need to add a decision path so player can access the Dining Hall location
void diningHallMenu(int mealChoice){
    do {
        if (playerLocation == "Dining Hall") {
            cout << "|| You've made it to the Dining Hall, here you can refill your hunger." << endl;
            cout << "|| Pick an option you'd enjoy." << endl;
            cout << "|| Meal (1): Burger and fries" << endl;
            cout << "|| Meal (2): Chicken nuggets and fries" << endl;
            cout << "|| Meal (3): Pizza and wings" << endl;
            cin >> mealChoice;

            switch (mealChoice) {
                case 1:
                    cout << "|| You chose the burger and fries." << endl;
                    break;
                case 2:
                    cout << "|| You chose the chicken nuggets and fries" << endl;
                    break;
                case 3:
                    cout << "|| You chose the pizza and wings." << endl;
                    break;
                default:
                    cout << "|| Invalid choice. Choose one of the listed options!" << endl;
                    continue;
            }
            cout << "|| After eating your food, you decide to head to your class in " << playerClassroom << "," << endl;
            cout << "|| walking outside from the Dining Hall you find yourself back at the plaza." << endl;
            playerLocation = "Plaza";
        }
        break;
    }while(true);
}

// Vending Machine interaction, need to add decision path that leads to vending machine
void vendingMachineMenu(int vendingChoice){
    do {
        if (playerLocation == "Vending Machine") {
            cout << "|| You've stumbled upon a vending machine..." << endl;
            cout << "|| Pick an option you'd enjoy." << endl;
            cout << "|| Meal (1): Protein Bar" << endl;
            cout << "|| Meal (2): M&Ms" << endl;
            cout << "|| Meal (3): Honey Bun microwaved for 10 minutes" << endl;
            cin >> vendingChoice;

            switch (vendingChoice) {
                case 1:
                    cout << "|| You chose the protein bar" << endl;
                    break;
                case 2:
                    cout << "|| You chose the M&Ms" << endl;
                    break;
                case 3:
                    cout << "|| You chose the honey bun" << endl;
                    break;
                default:
                    cout << "Invalid choice. Choose one of the listed options!" << endl;
                    continue;
            }
            cout << "|| walking away from the vending machine you find yourself back in the" << playerLocation <<"." << endl;
        }
        break;
    }while(true);
}

void secretMiniGame(bool& hasUmbrella, bool hasGuessed, int randomNumber, int playerGuess, int counter){
    randomNumber = rand() % 100 + 1;
    int bonusAmt;
    cout << "|| Guess the random number: ";
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

        if (playerGuess == randomNumber) {
            cout << "|| Congratulations! \n || You found the secret number!" << endl;
            if (counter <= 20) {
                cout << "|| You've won an umbrella!" << endl;
                hasUmbrella = true;  // Set hasUmbrella to true here
            }
            hasGuessed = true;
        } else if (playerGuess > randomNumber) {
            cout << "|| Lower, try again: ";
        } else if (playerGuess < randomNumber) {
            cout << "|| Higher, try again: ";
        }
    }while(!hasGuessed);
    if(counter <= 5){
        bonusAmt = 10;
    } else{
        bonusAmt = 5;
    }

    cout << "|| It took you " << counter << " attempts!" << endl;
    cout << "|| That scored you " << bonusAmt << " bonus points." << endl;
}

void loadSaveFile(int& playerActions, int& hungerBar) {
    ifstream saveFile("savefile.txt");

    if (!saveFile.is_open()) {
        cout << "No save found." << endl;
    } else {
        saveFile.ignore(numeric_limits<streamsize>::max(), ':'); // Ignore until the ':' character
        saveFile >> playerActions;

        saveFile.ignore(numeric_limits<streamsize>::max(), ':'); // Ignore until the ':' character
        saveFile >> hungerBar;

        saveFile.close();
    }
}
void saveProgress(int playerActions, int hungerBar) {
    // Save progress
    ofstream outputFile("savefile.txt");
    if (!outputFile.is_open()) {
        cout << "Unable to open save file." << endl;
    } else {
        // Write data to the file
        outputFile << "Player Actions: " << playerActions << endl;
        outputFile << "Hunger Bar    : " << hungerBar << endl;
        outputFile.flush();
        outputFile.close();
        cout << "|| File saved successfully" << endl;
    }
}

int finalExam(int& examScore) {
    char userAnswer;
    cout << "|| Welcome to your 'General College Knowledge Standardized Assessment Test' \n|| Please enter your answers in uppercase, such as A, B, C, T or F. \n" ;
    string questions[10] = {
            "|| How many years does it take to graduate with a bachelor's degree? \n || A) 4 years. B) 8 years. C) 2 years.\n",
            "|| How many credit hours do you have to have to be considered a full-time student? \n || A) 10 Hours. B) 15 hours. C) 12 hours.\n",
            "|| What building is Chic-Fil-A located in?  || A) University Center. B) Island Hall. C) Bay Hall. \n",
            "|| Which of the following subjects is a core curriculum? \n || A) English. B) History. C) Both \n",
            "|| Which letter grade corresponds to 80-89? \n || A) C. B) B. C) A.\n",
            "|| You must take 2 history classes as well as a political science class in order to graduate. T/F?\n",
            "|| Attending your classes is not mandatory and you can go when you want. T/F? \n",
            "|| You can get to any of your classes by walking. T/F?",
            "|| If you get caught cheating, you could risk receiving a 0 on the assignment or worse. T/F?\n",
            "|| With a meal plan, you can get unlimited meals all year long. T/F?\n"
    };

    char answers[10] = {
            'A','C','A','C','B','T','F','T','T', 'T'
    };

    for(int i = 0; i < 10; ++i){
        cout << questions[i];
        cin >> userAnswer;
        if (userAnswer == answers[i]) {
            playerLocationMessage = "|| You got the answer correct!\n";
            examScore += 10;
        } else{
            cout << "|| Incorrect, the answer is " << answers[i] << " \n";
        }
    }
    cout << "|| Exam Grade: " << examScore << endl;
    if (examScore >=60)
        cout << "|| Congrats on passing the exam! You've won!!" << endl;
    else
        cout << "|| Wow you failed. Not suprising." << endl;

    goalTwo = true;
    return examScore;
}

int useItems(bool& hasUmbrella, bool& hasProtein, int& hungerBar, int& wetnessBar){
    if (playerDecision == "Use umbrella" || playerDecision == "use umbrella") {
        cout << " You've equipped an umbrella." << endl;
        if (playerLocation == "Outside" && hasUmbrella) {
            wetnessBar += 10;
        }
    }

    if (playerDecision == "Use protein bar" || playerDecision == "use protein bar") {
        cout << "You've eaten a protein bar." << endl;
        hungerBar += 10;
        hasProtein = false;
    }
    return 0;
}
