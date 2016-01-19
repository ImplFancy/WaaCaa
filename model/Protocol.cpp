#include "Protocol.h"

#include "model/Chart.h"
#include "model/Dataset.h"
#include "model/ResponseUtils.h"
#include "window/WindowService.h"
#include "window/WindowInterface.h"
#include "utils/Log.h"

#include <string>

std::string Protocol::s_header = "_WCBEGIN";
std::string Protocol::s_tail   = "_WCEND";

bool Protocol::IsValidMessage(LinearBuffer &buffer)
{
    auto len = buffer.Length();
    auto buf = buffer.Buffer();

    if (len < 14) return false;

    auto fn = [](const std::string &gold, const char *bufferToCompare) -> bool {
        auto index = 0u;
        for (const char &g : gold) {
            if (bufferToCompare[index++] != g) return false;
        }
        return true;
    };

    if (!fn(s_header, buf + 0)) return false;
    if (!fn(s_tail, buf + len - s_tail.length())) return false;

    return true;
}

unsigned int Protocol::GetRequestMajorAndSubType(LinearBuffer &buffer)
{
    unsigned char majorType = buffer.Buffer()[s_header.length() + 10];
    unsigned char subType   = buffer.Buffer()[s_header.length() + 11];
    return (majorType << 8) + subType;
}

unsigned int Protocol::GetRequestBodyLength(LinearBuffer &buffer)
{
    const char *buf = buffer.Buffer();
    auto curr = s_header.length() + 1;

    unsigned int length = buf[curr++] << 3 * 8u;
    length += buf[curr++] << 2 * 8u;
    length += buf[curr++] << 1 * 8u;
    length += buf[curr++];

    return length;
}

