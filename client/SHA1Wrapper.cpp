#include "SHA1Wrapper.h"
#include <memory>
#include "SHA1.hpp"

std::string sha_1::hash(const std::string& value)
{
    auto checksum = std::make_shared<SHA1>();
    checksum->update(value);
    return checksum->final();
}
