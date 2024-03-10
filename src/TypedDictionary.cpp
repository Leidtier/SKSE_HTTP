#include <TypedDictionary.h>

namespace SKSE_HTTP_TypedDictionary
{
    //single values
    TypedValueString::TypedValueString(std::string value){ _value = value; }
    std::string TypedValueString::getValue(){ return _value; }
    std::string TypedValueString::getTypeName(){ return "string"; }

    TypedValueInt::TypedValueInt(int value){ _value = value; }
    int TypedValueInt::getValue(){ return _value; }
    std::string TypedValueInt::getTypeName() { return "int"; }

    TypedValueFloat::TypedValueFloat(float value){ _value = value; }
    float TypedValueFloat::getValue(){ return _value; }
    std::string TypedValueFloat::getTypeName() { return "float"; }

    TypedValueBool::TypedValueBool(bool value){ _value = value; }
    bool TypedValueBool::getValue(){ return _value; }
    std::string TypedValueBool::getTypeName() { return "bool"; }

    TypedValueNestedDictionary::TypedValueNestedDictionary(int value){ _value = value; }
    int TypedValueNestedDictionary::getValue(){ return _value; }
    std::string TypedValueNestedDictionary::getTypeName() { return "NestedDictionary"; }

    //Arrays
    TypedValueStringArray::TypedValueStringArray(std::vector<std::string> value){ _value = value; }
    std::vector<std::string> TypedValueStringArray::getValue(){ return _value; }
    std::string TypedValueStringArray::getTypeName(){ return "stringArray"; }

    TypedValueIntArray::TypedValueIntArray(std::vector<int> value){ _value = value; }
    std::vector<int> TypedValueIntArray::getValue(){ return _value; }
    std::string TypedValueIntArray::getTypeName() { return "intArray"; }

    TypedValueFloatArray::TypedValueFloatArray(std::vector<float> value){ _value = value; }
    std::vector<float> TypedValueFloatArray::getValue(){ return _value; }
    std::string TypedValueFloatArray::getTypeName() { return "floatArray"; }

    TypedValueBoolArray::TypedValueBoolArray(std::vector<bool> value){ _value = value; }
    std::vector<bool> TypedValueBoolArray::getValue(){ return _value; }
    std::string TypedValueBoolArray::getTypeName() { return "boolArray"; }

    TypedValueNestedDictionaryArray::TypedValueNestedDictionaryArray(std::vector<int> value){ _value = value; }
    std::vector<int> TypedValueNestedDictionaryArray::getValue(){ return _value; }
    std::string TypedValueNestedDictionaryArray::getTypeName() { return "NestedDictionaryArray"; }


    TypedDictionary::TypedDictionary(){
    }
    void TypedDictionary::setString(std::string name, std::string value){
        _dicElements[name] = std::make_shared<TypedValueString>(value);
    } 
    void TypedDictionary::setInt(std::string name, int value){
        _dicElements[name] = std::make_shared<TypedValueInt>(value);
    }
    void TypedDictionary::setFloat(std::string name, float value){
        _dicElements[name] = std::make_shared<TypedValueFloat>(value);
    }
    void TypedDictionary::setBool(std::string name, bool value){
        _dicElements[name] = std::make_shared<TypedValueBool>(value);
    }
    void TypedDictionary::setNestedDictionary(std::string name, int id){
        _dicElements[name] = std::make_shared<TypedValueNestedDictionary>(id);
    }

    void TypedDictionary::setStringArray(std::string name, std::vector<std::string> value){
        _dicElements[name] = std::make_shared<TypedValueStringArray>(value);
    } 
    void TypedDictionary::setIntArray(std::string name, std::vector<int> value){
        _dicElements[name] = std::make_shared<TypedValueIntArray>(value);
    }
    void TypedDictionary::setFloatArray(std::string name, std::vector<float> value){
        _dicElements[name] = std::make_shared<TypedValueFloatArray>(value);
    }
    void TypedDictionary::setBoolArray(std::string name, std::vector<bool> value){
        _dicElements[name] = std::make_shared<TypedValueBoolArray>(value);
    }
    void TypedDictionary::setArrayOfNestedDictionaries(std::string name, std::vector<int> value){
        _dicElements[name] = std::make_shared<TypedValueNestedDictionaryArray>(value);
    }

    bool TypedDictionary::hasElement(std::string name){
        return _dicElements.contains(name);
    }

    std::string TypedDictionary::getString(std::string name){
        std::shared_ptr<TypedValue> valueObject = _dicElements[name];
        std::shared_ptr<TypedValueString> realObject = std::static_pointer_cast<TypedValueString>(valueObject);
        return realObject->getValue();
    }
    int TypedDictionary::getInt(std::string name){
        return (std::static_pointer_cast<TypedValueInt>(_dicElements[name]))->getValue();
    }
    float TypedDictionary::getFloat(std::string name){
        return (std::static_pointer_cast<TypedValueFloat>(_dicElements[name]))->getValue();
    }
    bool TypedDictionary::getBool(std::string name){
        return (std::static_pointer_cast<TypedValueBool>(_dicElements[name]))->getValue();
    }    
    int TypedDictionary::getNestedDictionary(std::string name){
        return (std::static_pointer_cast<TypedValueNestedDictionary>(_dicElements[name]))->getValue();
    }

    std::vector<std::string> TypedDictionary::getStringArray(std::string name){
        return std::static_pointer_cast<TypedValueStringArray>(_dicElements[name])->getValue();
    }
    std::vector<int> TypedDictionary::getIntArray(std::string name){
        return (std::static_pointer_cast<TypedValueIntArray>(_dicElements[name]))->getValue();
    }
    std::vector<float> TypedDictionary::getFloatArray(std::string name){
        return (std::static_pointer_cast<TypedValueFloatArray>(_dicElements[name]))->getValue();
    }
    std::vector<bool> TypedDictionary::getBoolArray(std::string name){
        return (std::static_pointer_cast<TypedValueBoolArray>(_dicElements[name]))->getValue();
    }
    std::vector<int> TypedDictionary::getArrayOfNestedDictionaries(std::string name){
        return (std::static_pointer_cast<TypedValueNestedDictionaryArray>(_dicElements[name]))->getValue();
    }

}