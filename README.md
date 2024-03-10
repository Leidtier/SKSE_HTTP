# SKSE HTTP
A SKSE plugin to connect to a local http server and exchange strongly typed JSON data with it from Papyrus

## Status
- [x] Call http server and receive reply
- [x] Provide a simple strongly typed dictionary
    - [x] string
    - [x] int
    - [x] float
    - [x] bool
    - [x] nested dictionaries
    - [x] arrays
- [x] Serialize and deserialize typed dictionary to and from JSON
- [x] static link all the dependencies so that they don't have to sit in Skyrim's main directory
    - [ ] libcurl
    - [ ] cpr
    - [x] nlohmann/json
- [x] Make sure this works on
    - [ ] Skyrim SE (I assume this works, but untested)
    - [x] Skyrim AE
    - [x] Skyrim VR
    - [ ] Fallout 4
    - [ ] Fallout VR

## Usage
Create a strongly typed dictionary from Papyrus and send it as json to a local http server.  
Papyrus interface strongly influenced by JContainers
```Papyrus
function CallHttpServer()
    ;create a new dictionary and get its handle
    int handle = SKSE_HTTP.createDictionary()
    ;use the handle to store the string *"startConversation"* using the key "requestType"
    SKSE_HTTP.setString(handle, "requestType", "startConversation")
    SKSE_HTTP.setString(handle, "npc", "Lydia")
    ;use the handle to store the bool *false* using the key "isInCombatWithPlayer"
    SKSE_HTTP.setBool(handle, "isInCombatWithPlayer", false)
    ;use the handle to store the float *0.123* using the key "testFloat"
    SKSE_HTTP.setFloat(handle, "testFloat", 0.123)

    ;create a second dictionary and get its handle
    int handleForContext = SKSE_HTTP.createDictionary()
    ;use the new handleForContext to store the string *"Dragonsreach"* using the key "location"
    SKSE_HTTP.setString(handleForContext, "location", "Dragonsreach")
    ;use the new handleForContext to store the int *1328* using the key "time"
    SKSE_HTTP.setInt(handleForContext, "time", 1328)
    ;set the new handleForContext as a nested dictionary under the first dictionary using the "context" key
    SKSE_HTTP.setNestedDictionary(handle,"context",handleForContext)

    ;send the dictionary handle to *http://localhost:5000/mantella*
    SKSE_HTTP.sendLocalhostHttpRequest(handle,5000,"mantella")    
endFunction
```

Receive the reply from the http server
```Papyrus
event OnInit()
    ;Register for the event *SKSE_HTTP_OnHttpReplyReceived* using a custom event
    RegisterForModEvent("SKSE_HTTP_OnHttpReplyReceived","OnHttpReplyReceived")
endEvent

;*SKSE_HTTP_OnHttpReplyReceived* provides a handle to a dictionary that contains the contents of the reply
event OnHttpReplyReceived(int typedDictionaryHandle)
    ;retrieve string from dictionary using key "replytype". The last parameter is a default that will be used if the value does not exist
    string replyType = SKSE_HTTP.getString(typedDictionaryHandle, "replytype", "Error: No reply type received")
    string npc = SKSE_HTTP.getString(typedDictionaryHandle, "npc", "Error: No npc to say stuff")
    bool npcLikesPlayer = SKSE_HTTP.getBool(typedDictionaryHandle, "npcLikesPlayer", false)
    float testFloat = SKSE_HTTP.getFloat(typedDictionaryHandle, "testFloat", 1)

    int contextHandle = SKSE_HTTP.getNestedDictionary(typedDictionaryHandle, "context", 0)
    string current_location = SKSE_HTTP.getString(contextHandle, "location", "Only the gods know where")
    int time = SKSE_HTTP.getInt(contextHandle, "time", 0)
    DoSomethingWithAllThisStuff()
endEvent

```

Based on https://github.com/SkyrimScripting/SKSE_Templates Many thanks!

