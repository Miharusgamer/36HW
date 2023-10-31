#include "InputChecker.h"
#include <algorithm>

namespace {
const uint8_t MIN_LENGTH_USER_INPUT = 3;
}

uint8_t InputChecker::getMinLength()
{
    return MIN_LENGTH_USER_INPUT;
}

bool InputChecker::isTooShort(const std::string& input)
{
    return input.length() < MIN_LENGTH_USER_INPUT;
}

bool InputChecker::isContainsForbidenChars(const std::string& input)
{
    return std::any_of(input.begin(), input.end(), [](char c) {
        return !(std::isalnum(c) || c == '_' || c == '-');
    });
}

bool InputChecker::isStartsByNumber(const std::string& input)
{
    return std::isdigit(input[0]);
}
