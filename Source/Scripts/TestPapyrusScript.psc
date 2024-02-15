scriptName TestPapyrusScript extends Quest
{!BIND}
Import SKSE_HTTP
; Import SKSE_HTTP_TypedDictionary

int H_KEY = 35

event OnHttpReplyReceived(int typedDictionaryHandle)    
    string replyType = SKSE_HTTP.getString(typedDictionaryHandle, "replyType", "Error: No reply type received")
    string text = replyType + "\n"
    if(replyType == "conversationResponse")
        string npc = SKSE_HTTP.getString(typedDictionaryHandle, "npc", "Error: No npc to say stuff")
        bool npcLikesPlayer = SKSE_HTTP.getBool(typedDictionaryHandle, "npcLikesPlayer", false)
        float testFloat = SKSE_HTTP.getFloat(typedDictionaryHandle, "testFloat", 1)

        int contextHandle = SKSE_HTTP.getNestedDictionary(typedDictionaryHandle, "context", 0)
        string current_location = SKSE_HTTP.getString(contextHandle, "location", "Only the gods know where")
        int time = SKSE_HTTP.getInt(contextHandle, "time", 0)

        text += "npc: " + npc + "\n"
        text += "npcLikesPlayer: " + npcLikesPlayer + "\n"
        text += "testFloat: " + testFloat + "\n"
        text += "location: " + current_location + "\n"
        text += "time: " + time + "\n"
    endIf
    
    Debug.MessageBox(text)
endEvent

event OnInit()
    Debug.MessageBox("OnInit for TestPapyrusScript triggered")
    RegisterForModEvent("SKSE_HTTP_OnHttpReplyReceived","OnHttpReplyReceived")
    RegisterForKey(H_KEY)
endEvent

event OnKeyDown(int keyCode)
    if keyCode == H_KEY
        Debug.Notification("OnKeyDown for TestPapyrusScript triggered")
        DoSomething()
    endIf
endEvent

function DoSomething()
    int handle = SKSE_HTTP.createDictionary()
    SKSE_HTTP.setString(handle, "requestType", "startConversation")
    ;Debug.MessageBox(SKSE_HTTP.getString(handle,"requestType", "Did not work"))
    SKSE_HTTP.setString(handle, "npc", "Lydia")
    SKSE_HTTP.setBool(handle, "isInCombatWithPlayer", false)
    SKSE_HTTP.setFloat(handle, "testFloat", 0.123)

    int handleForContext = SKSE_HTTP.createDictionary()
    SKSE_HTTP.setString(handleForContext, "location", "Dragonsreach")
    SKSE_HTTP.setInt(handleForContext, "time", 1328)
    SKSE_HTTP.setNestedDictionary(handle,"context",handleForContext)

    SKSE_HTTP.sendLocalhostHttpRequest(handle,5000,"mantella")    
endFunction