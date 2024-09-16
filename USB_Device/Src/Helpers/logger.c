#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "Helpers/logger.h"
#include "stm32f1xx.h"

int _write(int file, char *ptr, int len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		ITM_SendChar(*ptr++);
	}

	return len;
}

char const* const _getLogLevelString(LOG_LEVEL_ENUM const logLevel) {
	switch (logLevel) {
	case LOG_LEVEL_ERROR:
		return "ERROR";
	case LOG_LEVEL_INFO:
		return "INFO";
	case LOG_LEVEL_DEBUG:
		return "DEBUG";
	}
	return "";
}

static void _log(LOG_LEVEL_ENUM const logLevel, char const *const format,
		va_list args) {
	if (logLevel > systemLogLevel) {
		return;
	}

	printf("[%s] ", _getLogLevelString(logLevel));
	vfprintf(stdout, format, args);
	printf("\n");
}

void logError(char const *const format, ...) {
	va_list args;
	va_start(args, format);
	_log(LOG_LEVEL_ERROR, format, args);
	va_end(args);
}

void logInfo(char const *const format, ...) {
	va_list args;
	va_start(args, format);
	_log(LOG_LEVEL_INFO, format, args);
	va_end(args);
}

void logDebug(char const *const format, ...) {
	va_list args;
	va_start(args, format);
	_log(LOG_LEVEL_DEBUG, format, args);
	va_end(args);
}

void logDebugArray(char const *const label, void const *array,
		uint16_t const len) {
	if (systemLogLevel < LOG_LEVEL_DEBUG)
		return;

	printf("[%s] %s[%d]: {", _getLogLevelString(LOG_LEVEL_DEBUG), label, len);
	for (uint16_t i = 0; i < len; i++) {
		uint8_t val = *((uint8_t*) (array + i));
		printf("0x%02X", val);

		if (i < len - 1) {
			printf(", ");
		}
	}
	printf("}\n");
}
