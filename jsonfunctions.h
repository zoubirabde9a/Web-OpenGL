#ifndef JSONCPP_H
#define JSONCPP_H
#include <json\json.h>
#include <glm\glm.hpp>
#include "vertex.h"

namespace json {
    Json::Value parseFile(const char* filePath);
    Json::Value parseBuffer(const std::string& buffer);
    std::string write(const Json::Value &root, bool styled = true);
    float getFloat(const Json::Value& value, float defaultValue);
    int getInt(const Json::Value& value, int defaultValue);
    bool getBool(const Json::Value& value, bool defaultValue);
    double getDouble(const Json::Value& value, double defaultValue);
    glm::vec2 getVec2(const Json::Value& value, const glm::vec2 &defaultValue);
    glm::vec3 getVec3(const Json::Value& value, const glm::vec3 &defaultValue);
    glm::vec4 getVec4(const Json::Value& value, const glm::vec4 &defaultValue);
    ColorRGBA8 getColor(const Json::Value& value, const ColorRGBA8 &defaultValue);
    std::string getString(const Json::Value& value, const std::string &defaultValue);
    const char* getCString(const Json::Value& value, const char* defaultValue);
    void writeToFile(const Json::Value &root, const char* filePath);
}

#endif // JSONCPP_H
