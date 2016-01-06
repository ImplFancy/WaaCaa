#pragma once

#include <memory>

class Dataset;
class ProtocolRequestListener
{
    virtual void OnDataComming(/*const*/ std::unique_ptr<Dataset> &newData) = 0;
};