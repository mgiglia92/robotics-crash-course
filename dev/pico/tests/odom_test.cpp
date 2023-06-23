#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    cout << "Starting odom test!" << "\n";
    Odom<12> left;
    Odom<13> right;
    
    while(true)
    {
        sleep_ms(100);
        cout << left.getCount() << " | " << right.getCount() << "\n";
			
    }
}
