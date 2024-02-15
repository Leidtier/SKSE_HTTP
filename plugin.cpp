#include <stdio.h>
#include <cpr\cpr.h>
#include <SKSE_HTTP_TypedDictionary.h>
#include <nlohmann\json.hpp>

using json = nlohmann::json;
using namespace SKSE_HTTP_TypedDictionary;

std::ostringstream os;
bool resultAlreadySent = false;

void getJsonFromHandle(int typedDictionaryHandle, json& jsonToUse)
{
    std::shared_ptr<TypedDictionary> dict = SKSE_HTTP_TypedDictionary::dicNestedDictionariesValues[typedDictionaryHandle];
    for (auto& [key, value]: dict->_dicElements)
    {
        
        if (value->getTypeName() == "string")
            jsonToUse[key] = dict->getString(key);
        else if (value->getTypeName() == "int")
            jsonToUse[key] = dict->getInt(key);
        else if (value->getTypeName() == "float")
            jsonToUse[key] = dict->getFloat(key);
        else if (value->getTypeName() == "bool")
            jsonToUse[key] = dict->getBool(key);
        else if (value->getTypeName() == "NestedDictionary")
        {
            int handle = dict->getInt(key);
            getJsonFromHandle(handle, jsonToUse[key]);
        }
    }
};

int generateDictionaryFromJson(json jsonToUse)
{
    int handle = SKSE_HTTP_TypedDictionary::createDictionary();
    for (auto& el : jsonToUse.items())
    {
        if (el.value().is_string())
            SKSE_HTTP_TypedDictionary::setString(handle, el.key(), el.value());
        else if (el.value().is_number_integer())
            SKSE_HTTP_TypedDictionary::setInt(handle, el.key(), el.value());
        else if (el.value().is_number_float())
            SKSE_HTTP_TypedDictionary::setFloat(handle, el.key(), el.value());
        else if (el.value().is_boolean())
            SKSE_HTTP_TypedDictionary::setBool(handle, el.key(), el.value());
        else if (el.value().is_object())
        {
            json nested = el.value();
            int subHandle = generateDictionaryFromJson(nested);
            SKSE_HTTP_TypedDictionary::setNestedDictionary(handle, el.key(), subHandle);
        }            
    }
    return handle;
};

bool writeCallback(std::string data, intptr_t userdata)
{
    os << data;
    return true;
};

int sendHttpRequestResultToSkyrimEvent(std::string completeReply)
{
    json reply = json::parse(completeReply);
    int newHandle = generateDictionaryFromJson(reply);
    // int newHandle = 1;
    auto* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
    auto eventArgs = RE::MakeFunctionArguments((int)newHandle);
    RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
    RE::BSFixedString Skse_Http = "SKSE_HTTP";
    RE::BSFixedString OnHttpReplyReceived = "raiseOnHttpReplyReceived";
    vm->DispatchStaticCall(Skse_Http, OnHttpReplyReceived, eventArgs, callback);
    return 0;
};

bool progressCallback(cpr::cpr_off_t downloadTotal, cpr::cpr_off_t downloadNow, cpr::cpr_off_t uploadTotal, cpr::cpr_off_t uploadNow, intptr_t userdata)
{
    if (!resultAlreadySent && downloadTotal > 0 && downloadNow == downloadTotal)
    {
        sendHttpRequestResultToSkyrimEvent(os.str());
        resultAlreadySent = true;
    }
    return true;
};

void sendLocalhostHttpRequest(RE::StaticFunctionTag*, int typedDictionaryHandle, int port, std::string route)
{
    json newJson;
    getJsonFromHandle(typedDictionaryHandle, newJson);
    std::string textToSend = newJson.dump();
    // std::string textToSend = "Hello";
	std::string url = "http://localhost:" + std::to_string(port) + "/" + route;
    resultAlreadySent = false;
    os.str("");
    os.clear();
    cpr::PostAsync(cpr::Url{ url },
            cpr::ProgressCallback{ progressCallback },
            cpr::WriteCallback{ writeCallback },
            cpr::Authentication{ "user", "pass", cpr::AuthMode::BASIC },
            cpr::Header{{"Content-Type", "application/json"}},
            cpr::Header{{"accept", "application/json"}},
            cpr::Body{textToSend});
};

