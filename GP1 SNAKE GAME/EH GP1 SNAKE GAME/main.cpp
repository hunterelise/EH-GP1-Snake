#include <conio.h>  
#include <iostream>
#include "ConsoleUtils.h"  
#include "SnakeGame.h"     

using namespace std;

int main() {
    while (true) {
        system("cls");  // Clear console
        cout << "==============================\n";
        cout << "        GAME SELECTION  \n";
        cout << "==============================\n";
        cout << "1. Snake\n";
        cout << "2. Exit\n";
        cout << "Choose a game: ";

        char choice = _getch();  // User input

        if (choice == '1') {
            SnakeGame game;
            game.init();
            game.run();
        }
        else if (choice == '2') {
            return 0;  // Exit program
        }
        else {
            cout << "Not an option! Press any key...";
            _getch();
        }
    }
}
