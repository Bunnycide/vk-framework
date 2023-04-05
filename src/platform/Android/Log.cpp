#include "Log.h"

#include <cstdarg>
#include <iostream>
#include <android/log.h>

void Log::info(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);

    __android_log_vprint(ANDROID_LOG_INFO, "AS_uB", pMessage, varArgs);

    va_end(varArgs);
}

void Log::error(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);

    __android_log_vprint(ANDROID_LOG_ERROR, "AS_uB", pMessage, varArgs);

    va_end(varArgs);
}

void Log::warn(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);

    __android_log_vprint(ANDROID_LOG_WARN, "AS_uB", pMessage, varArgs);

    va_end(varArgs);
}

void Log::debug(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);

    __android_log_vprint(ANDROID_LOG_DEBUG, "AS_uB", pMessage, varArgs);

    va_end(varArgs);
}