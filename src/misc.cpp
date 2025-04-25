#include "misc.hpp"
#include <string>

void die(uint8_t errorNum, std::string errorMsg) {
    std::cerr << "ERNO(" << std::to_string(errorNum) << "): " << errorMsg << std::endl;
    std::exit(errorNum);
}
