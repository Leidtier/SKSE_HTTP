#include <stdio.h>
#include <cpr\cpr.h>
#include <SKSE_HTTP_TypedDictionary.h>
#include <nlohmann\json.hpp>

using json = nlohmann::json;
using namespace SKSE_HTTP_TypedDictionary;

void toLowerCase(std::string* input) {
    std::transform(input->begin(), input->end(), input->begin(), [](unsigned char c) { return std::tolower(c); });
};

json getJsonFromHandle(int typedDictionaryHandle)
{
    std::shared_ptr<TypedDictionary> dict = SKSE_HTTP_TypedDictionary::dicNestedDictionariesValues[typedDictionaryHandle];
    json jsonToUse;
    for (auto& [key, value]: dict->_dicElements)
    {
        std::string valueType = value->getTypeName();
        if (valueType == "string")
            jsonToUse[key] = dict->getString(key);
        else if (valueType == "int")
            jsonToUse[key] = dict->getInt(key);
        else if (valueType == "float")
            jsonToUse[key] = dict->getFloat(key);
        else if (valueType == "bool")
            jsonToUse[key] = dict->getBool(key);
        else if (valueType == "stringArray")
            jsonToUse[key] = dict->getStringArray(key);
        else if (valueType == "intArray")
            jsonToUse[key] = dict->getIntArray(key);
        else if (valueType == "floatArray")
            jsonToUse[key] = dict->getFloatArray(key);
        else if (valueType == "boolArray")
            jsonToUse[key] = dict->getBoolArray(key);
        else if (valueType == "NestedDictionary")
        {
            int handle = dict->getNestedDictionary(key);
            jsonToUse[key] = getJsonFromHandle(handle);
        }
        else if (valueType == "NestedDictionaryArray")
        {
            std::vector<int> handles = dict->getArrayOfNestedDictionaries(key);
            auto jsonObjects = json::array();
            size_t sizeOfHandles = handles.size();
            for (auto i = 0; i < sizeOfHandles; ++i) {
                jsonObjects.push_back(getJsonFromHandle(handles[i]));
            }
            jsonToUse[key] = jsonObjects;
        }
    }
    return jsonToUse;
};

int generateDictionaryFromJson(json jsonToUse)
{
    int handle = SKSE_HTTP_TypedDictionary::createDictionary();
    for (auto& el : jsonToUse.items())
    {
        std::string key = el.key();
        toLowerCase(&key);
        if (el.value().is_string())
            SKSE_HTTP_TypedDictionary::setString(handle, key, el.value());
        else if (el.value().is_number_integer())
            SKSE_HTTP_TypedDictionary::setInt(handle, key, el.value());
        else if (el.value().is_number_float())
            SKSE_HTTP_TypedDictionary::setFloat(handle, key, el.value());
        else if (el.value().is_boolean())
            SKSE_HTTP_TypedDictionary::setBool(handle, key, el.value());
        else if (el.value().is_object())
        {
            json nested = el.value();
            int subHandle = generateDictionaryFromJson(nested);
            SKSE_HTTP_TypedDictionary::setNestedDictionary(handle, key, subHandle);
        }
        else if (el.value().is_array())
        {
            if(std::all_of(el.value().begin(),el.value().end(), [](const json& elSub){ return elSub.is_string(); }))
                SKSE_HTTP_TypedDictionary::setStringArray(handle, key, el.value());
            else if(std::all_of(el.value().begin(),el.value().end(), [](const json& elSub){ return elSub.is_number_integer(); }))
                SKSE_HTTP_TypedDictionary::setIntArray(handle, key, el.value());
            else if(std::all_of(el.value().begin(),el.value().end(), [](const json& elSub){ return elSub.is_number_float(); }))
                SKSE_HTTP_TypedDictionary::setFloatArray(handle, key, el.value());
            else if(std::all_of(el.value().begin(),el.value().end(), [](const json& elSub){ return elSub.is_boolean(); }))
                SKSE_HTTP_TypedDictionary::setBoolArray(handle, key, el.value());
            else if(std::all_of(el.value().begin(),el.value().end(), [](const json& elSub){ return elSub.is_object(); }))
            {
                std::vector<int> handles;
                for (auto& elSub : el.value().items())
                {
                    json nested = elSub.value();
                    int subHandle = generateDictionaryFromJson(nested);
                    handles.push_back(subHandle);
                }
                SKSE_HTTP_TypedDictionary::setArrayOfNestedDictionaries(handle, key, handles);
            }
        }
                    
    }
    return handle;
};

