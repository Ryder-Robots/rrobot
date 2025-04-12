#include <rrobot/protocols/curators/encoder.hpp>

#include <string.h>

using namespace rrobot;

/**
 * @fn encodeFloat
 * @brief
 * given float, encode it two a string.
 */
std::string Encoder::encodeFloat(float in) {
    const char* byte_ptr = reinterpret_cast<char*>(&in);
    std::string out(byte_ptr);
    return out;
}

/**
 * @fn decodeFloat
 * @brief
 * convert encoded string into floating point number.
 */
float Encoder::decodeFloat(std::string in) {
    float out;
    memcpy(&out, in.c_str(), sizeof(float));
    return out;
}

/**
 * @fn encodeUint32
 * @brief
 * encodes uint32.
 */
std::string Encoder::encodeUint32(uint32_t in) {
    std::string out = "";

    out.append(1, in >> 24 & 0xFF);
    out.append(1, in >> 16 & 0xFF);
    out.append(1, in >> 8 & 0xFF);
    out.append(1, in & 0xFF);
    return out;
}

/**
 * @fn dencodeUint32
 * @brief
 * dencodes uint32.
 */
uint32_t Encoder::decodeInt32(std::string in) {
    uint32_t out = 0;
    out = in[0] << 24 & 0xFF;
    out = in[1] << 16 & 0xFF;
    out = in[2] << 8 & 0xFF;
    out = in[3];
    return out;   
}