#include <rrobot/protocols/curators/encoder.hpp>

using namespace rrobot;

/**
 * @fn encodeFloat
 * @brief
 * given float, encode it two a string.
 */
std::string Encoder::encodeFloat(float in) {
    uint8_t msb_bytes[4];

    memcpy(msb_bytes, &in, sizeof(float));    
    std::string out = "";
    out.append(1, msb_bytes[3]);
    out.append(1, msb_bytes[2]);
    out.append(1, msb_bytes[1]);
    out.append(1, msb_bytes[0]);
    return out;
}

/**
 * @fn decodeFloat
 * @brief
 * convert encoded string into floating point number.
 */
float Encoder::decodeFloat(std::string in) {
    union out_u {
        uint8_t in[4];
        float value;
    };
    out_u out;
    out.in[3] = in.c_str()[0] & 0xFF;
    out.in[2] = in.c_str()[1] & 0xFF;
    out.in[1] = in.c_str()[2] & 0xFF;
    out.in[0] = in.c_str()[3] & 0xFF;

    return out.value;
}

/**
 * @fn encodeUint32
 * @brief
 * encodes uint32.
 */
std::string Encoder::encodeUint32(uint32_t in) {
    std::string out = "";
    uint32_t msb_value = boost::endian::native_to_big(in);
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&msb_value);

    out.append(1, bytes[0]);
    out.append(1, bytes[1]);
    out.append(1, bytes[2]);
    out.append(1, bytes[3]);
    return out;
}

/**
 * @fn dencodeUint32
 * @brief
 * dencodes uint32.
 */
uint32_t Encoder::decodeInt32(std::string in) {
    uint32_t big_endian_value;
    std::memcpy(&big_endian_value, in.c_str(), sizeof(big_endian_value));
    return boost::endian::big_to_native(big_endian_value);
}

std::string Encoder::encodeUint16(uint16_t in) {
    std::string out = "";
    uint16_t msb_value = boost::endian::native_to_big(in);
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&msb_value);

    out.append(1, bytes[0]);
    out.append(1, bytes[1]);
    return out;
}


uint16_t Encoder::dncodeUint16(std::string in) {
    uint16_t big_endian_value;
    std::memcpy(&big_endian_value, in.c_str(), sizeof(big_endian_value));
    return boost::endian::big_to_native(big_endian_value);
}