int sendHttpRequestResultToSkyrimEvent(std::string completeReply, RE::BSFixedString papyrusFunctionToCall) {
    json reply = json::parse(completeReply);
    int newHandle = generateDictionaryFromJson(reply);
    auto* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
    auto eventArgs = RE::MakeFunctionArguments((int)newHandle);
    RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
    RE::BSFixedString Skse_Http = "SKSE_HTTP";    
    vm->DispatchStaticCall(Skse_Http, papyrusFunctionToCall, eventArgs, callback);
    return 0;
};

void postCallbackMethod(cpr::Response response)
{ 
    if (response.status_code == 200)
    {
        RE::BSFixedString onHttpReplyReceived = "raiseOnHttpReplyReceived";
        sendHttpRequestResultToSkyrimEvent(response.text, onHttpReplyReceived);
    }
    else
    {
        json jsonToUse;
        jsonToUse["SKSE_HTTP_error"] = response.error.message;
        RE::BSFixedString onHttpErrorReceived = "raiseOnHttpErrorReceived";
        sendHttpRequestResultToSkyrimEvent(jsonToUse.dump(), onHttpErrorReceived);
    }
}

void sendLocalhostHttpRequest(RE::StaticFunctionTag*, int typedDictionaryHandle, int port, std::string route, int timeout)
{
    json newJson = getJsonFromHandle(typedDictionaryHandle);
    std::string textToSend = newJson.dump();
    std::string url = "http://localhost:" + std::to_string(port) + "/" + route;
    cpr::PostCallback(postCallbackMethod,
                        cpr::Url{url},
                        cpr::ConnectTimeout {timeout},
                        cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                        cpr::Header{{"Content-Type", "application/json"}}, 
                        cpr::Header{{"accept", "application/json"}},
                        cpr::Body{textToSend});
};

int createDictionaryRelay(RE::StaticFunctionTag*) { return createDictionary(); };

// Returns the value associated with the @key. If not, returns @default value
std::string getStringRelay(RE::StaticFunctionTag*, int object, std::string key, std::string defaultValue) {
    toLowerCase(&key);
    return getString(object, key, defaultValue);
};
int getIntRelay(RE::StaticFunctionTag*, int object, std::string key, int defaultValue) {
    toLowerCase(&key);
    return getInt(object, key, defaultValue);
};
float getFloatRelay(RE::StaticFunctionTag*, int object, std::string key, float defaultValue) {
    toLowerCase(&key);
    return getFloat(object, key, defaultValue);
};
bool getBoolRelay(RE::StaticFunctionTag*, int object, std::string key, bool defaultValue) {
    toLowerCase(&key);
    return getBool(object, key, defaultValue);
};
int getNestedDictionaryRelay(RE::StaticFunctionTag*, int object, std::string key, int defaultValue) {
    toLowerCase(&key);
    return getNestedDictionary(object, key, defaultValue);
};
std::vector<std::string> getStringArrayRelay(RE::StaticFunctionTag*, int object, std::string key) {
    toLowerCase(&key);
    return getStringArray(object, key);
};
std::vector<int> getIntArrayRelay(RE::StaticFunctionTag*, int object, std::string key) {
    toLowerCase(&key);
    return getIntArray(object, key);
};
std::vector<float> getFloatArrayRelay(RE::StaticFunctionTag*, int object, std::string key) {
    toLowerCase(&key);
    return getFloatArray(object, key);
};
std::vector<bool> getBoolArrayRelay(RE::StaticFunctionTag*, int object, std::string key) {
    toLowerCase(&key);
    return getBoolArray(object, key);
};
std::vector<int> getNestedDictionariesArrayRelay(RE::StaticFunctionTag*, int object, std::string key) {
    toLowerCase(&key);
    return getArrayOfNestedDictionaries(object, key);
};

// Inserts @key: @value pair. Replaces existing pair with the same @key

