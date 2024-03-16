#include <SKSE_HTTP_TypedDictionary.h>

namespace SKSE_HTTP_TypedDictionary
{
    std::unordered_map<int, std::shared_ptr<TypedDictionary>> dicNestedDictionariesValues;
    int counter = 0;
   
    int createDictionary(){
        int currentCounter = counter;
        dicNestedDictionariesValues[currentCounter] = std::make_shared<TypedDictionary>();
        counter++;
        return currentCounter;
    };

    void clearAll() {
        for (auto& [key, value] : dicNestedDictionariesValues) {
            value.reset();
        }
        dicNestedDictionariesValues.clear(); 
    };

    // Returns the value associated with the @key. If not, returns @default value
    std::string getString(int object, std::string key, std::string defaultValue){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getString(key);
        }
        return defaultValue;
    };
    int getInt(int object, std::string key, int defaultValue){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getInt(key);
        }
        return defaultValue;
    };
    float getFloat(int object, std::string key, float defaultValue){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getFloat(key);
        }
        return defaultValue;
    };
    bool getBool(int object, std::string key, bool defaultValue){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getBool(key);
        }
        return defaultValue;
    };
    int getNestedDictionary(int object, std::string key, int defaultValue){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getNestedDictionary(key);
        }
        return defaultValue;
    };

    std::vector<std::string> getStringArray(int object, std::string key){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getStringArray(key);
        }
        return std::vector<std::string>();
    };
    std::vector<int> getIntArray(int object, std::string key){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getIntArray(key);
        }
        return std::vector<int>();
    };
    std::vector<float> getFloatArray(int object, std::string key){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getFloatArray(key);
        }
        return std::vector<float>();
    };
    std::vector<bool> getBoolArray(int object, std::string key){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getBoolArray(key);
        }
        return std::vector<bool>();
    };
    std::vector<int> getArrayOfNestedDictionaries(int object, std::string key){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key)){
            return dicNestedDictionariesValues[object]->getArrayOfNestedDictionaries(key);
        }
        return std::vector<int>();
    };

    // Inserts @key: @value pair. Replaces existing pair with the same @key

    void setString(int object, std::string key, std::string value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setString(key, value);
    }
    void setInt(int object, std::string key, int value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setInt(key, value);
    }
    void setFloat(int object, std::string key, float value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setFloat(key, value);
    }
    void setBool(int object, std::string key, bool value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setBool(key, value);
    }
    void setNestedDictionary(int object, std::string key, int value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setNestedDictionary(key, value);
    }

    void setStringArray(int object, std::string key, std::vector<std::string> value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setStringArray(key, value);
    }
    void setIntArray(int object, std::string key, std::vector<int> value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setIntArray(key, value);
    }
    void setFloatArray(int object, std::string key, std::vector<float> value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setFloatArray(key, value);
    }
    void setBoolArray(int object, std::string key, std::vector<bool> value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setBoolArray(key, value);
    }
    void setArrayOfNestedDictionaries(int object, std::string key, std::vector<int> value){
        if(dicNestedDictionariesValues.contains(object))
            dicNestedDictionariesValues[object]->setArrayOfNestedDictionaries(key, value);
    }
    //  Returns true, if the container has @key: value pair
    bool hasKey(int object, std::string key){
        if(dicNestedDictionariesValues.contains(object) && dicNestedDictionariesValues[object]->hasElement(key))
            return true;
        return false;
    }

    
}