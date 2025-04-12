#include <rrobot/protocols/curators/multiwii/rmultiwii.hpp>

using namespace rrobot;

/*
Memory checking results:
Mismatched deallocation - 1
Memory Leak - 1
 */
RmMultiWii RmMultiWii::createInstance(std::string data, Crc32 crc) {
    MSPCOMMANDS cmd = getSupportCommand(data[0]);

    int16_t sz = Encoder::dncodeUint16(data.substr(1,2));
    int32_t crc32 = Encoder::decodeInt32(data.substr(3, 4));
    RmMultiWii result = RmMultiWii(cmd, sz);

    for (int i = 0; i < sz; i++) {
        result._payload += static_cast<char>(data[i + 7]);

        if (data[i + 7] == static_cast<char>(_TERMINATION_CHAR)) {
            throw RrIOException("unexpected character in sequence");
        }
    }

    if (sz > 0) {
        if (crc32 != crc.calculate(result.getPayload())) {
            throw RrIOException("failed CRC check");
        }
    }
    return result;
}

RmMultiWii RmMultiWii::createInstance(std::string data, MSPCOMMANDS cmd) {
    RmMultiWii result = RmMultiWii(cmd, static_cast<uint16_t>(data.size()));
    result._payload = data;
    result._sz = static_cast<uint16_t>(data.size());
    return result;
}

string RmMultiWii::getPayload() {
    return _payload;
}

std::string RmMultiWii::encode(Crc32 crc) {
    std::string data;
    data = static_cast<char>(_cmd);
    
    // size
    data += Encoder::encodeUint16(_sz);

    // calculate CRC
    uint32_t crc32 = 0;

    if (_sz != 0) {
        crc32 = crc.calculate(getPayload());
    }
    data += Encoder::encodeUint32(crc32);

    if (_sz != 0) {
        for (int i = 0;i < _sz; i++) 
            data += _payload.c_str()[i];
    }
    data += _TERMINATION_CHAR & 0xFF;
    return data; 
}

RmMultiWii::~RmMultiWii() {
}