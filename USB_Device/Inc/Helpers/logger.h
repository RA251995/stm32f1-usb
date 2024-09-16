#include <stdint.h>

typedef enum {
	LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG,
} LOG_LEVEL_ENUM;

extern LOG_LEVEL_ENUM systemLogLevel;

void logError(char const *const format, ...);
void logInfo(char const *const format, ...);
void logDebug(char const *const format, ...);
void logDebugArray(char const *const label, void const *array,
		uint16_t const len);
