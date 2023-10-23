#include <iostream>

using namespace std;

int main()
{
    char continueGame;
    const int NO_STAMINA = 0;
    int characterClass, hungerBar = 0, characterStamina = 25;
    bool justStarted = true;
    string characterClassroom, playerDecision, locationBank;
    string location, locationMessage;

    // Class pick menu
    while(true){
        cout << "Please pick your class type:" << endl;
        cout << "Athlete Class (1) - Starts with protein bars" << "           "<< "Geek Class (2) - Starts with umbrella" << endl;

        if (cin >> characterClass) {
           break;
        } else { // Clears the cin if characterClass is something other than an int, such as a char or string
            cin.clear();
            cin.ignore();
            cout << "Invalid Input" << endl;
            continue;
        }
    }
    // Cases for each class, needs to include items into player inventory before break also
    switch (characterClass)
    {
        case 1: cout << "The Athlete starts with 1 special protein bar (More energy = more moves and points)" << endl;
        characterClassroom = "Dugan Wellness Center";
            break;
        case 2: cout << "The Geek has an umbrella to avoid getting wet (Easier to travel)" << endl;
        characterClassroom = "Science Center";
            break;
        default: cout << "Invalid input" << endl;
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
    // Set Starting conditions
    if (justStarted){
        location = "Library";
    }
    // Game loop continues while the endgame flag is false
    while (characterStamina > NO_STAMINA) {
        characterStamina -= 1;
        // Decision tree below based on player movement choices, and characterStamina.
        // definitely need to change how the movement system will work, need to use a method that doesn't use character
        // stamina to limit interactions
        // Library * this is restructured so that locationMessage will print correctly
        if (location == "Library") {
            locationMessage = "Welcome to the library";
            if (playerDecision == "f" && characterStamina == 23)
                locationMessage = "You walk to the sliding doors, it seems to be raining outside.";
            else if (playerDecision == "f" && characterStamina == 22) {
                locationMessage = "Walking through the sliding doors, you enter the Breezeway";
            }
            else if (playerDecision == "f" && characterStamina == 21 ){
                justStarted = false;
                location = "Breezeway";

            }
        }
        // Breezeway
        if (location == "Breezeway") {
            if (playerDecision == "f" && characterStamina == 21)
                locationMessage = "You are now in the Breezeway"; // need to fix location because it keeps getting set to library after here
            else if (playerDecision == "f" && characterStamina == 20)
                locationMessage = "Walking straight, you're on a path in between starbucks and a statue.";
            else if (playerDecision == "l" && characterStamina == 19) {
                locationMessage = "You enter the hector garcia plaza thing, kinda cool ig. You see something shiny at your feet...";
                location = "Hector";
            }
            else
                locationMessage = "error";
        }
        // Hector plaza
        if (location == "Hector") {
            if (playerDecision == "f" && characterStamina == 18) {
                locationMessage = "You can pickup the item, or chose to move";
            }
        }
        // UI
        cout << "|| " << locationMessage << endl;
        cout << "|]===============================[|" << endl;
        cout << "||         Forward (f)           ||" << endl;
        cout << "|| Left (l)            Right (r) ||"<< endl;
        cout << "||         Backward (b)          ||" << endl;
        cout << "||                               ||" << endl;
        cout << "||      " << characterStamina <<"  Stamina Left         ||" << endl;
        cout << "|]===============================[|" << endl;
        cin >> playerDecision;

        // Warning that the player is running out of turns
        if (characterStamina < 15)
            cout << "You're feeling tired..." << endl;
    }
    // End text to show user stats, etc.
    if (characterStamina == 0) {
        cout << "Game Over. You ran out of time and missed your class." << endl;
        cout << "You finished with a total of " << hungerBar << " points!";
    }
}
