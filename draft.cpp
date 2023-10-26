#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    char continueGame;
    const int NO_STAMINA = 0;
    int characterClass, hungerBar = 100, characterStamina = 25;
    string characterClassroom, 
           playerDecision,
           playerLocationBank,
           playerLocation,
           playerLocationMessage;


    // Splash Screen
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
    cout << "||==========================================================================================================||" << endl;

    // Class Pick Menu
    do{
        cout << "Please pick your class type:" << endl;
        cout << "Athlete Class (1) - Starts with protein bars" << endl;
        cout << "Geek Class    (2) - Starts with umbrella" << endl;

        if (cin >> characterClass && characterClass < 3 && characterClass > 0) {
                break;
        } else { // Clears the input if characterClass is something other than an int, such as a char or string
            cin.clear();
            cin.ignore();
            cout << "Invalid Input" << endl;
            continue;
        }
    } while(true);
    
    // Cases for each class, needs to include items into player inventory before break also
    switch (characterClass)
    {
        case 1: cout << "The Athlete starts with 1 special protein bar (More energy = more moves and points)" << endl;
        characterClassroom = "Dugan Wellness Center";
        playerLocation = "Library"; // Set starting location
            break;
        case 2: cout << "The Geek has an umbrella to avoid getting wet (Easier to travel)" << endl;
        characterClassroom = "Science Center";
        playerLocation = "Library"; // Set starting location
            break;
        default: cout << "Invalid input" << endl <<endl;
    }

    // Start of the Game
    cout << "When ready, press the enter key to proceed with the game." << endl;
    cin.get();
    cin.get(continueGame);
    
    // Intro text
    cout << "You just finished playing a game of chess in the library and feel quite hungry, your next class begins in 45 minutes." << endl;
    cout << "Make it to the dining hall to sooth your hunger before you make it to your next class in the " << characterClassroom << "." << endl << endl;
    cout << "press the enter key to proceed with the game." << endl;
    cin.get(continueGame);
    
    // Explain game mechanics
    cout << "Move either forward, backward, left or right." << endl;
    cout << "You can also type in actions such as examine, use, and store followed by the item name." << endl << endl;
    
    // Game loop continues while the endgame flag is false
    while (characterStamina > NO_STAMINA) {
        characterStamina -= 1;
        
        // Decision tree below is based on player movement choices, and characterStamina.
        // definitely need to change how the movement system will work, need to use a method that doesn't use character
        // stamina to limit interactions
        // Library * this is restructured so that playerLocationMessage will print correctly
        if (playerLocation == "Library") {
            playerLocationMessage = "Welcome to the library";
            if (playerDecision == "f" && characterStamina == 23)
                playerLocationMessage = "You walk to the sliding doors, it seems to be raining outside.";
            else if (playerDecision == "f" && characterStamina == 22) {
                playerLocationMessage = "Walking through the sliding doors, you enter the Breezeway";
            }
            else if (playerDecision == "f" && characterStamina == 21 ){
                playerLocation = "Breezeway";

            }
        }
        
        // Breezeway
        if (playerLocation == "Breezeway") {
            if (playerDecision == "f" && characterStamina == 21)
                playerLocationMessage = "You are now in the Breezeway"; // need to fix playerLocation because it keeps getting set to library after here
            else if (playerDecision == "f" && characterStamina == 20)
                playerLocationMessage = "Walking straight, you're on a path in between starbucks and a statue.";
            else if (playerDecision == "l" && characterStamina == 19) {
                playerLocationMessage = "You enter the hector garcia plaza thing, kinda cool ig. You see something shiny at your feet...";
                playerLocation = "Hector";
            }
            else
                playerLocationMessage = "error";
        }
        
        // Hector plaza
        if (playerLocation == "Hector") {
            if (playerDecision == "f" && characterStamina == 18) {
                playerLocationMessage = "You can pickup the item, or chose to move";
            }
        }
        
        // UI
        cout << "|| " << playerLocationMessage << endl;
        cout << "|]===============================[|" << endl;
        cout << "||         Forward (f)           ||" << endl;
        cout << "|| Left (l)            Right (r) ||"<< endl;
        cout << "||         Backward (b)          ||" << endl;
        cout << "||                               ||" << endl;
        cout << "||         " << characterStamina <<" Stamina" << setw(14) << "||" << endl;
        cout << "||         "<< hungerBar << " Hunger" << setw(14) << "||" << endl;
        cout << "|]===============================[|" << endl;
        cin >> playerDecision;

        // Warning that the player is running out of turns and hunger
        if (hungerBar <= 40)
            cout << "You're feeling tired, Find food to refill your stamina." << endl;
    }
    
    // End text to show user stats, etc.
    if (characterStamina == 0) {
        cout << "Game Over. You ran out of time and missed your class." << endl;
        cout << "You finished with a total of " << hungerBar << " points!";
    }

}
