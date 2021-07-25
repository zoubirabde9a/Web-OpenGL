#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <glm\glm.hpp>
#include <vector>

inline glm::vec2 rotatePoint(const glm::vec2& pos, float angle) {
    glm::vec2 newv;
    newv.x = pos.x * cos(angle) - pos.y * sin(angle);
    newv.y = pos.x * sin(angle) + pos.y * cos(angle);
    return newv;
}
template < typename T >
inline T max(T a, T b) {
    return a * (a > b) + b * (b >= a);
}
template < typename T, typename C >
inline T max(T a, C b) {
    return a * (a > b) + b * (b >= a);
}
template < typename T >
inline T max(T a, T b, T c) {
    T maxbc = max(b, c);
    return a * (a > maxbc) + maxbc * (maxbc >= a);
}
template < typename T >
inline T min(T a, T b) {
    return b * (a > b) + a * (b >= a);
}
template < typename T, typename C >
inline T min(T a, C b) {
    return b * (a > b) + a * (b >= a);
}
template < typename T >
inline T min(T a, T b, T c) {
    T minbc = min(b, c);
    return minbc * (a > minbc) + a * (minbc >= a);
}
template <typename T>
inline int signe(T n) {
    if (n >= 0) {
        return 1;
    }
    else return -1;
}
template < typename T>
inline T abs(T x) {
    return x * (x >= 0) + -x * (x < 0);
}
template <typename T >
int find(const T& value, const std::vector < T >& container) {
    bool found = false;
    int i = 0;
    for (; i < container.size() && !found; i++) {
        found = container[i] == value;
    }
    return !found * -1 + found * (i - 1);
}
extern glm::vec2 adjustPosition(const glm::vec2 &pos, const glm::ivec2 &dims, const glm::vec2 &startingPosition);
extern glm::ivec2 adjustDimentions(const glm::ivec2 &dimentions, const glm::ivec2 &dims);
extern glm::vec4 adjustRectangle(const glm::vec4 &uvs);
extern std::string getPath(const std::string &s);
extern bool getExtension(const std::string& s, std::string &extension);
extern bool hasExtension(const std::string& s);
extern void toString(std::string& buffer, int number);
extern void toCString(char* buffer, size_t maxSize, int number);
extern void toCStringU(char* buffer, size_t maxSize, size_t number);
extern void toCStringUL(char* buffer, size_t maxSize, unsigned long number);
extern void toCString(char* buffer, size_t maxSize, int number, size_t& outSize);
extern void toCStringU(char* buffer, size_t maxSize, size_t number, size_t& outSize);
extern void toCStringUL(char* buffer, size_t maxSize, unsigned long number, size_t& outSize);
extern void toString(std::string& buffer, float number, size_t precision = 5u);
extern double toDouble(const std::string& string);
extern float toFloat(const std::string& string);
extern int toInteger(const std::string& string);
extern double toDouble(const std::string& string, size_t i, size_t j);
extern float toFloat(const std::string& string, size_t i, size_t j);
extern int toInteger(const std::string& string, size_t i, size_t j);



#endif // UTILS_H