void setStringRelay(RE::StaticFunctionTag*, int object, std::string key, std::string value) {
    toLowerCase(&key);
    setString(object, key, value);
};
void setIntRelay(RE::StaticFunctionTag*, int object, std::string key, int value) {
    toLowerCase(&key);
    setInt(object, key, value);
};
void setFloatRelay(RE::StaticFunctionTag*, int object, std::string key, float value) {
    toLowerCase(&key);
    setFloat(object, key, value);
};
void setBoolRelay(RE::StaticFunctionTag*, int object, std::string key, bool value) {
    toLowerCase(&key);
    setBool(object, key, value);
};
void setNestedDictionaryRelay(RE::StaticFunctionTag*, int object, std::string key, int value) {
    toLowerCase(&key);
    setNestedDictionary(object, key, value);
};
void setStringArrayRelay(RE::StaticFunctionTag*, int object, std::string key,
                         const RE::reference_array<std::string> value) {
    toLowerCase(&key);
    std::vector<std::string> vector;
    for (int i = 0; i < value.size(); ++i) vector.push_back(value[i]);
    setStringArray(object, key, vector);
};
void setIntArrayRelay(RE::StaticFunctionTag*, int object, std::string key, const RE::reference_array<int> value) {
    toLowerCase(&key);
    std::vector<int> vector;
    for (int i = 0; i < value.size(); ++i) vector.push_back(value[i]);
    setIntArray(object, key, vector);
};
void setFloatArrayRelay(RE::StaticFunctionTag*, int object, std::string key, const RE::reference_array<float> value) {
    toLowerCase(&key);
    std::vector<float> vector;
    for (int i = 0; i < value.size(); ++i) vector.push_back(value[i]);
    setFloatArray(object, key, vector);
};
void setBoolArrayRelay(RE::StaticFunctionTag*, int object, std::string key, std::vector<bool> value) {
    toLowerCase(&key);
    std::vector<bool> vector;
    for (int i = 0; i < value.size(); ++i) vector.push_back(value[i]);
    setBoolArray(object, key, vector);
};
void setNestedDictionariesArrayRelay(RE::StaticFunctionTag*, int object, std::string key,
                                       const RE::reference_array<int> value) {
    toLowerCase(&key);
    std::vector<int> vector;
    for (int i = 0; i < value.size(); ++i) vector.push_back(value[i]);
    setArrayOfNestedDictionaries(object, key, vector);
};


//  Returns true, if the container has @key: value pair
bool hasKeyRelay(RE::StaticFunctionTag*, int object, std::string key) { return hasKey(object, key); };


bool Bind(RE::BSScript::IVirtualMachine* vm) {
    std::string className = "SKSE_HTTP";
    vm->RegisterFunction("sendLocalhostHttpRequest", className, sendLocalhostHttpRequest);

    vm->RegisterFunction("createDictionary", className, createDictionaryRelay);

    vm->RegisterFunction("getString", className, getStringRelay);
    vm->RegisterFunction("getInt", className, getIntRelay);
    vm->RegisterFunction("getFloat", className, getFloatRelay);
    vm->RegisterFunction("getBool", className, getBoolRelay);
    vm->RegisterFunction("getNestedDictionary", className, getNestedDictionaryRelay);
    vm->RegisterFunction("getStringArray", className, getStringArrayRelay);
    vm->RegisterFunction("getIntArray", className, getIntArrayRelay);
    vm->RegisterFunction("getFloatArray", className, getFloatArrayRelay);
    vm->RegisterFunction("getBoolArray", className, getBoolArrayRelay);
    vm->RegisterFunction("getNestedDictionariesArray", className, getNestedDictionariesArrayRelay);

    vm->RegisterFunction("setString", className, setStringRelay);
    vm->RegisterFunction("setInt", className, setIntRelay);
    vm->RegisterFunction("setFloat", className, setFloatRelay);
    vm->RegisterFunction("setBool", className, setBoolRelay);
    vm->RegisterFunction("setNestedDictionary", className, setNestedDictionaryRelay);
    vm->RegisterFunction("setStringArray", className, setStringArrayRelay);
    vm->RegisterFunction("setIntArray", className, setIntArrayRelay);
    vm->RegisterFunction("setFloatArray", className, setFloatArrayRelay);
    vm->RegisterFunction("setBoolArray", className, setBoolArrayRelay);
    vm->RegisterFunction("setNestedDictionariesArray", className, setNestedDictionariesArrayRelay);

    vm->RegisterFunction("hasKey", className, hasKeyRelay);
    return true;
};

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    SKSE::GetPapyrusInterface()->Register(Bind);
    return true;
};