scriptName SKSE_HTTP hidden

function sendLocalhostHttpRequest(int typedDictionaryHandle, int port, string route) global native

function raiseOnHttpReplyReceived(int typedDictionaryHandle) global
    int handle = ModEvent.Create("SKSE_HTTP_OnHttpReplyReceived")
    if (handle)
        ModEvent.PushInt(handle, typedDictionaryHandle)
        ModEvent.Send(handle)
    endIf    
endFunction

; Dictionary

Int function createDictionary() global native

;/  Returns the value associated with the @key. If not, returns @default value
/;
Int function getInt(Int object, String key, Int default=0) global native
Float function getFloat(Int object, String key, Float default=0.0) global native
String function getString(Int object, String key, String default="") global native
Bool function getBool(Int object, String key, Bool default=false) global native
Int function getNestedDictionary(Int object, String key, Int default=0) global native

;/  Inserts @key: @value pair. Replaces existing pair with the same @key
/;
function setInt(Int object, String key, Int value) global native
function setFloat(Int object, String key, Float value) global native
function setString(Int object, String key, String value) global native
function setBool(Int object, String key, Bool value) global native
function setNestedDictionary(Int object, String key, Int value) global native

;/  Returns true, if the container has @key: value pair
/;
Bool function hasKey(Int object, String key) global native

;/  Returns type of the value associated with the @key.
    0 - no value, 1 - none, 2 - int, 3 - float, 4 - form, 5 - object, 6 - string
/;
Int function valueType(Int object, String key) global native