void Protocol::ProcessRequestBody(LinearBuffer::Static *pReqBody, unsigned char reqMajorType, unsigned char reqSubType, LinearBuffer *pRespBuffer)
{
    if (pReqBody == nullptr || pRespBuffer == nullptr) {
        Log::e() << "wtf";
        return;
    }
    // assert(p_respBuffer != nullptr);
    char respTypeChar = ALL_RIGHT;


    if (RequestMajorType::A == (RequestMajorType)reqMajorType) {
        // Type A:
        if (RequestSubTypeA::CreateOneChart == (RequestSubTypeA)reqSubType) {
            auto chartIndex = WindowService::Instance().CreateOneWindow()->ChartIndex();
            if (chartIndex > 0) {
                respTypeChar = ALL_RIGHT;
                pRespBuffer->Append(&respTypeChar, 1);
                pRespBuffer->Append((const char *)(&chartIndex), 1);
            }
            else {
                respTypeChar = CREATE_CHART_FAILED;
                pRespBuffer->Append(&respTypeChar, 1);
            }
        }
        else if (RequestSubTypeA::CloseOneChart == (RequestSubTypeA)reqSubType) {
            unsigned char chartIndex = *(pReqBody->Buffer());
            if (WindowService::Instance().CloseWindow(chartIndex)) {
                respTypeChar = ALL_RIGHT;
            }
            else {
                respTypeChar = CHART_INDEX_NOT_FOUND;
            }
            pRespBuffer->Append(&respTypeChar, 1);
        }
        else if (RequestSubTypeA::CloseAllChart == (RequestSubTypeA)reqSubType) {
            WindowService::Instance().CloseAllWindow();
            respTypeChar = ALL_RIGHT;
            pRespBuffer->Append(&respTypeChar, 1);
        }
        else {
            respTypeChar = UNKNOWN_REQUEST;
            pRespBuffer->Append(&respTypeChar, 1);
        }
    }
    else if (RequestMajorType::B == (RequestMajorType)reqMajorType) {
        // Type B:
        unsigned char chartIndex = *(pReqBody->Buffer());
        Chart *pChart = WindowService::Instance().FindByIndex(chartIndex);
        if (pChart == nullptr) {
            respTypeChar = CHART_INDEX_NOT_FOUND;
            pRespBuffer->Append(&respTypeChar, 1);
        }

        if (RequestSubTypeB::SetMaxNbDataset == (RequestSubTypeB)reqSubType) {
            pChart->SetMaxNbDataset((unsigned char)(*(pReqBody->Buffer() + 1)));
        }
        else if (RequestSubTypeB::SetViewportHoldOn == (RequestSubTypeB)reqSubType) {
            pChart->SetViewportHoldOrNot(true);
        }
        else if (RequestSubTypeB::SetViewportHoldOff == (RequestSubTypeB)reqSubType) {
            pChart->SetViewportHoldOrNot(false);
        }
        else if (RequestSubTypeB::SetViewport == (RequestSubTypeB)reqSubType) {
            float *a = (float *)(pReqBody->Buffer() + 1 + 4*0);
            float *b = (float *)(pReqBody->Buffer() + 1 + 4*1);
            float *c = (float *)(pReqBody->Buffer() + 1 + 4*2);
            float *d = (float *)(pReqBody->Buffer() + 1 + 4*3);
            pChart->SetViewport(*a, *b, *c, *d);
        }
        else {
            respTypeChar = UNKNOWN_REQUEST;
            pRespBuffer->Append(&respTypeChar, 1);
        }
    }
    else if (RequestMajorType::C == (RequestMajorType)reqMajorType) {
        // Type C:
        if (reqSubType < 0x10) {
            // Send Data
            unsigned char dimType = reqSubType;

            unsigned char chartIndex = *(pReqBody->Buffer());
            unsigned char arrangeType = *(pReqBody->Buffer() + 1);
            unsigned char eleType = *(pReqBody->Buffer() + 2);

            const char *lenP = pReqBody->Buffer() + 3;

            // remember: please use "unsigned char" for temp var, or you need to mask it (e.g. &0xFF0000, &0xFF00, cause C/C++ use "int" by default)
            unsigned char tempByte;
            unsigned int nbBytesOfDataset = 0;
            tempByte = *(lenP++); nbBytesOfDataset += (tempByte << (3 * 8u));
            tempByte = *(lenP++); nbBytesOfDataset += (tempByte << (2 * 8u));
            tempByte = *(lenP++); nbBytesOfDataset += (tempByte << (1 * 8u));
            tempByte = *(lenP++); nbBytesOfDataset += tempByte;

            const void *dataset = (const void *)(lenP++);


            Chart *pChart = WindowService::Instance().FindByIndex(chartIndex);

            if (pChart == nullptr) {
                respTypeChar = CHART_INDEX_NOT_FOUND;
                pRespBuffer->Append(&respTypeChar, 1);
            }
            else {
                // TODO: validate dimType/arrangeType/eleType
                std::unique_ptr<Dataset> p(new Dataset((Dataset::Dimension)dimType, (Dataset::ArrangeType)arrangeType,(Dataset::ElemDataType)eleType, dataset, nbBytesOfDataset));
                auto dataId = p->GetId();

                respTypeChar = ALL_RIGHT;
                pRespBuffer->Append(&respTypeChar, 1);

                pRespBuffer->AppendOneByte((unsigned char)(dataId >> (3 * 8u)));
                pRespBuffer->AppendOneByte((unsigned char)((dataId << 1 * 8u) >> (3 * 8u)));
                pRespBuffer->AppendOneByte((unsigned char)((dataId << 2 * 8u) >> (3 * 8u)));
                pRespBuffer->AppendOneByte((unsigned char)(dataId & 0xff));

                // mvoe Dataset to pChart
                pChart->OnDataComming(p);
            }
        }
        else if (reqSubType == 0x11) {
            // clear all data
            unsigned char chartIndex = *(pReqBody->Buffer());

            Chart *pChart = WindowService::Instance().FindByIndex(chartIndex);

            if (pChart == nullptr) {
                respTypeChar = CHART_INDEX_NOT_FOUND;
                pRespBuffer->Append(&respTypeChar, 1);
            }
            else {
                respTypeChar = ALL_RIGHT;
                pRespBuffer->Append(&respTypeChar, 1);
                pChart->ClearAllDataset();
            }
        }
        else {
            respTypeChar = UNKNOWN_REQUEST;
            pRespBuffer->Append(&respTypeChar, 1);
        }
    }
    else if (RequestMajorType::D == (RequestMajorType)reqMajorType) {
        unsigned char chartIndex = *(pReqBody->Buffer());
        const char *pDataId = pReqBody->Buffer() + 1;
        unsigned char dataShape = *(pReqBody->Buffer() + 1 + 4);
        unsigned int dataId = 0;
        {
            unsigned char tempByte;
            tempByte = *(pDataId++); dataId += (tempByte << (3 * 8u));
            tempByte = *(pDataId++); dataId += (tempByte << (2 * 8u));
            tempByte = *(pDataId++); dataId += (tempByte << (1 * 8u));
            tempByte = *(pDataId++); dataId += tempByte;
        }

        Chart *pChart = WindowService::Instance().FindByIndex(chartIndex);

        if (pChart == nullptr) {
            respTypeChar = CHART_INDEX_NOT_FOUND;
            pRespBuffer->Append(&respTypeChar, 1);
        }
        else {
            respTypeChar = ALL_RIGHT;
            pRespBuffer->Append(&respTypeChar, 1);
            pChart->OnSetShowData(dataId, dataShape);
        }
    }
}
