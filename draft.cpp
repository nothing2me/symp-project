#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    char playerDecision, ch, characterClass;
    int characterStamina = 25;
    int hungerBar = 0;
    bool endgame = false;

    // Class pick menu
    cout << "Please pick your class type:" << endl;
    cout << "Athlete Class (a) - Starts with protein bars" << "           " << "Geek Class (g) - Starts with umbrella" << endl;
    cin >> characterClass;
    // Cases for each class, needs to include items into player inventory before break also
    switch (characterClass)
    {
        case 'a': cout << "The Athlete starts with 1 special protein bar (More energy = more moves)" << endl;
            break;
        case 'g': cout << "The Geek has an umbrella to avoid getting wet (Easier to travel)" << endl;
            break;
        default: cout << "Invalid input" << endl;
    }

    // Start of the Game
    cout << "When ready, press the enter key to proceed with the game." << endl;
    cin.get();
    cin.get(ch);

    // Intro prompt
    cout << "You just finished playing a game of chess in the library and feel quite hungry, your next class begins in an hour and 30 minutes." << endl;
    cout << "Make it to the dining hall to quench your hunger before you make it to your next class." << endl << endl;

    // Game loop continues while the endgame flag is false
    while (!endgame)
    {
        cout << "       " << setw(6) << " Forward (F)" << endl;
        cout << setw(6) << "Left (L) " << "          " << " Right (R)"<< endl;
        cout << "       " << setw(6) << " Backward (B)" << endl << endl;

        cin >> playerDecision;
        characterStamina -= 1;

        cout << characterStamina << endl;

        // Decision tree based on player movement choices, and characterStamina.
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

        if (characterStamina < 15)
            cout << "You're feeling tired..." << endl;

    }


}
