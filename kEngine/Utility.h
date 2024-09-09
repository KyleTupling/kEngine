#pragma once

#include <string>
#include <sstream>

namespace Utility
{
    /**
     * Converts a given value of datatype T to std::string.
     * E.g. ToString(12.3f) will return "12.3".
     *
     * @param value The data to be converted.
     * @return The std::string.
     */
    template <typename T>
    std::string ToString(T value)
    {
        std::ostringstream ss;
        ss << value;
        return ss.str();
    }
}