#include "Log.h"

#include <cstdarg>
#include <cstdio>

void Log::info(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);
    printf("LOG_INFO : vk-framework : ");
    vprintf(pMessage, varArgs);
    printf("\n");

    va_end(varArgs);
}

void Log::error(const char *pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);

    printf("LOG_ERROR : vk-framework : ");
    vprintf(pMessage, varArgs);
    printf("\n");

    va_end(varArgs);
}

static void warn(const char* pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);

    printf("LOG_WARN : vk-framework : ");
    vprintf(pMessage, varArgs);
    printf("\n");

    va_end(varArgs);
}

static void debug(const char* pMessage, ...) {
    va_list varArgs;
    va_start(varArgs, pMessage);

    printf("LOG_DEBUG : vk-framework : ");
    vprintf(pMessage, varArgs);
    printf("\n");

    va_end(varArgs);
}
