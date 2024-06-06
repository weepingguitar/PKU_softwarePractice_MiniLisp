#include "checkLegal.h"
#include "error.h"

void checkInputNumber(const std::vector<ValuePtr>& input, std::vector<int> nums){
    bool right = 0;
    for (int i : nums)
        if (input.size() == i) {
            right = 1;
            break;
        }
    if(!right){
        throw LispError("Wrong number of input.");
    }
}