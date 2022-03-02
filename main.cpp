#include <iostream>
#include "base/directory.h"

int main() {
    std::cout << utils::base::GetCurrentProcPath() << std::endl;
    return 0;
}
