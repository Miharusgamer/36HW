#ifndef INPUTCHECKER_H
#define INPUTCHECKER_H

#include <string>

class InputChecker{
  public:
    InputChecker() = delete;
    static uint8_t getMinLength();
    static bool isTooShort(const std::string& input);
    static bool isContainsForbidenChars(const std::string& input);
    static bool isStartsByNumber(const std::string& input);
};

#endif // INPUTCHECKER_H
