#ifndef MESSAGE_H_
#define MESSAGE_H_
#include "string.h"
#include "compatibility.h"

#ifdef DEBUG	
#define MESSAGE_DEBUG \
	(printf("[DEBUG] %s:%s:%d ", __FILE__, __FUNCTION__, __LINE__), printf)

#define MESSAGE_ERROR \
	(printf("[ERROR] %s:%s:%d ", __FILE__, __FUNCTION__, __LINE__), printf)
#else
#define MESSAGE_DEBUG
#define MESSAGE_ERROR \
	(printf("[ERROR]"), printf)
#endif
#define MESSAGE_ERROR_ABORT(...) \
	(MESSAGE_ERROR(__VA_ARGS__), abort())

#endif /*MESSAGE_H_ */
