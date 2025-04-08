#include <rrobot/protocols/curators/multiwii/rmultiwii.hpp>

using namespace rrobot;

/*
Memory checking results:
Mismatched deallocation - 1
Memory Leak - 1
 */
RmMultiWii RmMultiWii::createInstance(uint8_t* data, Crc32 crc) {
    MSPCOMMANDS cmd = getSupportCommand(data[0]);
    int16_t sz = static_cast<uint16_t>(data[1] | (data[2] << 8));
    int32_t crc32 = static_cast<uint32_t>(data[3] | data[4] << 8 | data[5] << 16 | data[6] << 24);
    RmMultiWii result = RmMultiWii(cmd, sz);

    for (int i = 0; i < sz; i++) {
        result._payload += static_cast<char>(data[i + 7]);

        if (data[i + 7] == static_cast<char>(_TERMINATION_CHAR)) {
            throw RrIOException("unexpected character in sequence");
        }
    }

    if (sz > 0) {
        if (crc32 != crc.calculate(result.getPayload(), sz)) {
            throw RrIOException("failed CRC check");
        }
    }
    return result;
}

const uint8_t* RmMultiWii::getPayload() {
    return reinterpret_cast<const uint8_t*>(_payload.c_str());
}

std::string RmMultiWii::encode(Crc32 crc) {
    std::string data;
    data = static_cast<char>(_cmd);
    
    // size
    data += static_cast<char>((_sz >> 8) & 0xFF);
    data += static_cast<char>(_sz & 0xFF);

    // calculate CRC
    uint32_t crc32 = 0;

    if (_sz != 0) {
        crc32 = crc.calculate(getPayload(), _sz);
    }
    data += (_sz >> 24) & 0xFF;
    data += (_sz >> 16) & 0xFF;
    data += (_sz >> 8) & 0xFF;
    data += _sz & 0xFF;

    if (_sz != 0) {
        for (int i = 0;i < _sz; i++) 
            data += _payload.c_str()[i];
    }
    data += _TERMINATION_CHAR & 0xFF;
    return data; 
}

RmMultiWii::~RmMultiWii() {
}