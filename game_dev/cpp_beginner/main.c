#include <iostream>

int main(void)
{
    bool isRunning = true;
    char choice;

    while(isRunning)
    {
                

        
        std::cout << "Do another calculation? ('y' to do another): ";
        std::cin >> choice;
        if(choice == 'y')
            continue;
        else
            isRunning = false;
    }

    return 0;    
}
