#include "utils.h"

glm::vec2 adjustPosition(const glm::vec2 &pos, const glm::ivec2 &dims, const glm::vec2 &startingPosition) {
    glm::vec2 output;
    float offsetX = int(startingPosition.x) % dims.x + startingPosition.x - int(startingPosition.x);
    float offsetY = int(startingPosition.y) % dims.y + startingPosition.y - int(startingPosition.y);
    output.x = int((pos.x - offsetX) / dims.x) * dims.x + offsetX;
    output.y = int((pos.y - offsetY) / dims.y) * dims.y + offsetY;
    return output;
}
glm::ivec2 adjustDimentions(const glm::ivec2 &dimentions, const glm::ivec2 &dims) {
    glm::ivec2 output;
    output.x = (abs(dimentions.x) / dims.x + 1) * dims.x * signe(dimentions.x);
    output.y = (abs(dimentions.y) / dims.y + 1) * dims.y * signe(dimentions.y);
    return output;
}
glm::vec4 adjustRectangle(const glm::vec4 &uvs) {
    glm::vec4 output = uvs;
    bool temp = output.z < 0;
    output.z = -output.z * temp + output.z * !temp;
    output.x -= output.z * temp;
    temp = output.w < 0;
    output.w = -output.w * temp + output.w * !temp;
    output.y -= output.w * temp;
    return output;
}
std::string getPath(const std::string &s) {
    std::string output;
    size_t result = s.rfind("/");
    if (result != std::string::npos) {
        output = s.substr(0, result + 1);
    }
    return output;
}
bool getExtension(const std::string& s, std::string &extension) {
    size_t result = s.rfind(".");
    if (result != std::string::npos) {
        extension = s.substr(result + 1, s.size() - result);
        return true;
    }
    extension.clear();
    return false;
}
bool hasExtension(const std::string& s) {
    size_t result = s.rfind(".");
    if (result != std::string::npos) {
        return true;
    }
    return false;
}
void toString(std::string& buffer, int number) {
    if (number == 0) {
        buffer = "0";
        return;
    }
    int n1 = number * (number >= 0) + -number * (number < 0);
    int n2 = n1;
    size_t i = 0;
    while (n1 != 0)
    {
        i++;
        n1 /= 10;
    }
    buffer.resize(i + (number < 0));
    i = 0;
    while (n2 != 0)
    {
        buffer[buffer.size() - 1 - i] = (n2 % 10) + '0';
        i++;
        n2 /= 10;
    }

    if (number < 0) {
        buffer[buffer.size() - 1 - i] = '-';
        i++;
    }
}
void toCString(char* buffer, size_t maxSize, int number) {
    if (maxSize == 0) {
        return;
    }
    int n1 = number * (number >= 0) + -number * (number < 0);
    size_t i = 0;
    while (n1 != 0 && i < maxSize - 1)
    {
        buffer[maxSize - 1 - i] = (n1 % 10) + '0';
        i++;
        n1 /= 10;
    }

    if (number < 0) {
        buffer[maxSize - 1 - i] = '-';
        i++;
    }
    for (; i < maxSize; i++) {
        buffer[maxSize - 1 - i] = '0';
    }
    buffer[maxSize] = '\0';
}
void toCStringU(char* buffer, size_t maxSize, size_t number) {
    if (maxSize == 0) {
        return;
    }
    int n1 = number;
    size_t i = 0;
    while (n1 != 0 && i < maxSize - 1)
    {
        buffer[maxSize - 1 - i] = (n1 % 10) + '0';
        i++;
        n1 /= 10;
    }
    for (; i < maxSize; i++) {
        buffer[maxSize - 1 - i] = '0';
    }
    buffer[maxSize] = '\0';
}
void toCStringUL(char* buffer, size_t maxSize, unsigned long number) {
    if (maxSize == 0) {
        return;
    }
    int n1 = number;
    size_t i = 0;
    while (n1 != 0 && i < maxSize - 1)
    {
        buffer[maxSize - 1 - i] = (n1 % 10) + '0';
        i++;
        n1 /= 10;
    }
    for (; i < maxSize; i++) {
        buffer[maxSize - 1 - i] = '0';
    }
    buffer[maxSize] = '\0';
}
void toCString(char* buffer, size_t maxSize, int number, size_t& outSize) {
    if (maxSize == 0) {
        outSize = 0;
        return;
    }
    if (number == 0) {
        outSize = 1;
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    int n1 = number * (number >= 0) + -number * (number < 0);
    int n2 = n1;
    size_t i = 0;
    while (n1 != 0 && i < maxSize - 1)
    {
        i++;
        n1 /= 10;
    }
    outSize = i + (number < 0);
    i = 0;
    while (n2 != 0 && i < outSize)
    {
        buffer[outSize - 1 - i] = (n2 % 10) + '0';
        i++;
        n2 /= 10;
    }

    if (number < 0) {
        buffer[outSize - 1 - i] = '-';
        i++;
    }
    buffer[outSize] = '\0';
}
void toCStringU(char* buffer, size_t maxSize, size_t number, size_t& outSize) {
    if (maxSize == 0) {
        outSize = 0;
        return;
    }
    if (number == 0) {
        outSize = 1;
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    int n1 = number;
    int n2 = n1;
    size_t i = 0;
    while (n1 != 0 && i < maxSize - 1)
    {
        i++;
        n1 /= 10;
    }
    outSize = i;
    i = 0;
    while (n2 != 0 && i < outSize)
    {
        buffer[outSize - 1 - i] = (n2 % 10) + '0';
        i++;
        n2 /= 10;
    }
    buffer[outSize] = '\0';
}
void toCStringUL(char* buffer, size_t maxSize, unsigned long number, size_t& outSize) {
    if (maxSize == 0) {
        outSize = 0;
        return;
    }
    if (number == 0) {
        outSize = 1;
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    int n1 = number;
    int n2 = n1;
    size_t i = 0;
    while (n1 != 0 && i < maxSize - 1)
    {
        i++;
        n1 /= 10;
    }
    outSize = i;
    i = 0;
    while (n2 != 0 && i < outSize)
    {
        buffer[outSize - 1 - i] = (n2 % 10) + '0';
        i++;
        n2 /= 10;
    }
    buffer[outSize] = '\0';
}
void toString(std::string& buffer, float number, size_t precision) {
    if (number == 0) {
        buffer = "0";
        return;
    }
    bool isNegative = (number < 0);
    int n1i = number * (number >= 0) + -number * isNegative;
    int n2i = n1i;
    float n1f = number * (number >= 0) + -number * isNegative - (float)n1i;
    float n2f = n1f;
    bool isFloat = n2f != 0;
    size_t i = 0;
    size_t j = 0;
    while (n1f != 0 && i < precision)
    {
        i++;
        n1f *= 10;
        n1f -= int(n1f);
    }
    while (n1i != 0)
    {
        j++;
        n1i /= 10;
    }
    if (n2i == 0) {
        j = 1;
        buffer.resize(i + j + isNegative + isFloat);
        buffer[isNegative] = '0';
    }
    else {
        buffer.resize(i + j + isNegative + isFloat);
    }
    i = 0;
    if (isFloat) {
        buffer[j + isNegative] = '.';
    }
    if (isNegative) {
        buffer[i] = '-';
    }
    while (n2i != 0)
    {
        buffer[j - i + isNegative - 1] = (n2i % 10) + '0';
        i++;
        n2i /= 10;
    }
    size_t nbf = 0;
    while (n2f != 0.0f && nbf < precision)
    {
        n2f *= 10.0f;
        buffer[j + isNegative + isFloat] = int(n2f) + '0';
        n2f -= int(n2f);
        j++;
        nbf++;
    }
}
double toDouble(const std::string& string) {
    double output = 0.0f;
    size_t i;
    if (string.size() > 1 && (string[0] == '-' || string[0] == '+')) {
        if (string[1] == '.') {
            if (string.size() > 2) {
                float multiplier = 0.1f;
                for (i = 2; i < string.size(); i++) {
                    output += (string[i] - '0') * multiplier;
                    multiplier *= 0.1f;
                }
                if (string.size() > 0) {
                    output *= -1 * (string[0] == '-') + (string[0] != '-');
                }
                return output;
            }
            else {
                i = 2;
            }
        }
        else {
            output += string[1] - '0';
            i = 2;
        }
    }
    else if (string.size() > 1 && string[0] == '.') {
        float multiplier = 0.1f;
        for (i = 1; i < string.size(); i++) {
            output += (string[i] - '0') * multiplier;
            multiplier *= 0.1f;
        }
        if (string.size() > 0) {
            output *= -1 * (string[0] == '-') + (string[0] != '-');
        }
        return output;
    }
    else if (string.size() > 0) {
        output += string[0] - '0';
        i = 1;
    }
    else {
        return output;
    }

    for (; i < string.size() && string[i] != '.'; i++) {
        output *= 10;
        output += string[i] - '0';
    }
    if (i < string.size() && string[i] == '.')i++;
    float multiplier = 0.1f;
    for (; i < string.size(); i++) {
        output += (string[i] - '0') * multiplier;
        multiplier *= 0.1f;
    }
    if (string.size() > 0) {
        output *= -1 * (string[0] == '-') + (string[0] != '-');
    }
    return output;
}
float toFloat(const std::string& string) {
    float output = 0.0f;
    size_t i;
    if (string.size() > 1 && (string[0] == '-' || string[0] == '+')) {
        if (string[1] == '.') {
            if (string.size() > 2) {
                float multiplier = 0.1f;
                for (i = 2; i < string.size(); i++) {
                    output += (string[i] - '0') * multiplier;
                    multiplier *= 0.1f;
                }
                if (string.size() > 0) {
                    output *= -1 * (string[0] == '-') + (string[0] != '-');
                }
                return output;
            }
            else {
                i = 2;
            }
        }
        else {
            output += string[1] - '0';
            i = 2;
        }
    }
    else if (string.size() > 1 && string[0] == '.') {
        float multiplier = 0.1f;
        for (i = 1; i < string.size(); i++) {
            output += (string[i] - '0') * multiplier;
            multiplier *= 0.1f;
        }
        if (string.size() > 0) {
            output *= -1 * (string[0] == '-') + (string[0] != '-');
        }
        return output;
    }
    else if (string.size() > 0) {
        output += string[0] - '0';
        i = 1;
    }
    else {
        return output;
    }

    for (; i < string.size() && string[i] != '.'; i++) {
        output *= 10;
        output += string[i] - '0';
    }
    if (i < string.size() && string[i] == '.')i++;
    float multiplier = 0.1f;
    for (; i < string.size(); i++) {
        output += (string[i] - '0') * multiplier;
        multiplier *= 0.1f;
    }
    if (string.size() > 0) {
        output *= -1 * (string[0] == '-') + (string[0] != '-');
    }
    return output;
}
int toInteger(const std::string& string) {
    int output = 0;
    size_t i;
    if (string.size() > 0 && string[0] != '-' && string[0] != '+') {
        output += (string[0] - '0');
        i = 1;
    }
    else if (string.size() > 1) {
        output += (string[1] - '0');
        i = 2;
    }
    else return output;
    for (; i < string.size(); i++) {
        output *= 10;
        output += string[i] - '0';
    }
    if (string.size() > 0) {
        output *= -1 * (string[0] == '-') + (string[0] != '-');
    }
    return output;
}
double toDouble(const std::string& string, size_t i, size_t j) {
    double output = 0.0f;
    size_t k;
    if (string.size() > i + 1 && i + 1 <= j && (string[i] == '-' || string[i] == '+')) {
        if (string[i + 1] == '.') {
            if (string.size() > i + 2 && i + 2 <= j) {
                float multiplier = 0.1f;
                for (k = i + 2; k < string.size() && k < j; k++) {
                    output += (string[k] - '0') * multiplier;
                    multiplier *= 0.1f;
                }
                if (string.size() > i) {
                    output *= -1 * (string[i] == '-') + (string[i] != '-');
                }
                return output;
            }
            else {
                k = i + 2;
            }
        }
        else {
            output += string[i + 1] - '0';
            k = i + 2;
        }
    }
    else if (string.size() > i + 1 && string[i] == '.') {
        float multiplier = 0.1f;
        for (k = i + 1; i < string.size() && k < j; k++) {
            output += (string[k] - '0') * multiplier;
            multiplier *= 0.1f;
        }
        if (string.size() > i) {
            output *= -1 * (string[i] == '-') + (string[i] != '-');
        }
        return output;
    }
    else if (string.size() > i) {
        output += string[i] - '0';
        k = i + 1;
    }
    else {
        return output;
    }
    for (; k < string.size() && k < j && string[k] != '.'; k++) {
        output *= 10;
        output += string[k] - '0';
    }
    if (k < string.size() && k < j && string[k] == '.')k++;
    float multiplier = 0.1f;
    for (; k < string.size() && k < j; k++) {
        output += (string[k] - '0') * multiplier;
        multiplier *= 0.1f;
    }
    if (string.size() > i) {
        output *= -1 * (string[i] == '-') + (string[i] != '-');
    }
    return output;
}
float toFloat(const std::string& string, size_t i, size_t j) {
    float output = 0.0f;
    size_t k;
    if (string.size() > i + 1 && i + 1 <= j && (string[i] == '-' || string[i] == '+')) {
        if (string[i + 1] == '.') {
            if (string.size() > i + 2 && i + 2 <= j) {
                float multiplier = 0.1f;
                for (k = i + 2; k < string.size() && k < j; k++) {
                    output += (string[k] - '0') * multiplier;
                    multiplier *= 0.1f;
                }
                if (string.size() > i) {
                    output *= -1 * (string[i] == '-') + (string[i] != '-');
                }
                return output;
            }
            else {
                k = i + 2;
            }
        }
        else {
            output += string[i + 1] - '0';
            k = i + 2;
        }
    }
    else if (string.size() > i + 1 && string[i] == '.') {
        float multiplier = 0.1f;
        for (k = i + 1; i < string.size() && k < j; k++) {
            output += (string[k] - '0') * multiplier;
            multiplier *= 0.1f;
        }
        if (string.size() > i) {
            output *= -1 * (string[i] == '-') + (string[i] != '-');
        }
        return output;
    }
    else if (string.size() > i) {
        output += string[i] - '0';
        k = i + 1;
    }
    else {
        return output;
    }
    for (; k < string.size() && k < j && string[k] != '.'; k++) {
        output *= 10;
        output += string[k] - '0';
    }
    if (k < string.size() && k < j && string[k] == '.')k++;
    float multiplier = 0.1f;
    for (; k < string.size() && k < j; k++) {
        output += (string[k] - '0') * multiplier;
        multiplier *= 0.1f;
    }
    if (string.size() > i) {
        output *= -1 * (string[i] == '-') + (string[i] != '-');
    }
    return output;
}
