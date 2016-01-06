#pragma once

#include "utils/LinearBuffer.h"
#include "model/RequestUtils.h"

#include <string>

enum ResponseType;

class Protocol
{
public:

    static bool IsValidMessage(LinearBuffer &buffer);

    static unsigned char GetRequestMajorAndSubType(LinearBuffer &buffer);

    static unsigned int GetRequestBodyLength(LinearBuffer &buffer);

    static void ProcessRequestBody(LinearBuffer::Static *pReqBody, unsigned char reqMajorType, unsigned char reqSubType, LinearBuffer *pRespBuffer);

    static std::string s_header, s_tail;
};
