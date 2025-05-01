#include <iostream>
#include "HashT.h"
#include "Windows.h"

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Hashmap hm = Hashmap();
    std::string filename = (argc > 1) ? argv[1] : "";
    hm.menu(filename);
    return 0;
}
