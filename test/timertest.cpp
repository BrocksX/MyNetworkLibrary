#include "Timestamp.h"

int main()
{
    Timestamp time;
    std::cout << time.now().toFormattedString() << std::endl;
    std::cout << time.now().toFormattedString(true) << std::endl;
    return 0;
}