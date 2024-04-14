#define MESSAGE_DATA_STATIC

#include "message_data_fmt.h"

#define DEFINE_MESSAGE(textId, type, yPos, nesMessage, gerMessage, fraMessage) \
    const char _message_##textId##_nes[sizeof(nesMessage)] = { nesMessage END };

#define DEFINE_MESSAGE_NES(textId, type, yPos, nesMessage) DEFINE_MESSAGE(textId, type, yPos, nesMessage, , )

#include "assets/text/message_data.enc.h"
