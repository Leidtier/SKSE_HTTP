#pragma once
#include "TypedDictionary.h"

namespace SKSE_HTTP_TypedDictionary
{
    extern std::unordered_map<int, std::shared_ptr<TypedDictionary>> dicNestedDictionariesValues;
    extern int counter;

    int createDictionary();

    // Returns the value associated with the @key. If not, returns @default value
    std::string getString(int object, std::string key, std::string defaultValue);
    int getInt(int object, std::string key, int defaultValue);
    float getFloat(int object, std::string key, float defaultValue);
    bool getBool(int object, std::string key, bool defaultValue);
    int getNestedDictionary(int object, std::string key, int defaultValue);
    std::vector<std::string> getStringArray(int object, std::string key);
    std::vector<int> getIntArray(int object, std::string key);
    std::vector<float> getFloatArray(int object, std::string key);
    std::vector<bool> getBoolArray(int object, std::string key);
    std::vector<int> getArrayOfNestedDictionaries(int object, std::string key);

    // Inserts @key: @value pair. Replaces existing pair with the same @key

    void setString(int object, std::string key, std::string value);
    void setInt(int object, std::string key, int value);
    void setFloat(int object, std::string key, float value);
    void setBool(int object, std::string key, bool value);
    void setNestedDictionary(int object, std::string key, int value);
    void setStringArray(int object, std::string key, std::vector<std::string> value);
    void setIntArray(int object, std::string key, std::vector<int> value);
    void setFloatArray(int object, std::string key, std::vector<float> value);
    void setBoolArray(int object, std::string key, std::vector<bool> value);
    void setArrayOfNestedDictionaries(int object, std::string key, std::vector<int> value);

    //  Returns true, if the container has @key: value pair
    bool hasKey(int object, std::string key);
}