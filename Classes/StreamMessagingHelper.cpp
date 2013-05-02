//
//  StreamMessagingHelper.cpp
//  CocosPlayer
//
//  Created by Rohan Kuruvilla on 02/05/2013.
//
//

#include "StreamMessagingHelper.h"
#include "ScriptingCore.h"

void sendLogMsg(const char *msg);
char *_cocosplayer_log_buf = NULL;

void cocosPlayerNetworkLog(const char * pszFormat, ...)
{
    printf("Cocos2d: ");
    char szBuf[kMaxLogLen+1] = {0};
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");
    sendLogMsg(szBuf);
}

void cocosplayer_js_log(const char *format, ...) {
    if (_cocosplayer_log_buf == NULL) {
        _cocosplayer_log_buf = (char *)calloc(sizeof(char), kMaxLogLen+1);
    }
    va_list vl;
    va_start(vl, format);
    int len = vsnprintf(_cocosplayer_log_buf, kMaxLogLen, format, vl);
    va_end(vl);
    if (len) {
        cocosPlayerNetworkLog("CocosPlayer: %s\n", _cocosplayer_log_buf);
    }
}

JSBool JS_cocosPlayerLog(JSContext* cx, uint32_t argc, jsval *vp)
{
    if (argc > 0) {
        JSString *string = NULL;
        JS_ConvertArguments(cx, argc, JS_ARGV(cx, vp), "S", &string);
        if (string) {
            JSStringWrapper wrapper(string);
            cocosplayer_js_log((char *)wrapper);
        }
    }
    return JS_TRUE;
}


void register_CocosPlayer(JSContext* cx, JSObject* obj) {
	// first, try to get the ns
	jsval nsval;
	JSObject *ns;
	JS_GetProperty(cx, obj, "cc", &nsval);
	if (nsval == JSVAL_VOID) {
		ns = JS_NewObject(cx, NULL, NULL, NULL);
		nsval = OBJECT_TO_JSVAL(ns);
		JS_SetProperty(cx, obj, "cc", &nsval);
	} else {
		JS_ValueToObject(cx, nsval, &ns);
	}
	obj = ns;
    
    JS_DefineFunction(cx, obj, "_cocosplayerLog", JS_cocosPlayerLog, 2, JSPROP_READONLY | JSPROP_PERMANENT);
}
