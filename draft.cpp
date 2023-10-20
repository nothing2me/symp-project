#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
 char decision;
 int stamina = 25;
 int hungerBar = 0;
 bool endgame = false;
 
 
 cout << "You just finished playing a game of chess in the library and feel quite hungy, your next class begins in an hour and 30 minutes." << endl;
 cout << "Make it to the dining hall to quench your hunger before you make it to your next class." << endl << endl;
 
 while (endgame == false)
 {
    cout << "       " << setw(6) << " Forward (F)" << endl;
    cout << setw(6) << "Left (L) " << "          " << " Right (R)"<< endl;
    cout << "       " << setw(6) << " Backward (B)" << endl << endl;
    
    cin >> decision;
    stamina -= 1;
    
    cout << stamina << endl;
    
    if (stamina < 15)
        cout << "You're feeling tired..." << endl;
    
    if (decision == 'F' && stamina == 24)
        cout << "You walk to the sliding doors, it seems to be raining outside." << endl;
    else if (decision == 'B' && stamina == 24)
        cout << "Watch out for those chess pieces!" << endl;
    else if (decision == 'L' && stamina == 24)
        cout << "Looks like theres a bunch of chairs and books in the way, cant go there." << endl;
    else if (decision == 'R' && stamina == 24)
        cout << "Theres the help desk! Looks like someone is waiting for you to ask a question." << endl;
    else
        cout << "Invalid input" << endl;
    }
}