int createDictionaryRelay(RE::StaticFunctionTag*){
    return createDictionary();
}

// Returns the value associated with the @key. If not, returns @default value
std::string getStringRelay(RE::StaticFunctionTag*,int object, std::string key, std::string defaultValue){
    return getString(object, key, defaultValue);
}
int getIntRelay(RE::StaticFunctionTag*,int object, std::string key, int defaultValue){
    return getInt(object, key, defaultValue);
}
float getFloatRelay(RE::StaticFunctionTag*,int object, std::string key, float defaultValue){
    return getFloat(object, key, defaultValue);
}
bool getBoolRelay(RE::StaticFunctionTag*,int object, std::string key, bool defaultValue){
    return getBool(object, key, defaultValue);
}
int getNestedDictionaryRelay(RE::StaticFunctionTag*,int object, std::string key, int defaultValue){
    return getNestedDictionary(object, key, defaultValue);
}

// Inserts @key: @value pair. Replaces existing pair with the same @key

void setStringRelay(RE::StaticFunctionTag*,int object, std::string key, std::string value){
    setString(object, key, value);
}
void setIntRelay(RE::StaticFunctionTag*,int object, std::string key, int value){
    setInt(object, key, value);
}
void setFloatRelay(RE::StaticFunctionTag*,int object, std::string key, float value){
    setFloat(object, key, value);
}
void setBoolRelay(RE::StaticFunctionTag*,int object, std::string key, bool value){
    setBool(object, key, value);
}
void setNestedDictionaryRelay(RE::StaticFunctionTag*,int object, std::string key, int value){
    setNestedDictionary(object, key, value);
}

//  Returns true, if the container has @key: value pair
bool hasKeyRelay(RE::StaticFunctionTag*,int object, std::string key){
    return hasKey(object, key);
}


bool Bind(RE::BSScript::IVirtualMachine* vm) {
    std::string className = "SKSE_HTTP";
    vm->RegisterFunction("sendLocalhostHttpRequest",className,sendLocalhostHttpRequest);

    vm->RegisterFunction("createDictionary",className, createDictionaryRelay);

    vm->RegisterFunction("getString",className,getStringRelay);
    vm->RegisterFunction("getInt",className,getIntRelay);
    vm->RegisterFunction("getFloat",className,getFloatRelay);
    vm->RegisterFunction("getBool",className,getBoolRelay);
    vm->RegisterFunction("getNestedDictionary",className, getNestedDictionaryRelay);

    vm->RegisterFunction("setString",className,setStringRelay);
    vm->RegisterFunction("setInt",className,setIntRelay);
    vm->RegisterFunction("setFloat",className,setFloatRelay);
    vm->RegisterFunction("setBool",className,setBoolRelay);
    vm->RegisterFunction("setNestedDictionary",className,setNestedDictionaryRelay);

    vm->RegisterFunction("hasKey",className, hasKeyRelay);
    return true;
};

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    // We need to do *something* in the game to activate our "MessageBox" and "Notification".
    // This registers an "EventHandler" to handle whenever any object in the game is "Activated".
    // If the *player* activated the object, the name of the activated object is passed to OnPlayerActivateItem() above.
    // If you are curious about the event handler, see EventHandler.h
    // But you can also see more examples of game event handlers in the template:
    //                                                       https://github.com/SkyrimScripting/SKSE_Template_GameEvents
    SKSE::GetPapyrusInterface()->Register(Bind);
    // SKSE::GetPapyrusInterface()->Register(BindTypedDictionary);
    return true;
};