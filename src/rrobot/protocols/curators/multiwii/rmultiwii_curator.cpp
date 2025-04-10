#include <rrobot/protocols/curators/multiwii/rmultiwii_curator.hpp>

using namespace rrobot;

Event* AbstractRmultiiCurator::deserialize(std::string in) {
    RmMultiWii multiwii =  RmMultiWii::createInstance(in, _crc);
    return deserializePayload(multiwii);
}


std::string AbstractRmultiiCurator::serialize(Event* in) {
    return (RmMultiWii::createInstance(serializePayload(in), in->getCommand())).encode(_crc);    
}

std::string AbstractRmultiiCurator::encodeFloat(float f, std::string& in) {
    const uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(&f);
    in.append(1, byte_ptr[0]);
    in.append(1, byte_ptr[1]);
    in.append(1, byte_ptr[2]);
    in.append(1, byte_ptr[3]);
    return in;
}


std::string AbstractRmultiiCurator::encodeUint32(uint32_t i, std::string& in) {
    in.append(1, static_cast<char>((i >> 24) & 0xFF));
    in.append(1, static_cast<char>((i >> 16) & 0xFF));
    in.append(1, static_cast<char>((i >> 8) & 0xFF));
    in.append(1, static_cast<char>(i & 0xFF));
    return in;
}

uint32_t AbstractRmultiiCurator::decodeUint32(std::string enc, int p) {
    uint32_t r = enc[p] << 24 & 0xFF;
    r = enc[p + 1] << 16 & 0xFF;
    r = enc[p + 2] << 18 & 0xFF;
    r = enc[p + 3] & 0xFF;
    return r;
}

float AbstractRmultiiCurator::decodeFloat(std::string enc, int p) {
    union BytesToFloat {
        uint8_t bytes[4];
        float value;
    };

    BytesToFloat converter;
    converter.bytes[0] = enc[p];
    converter.bytes[1] = enc[p+1];
    converter.bytes[2] = enc[p+2];
    converter.bytes[3] = enc[p+3];
    return converter.value;
}