#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    char continueGame, characterClass;
    const int NO_STAMINA = 0;
    int characterStamina = 25;
    int hungerBar = 0;
    string characterClassroom, playerDecision, locationBank;


    // Class pick menu
    cout << "Please pick your class type:" << endl;
    cout << "Athlete Class (a) - Starts with protein bars" << "           " << "Geek Class (g) - Starts with umbrella" << endl;
    cin >> characterClass;
    // Cases for each class, needs to include items into player inventory before break also
    switch (characterClass)
    {
        case 'a': cout << "The Athlete starts with 1 special protein bar (More energy = more moves and points)" << endl;
        characterClassroom = "Dugan Wellness Center";
            break;
        case 'g': cout << "The Geek has an umbrella to avoid getting wet (Easier to travel)" << endl;
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

    // Game mechanics
    cout << "In this game you can either move forward, backward, left or right." << endl;
    cout << "You can also type in actions such as examine, use, and store followed by the desired item." << endl << endl;


    // Game loop continues while the endgame flag is false
    while (characterStamina > NO_STAMINA) {
        string location = "Library";

        cout << "       " << setw(6) << " Forward (f)" << endl;
        cout << setw(6) << "Left (l) " << "          " << " Right (r)"<< endl;
        cout << "       " << setw(6) << " Backward (b)" << endl << endl;
        cout << "  " << setw(6) << characterStamina << " Stamina Left" << endl << endl;
        cin >> playerDecision;
        characterStamina -= 1;

        // Warning that the player is running out of turns
        if (characterStamina < 15)
            cout << "You're feeling tired..." << endl;
        
        // Decision tree below based on player movement choices, and characterStamina.
        // definitely need to change how the movement system will work, need to use a method that doesnt use character
        // stamina to limit interactions
        
        // Library
        if (location == "Library") {
            if (playerDecision == "f" && characterStamina == 24)
                cout << "You walk to the sliding doors, it seems to be raining outside." << endl;
            else if (playerDecision == "f" && characterStamina == 23) {
                cout << "Walking through the sliding doors, you enter the Breezeway" << endl;
                location = "Breezeway";
            }
        }

        // Breezeway
        if (location == "Breezeway") {
            if (playerDecision == "f" && characterStamina == 23)
                cout << "You are now in the Breezeway" << endl;
            else if (playerDecision == "f" && characterStamina == 22)
                cout << "Walking straight, you're on a path in between starbucks and a statue." << endl;
        }

        /* playerDecision is stored as a char here still
        if (playerDecision == 'F' && characterStamina == 24)
            cout << "You walk to the sliding doors, it seems to be raining outside." << endl;
        else if (playerDecision == 'B' && characterStamina == 24)
            cout << "Watch out for those chess pieces!" << endl;
        else if (playerDecision == 'L' && characterStamina == 24)
            cout << "Looks like theres a bunch of chairs and books in the way, cant go there." << endl;
        else if (playerDecision == 'R' && characterStamina == 24)
            cout << "Theres the help desk! Looks like someone is waiting for you to ask a question." << endl;
        else
            cout << "Invalid input" << endl;
        */


    }
    
    // End text to show user stats, etc.
    if (characterStamina == 0) {
        cout << "Game Over. You ran out of time and missed your class." << endl;
        cout << "You finished with a total of " << hungerBar << " points!";
    }

}
