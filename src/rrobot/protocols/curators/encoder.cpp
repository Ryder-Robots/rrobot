#include <rrobot/protocols/curators/encoder.hpp>

using namespace rrobot;

/**
 * @fn encodeFloat
 * @brief
 * given float, encode it two a string.
 */
std::string Encoder::encodeFloat(float in) {
    return std::to_string(in);
}

/**
 * @fn decodeFloat
 * @brief
 * convert encoded string into floating point number.
 */
float Encoder::decodeFloat(std::string in) {
    std:string s; 
    if (in.empty()) {
        s = "0";
    } else {
       s = in;
    }
    float v = 0;
    try {
        v = std::stof(s);
    } catch (...) {
        throw InvalidFormat("argument was not an int32");
    }
    return v;
}

/**
 * @fn encodeUint32
 * @brief
 * encodes uint32.
 */
std::string Encoder::encodeUint32(uint32_t in) {
    return std::to_string(in);
}

/**
 * @fn dencodeUint32
 * @brief
 * dencodes uint32.
 */
uint32_t Encoder::decodeInt32(std::string in) {
    std:string s; 
    if (in.empty()) {
        s = "0";
    } else {
       s = in;
    }
    uint32_t v = 0;
    try {
        v = std::stoi(s);
    } catch (...) {
        throw InvalidFormat("argument was not an int32");
    }
    return v;
}

std::string Encoder::encodeUint16(uint16_t in) {
    return std::to_string(in);
}


uint16_t Encoder::dncodeUint16(std::string in) {
    std:string s; 
    if (in.empty()) {
        s = "0";
    } else {
       s = in;
    }
    uint16_t v = 0;
    try {
        v = std::stol(s);
    } catch (...) {
        throw InvalidFormat("argument was not a long");
    }
    return v;
}


/**
 * @fn encodeUint32
 * @brief
 * encodes uint32.
 */
std::string Encoder::encodeUint32Msb(uint32_t in) {
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
uint32_t Encoder::decodeInt32Msb(std::string in) {
    uint32_t big_endian_value;
    std::memcpy(&big_endian_value, in.c_str(), sizeof(big_endian_value));
    return boost::endian::big_to_native(big_endian_value);
}

std::string Encoder::encodeUint16Msb(uint16_t in) {
    std::string out = "";
    uint16_t msb_value = boost::endian::native_to_big(in);
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&msb_value);

    out.append(1, bytes[0]);
    out.append(1, bytes[1]);
    return out;
}


uint16_t Encoder::dncodeUint16Msb(std::string in) {
    uint16_t big_endian_value;
    std::memcpy(&big_endian_value, in.c_str(), sizeof(big_endian_value));
    return boost::endian::big_to_native(big_endian_value);
}