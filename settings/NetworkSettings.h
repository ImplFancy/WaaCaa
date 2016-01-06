#pragma once

class NetworkSettings
{
public:

    // TODO: dealing with port conflict
    int GetLocalPort() { return 6666; }

    static NetworkSettings &Get() {
        static NetworkSettings settings;
        return settings;
    }
};