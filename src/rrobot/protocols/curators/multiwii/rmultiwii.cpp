#include <rrobot/protocols/curators/multiwii/rmultiwii.hpp>

using namespace rrobot;

RmMultiWii RmMultiWii::createInstance(std::string data, Crc32 crc) {
    std::vector<string> tokens;
    boost::split(tokens, data, boost::is_any_of(_DELIMETER));
    MSPCOMMANDS cmd = getSupportCommand(std::stoi(tokens.at(0)));
    int16_t sz = Encoder::dncodeUint16(tokens.at(1));
    std::string crc32 = tokens.at(2);
    RmMultiWii result = RmMultiWii(cmd, sz);

    if (sz > 0) {
        std::vector<string> payload;
        for (int i = 3; i < tokens.size() - 1; i++) {
            payload.push_back(tokens[i]);
        }
        result._payload = boost::join(payload, _DELIMETER);
    }
    return result;
}

RmMultiWii RmMultiWii::createInstance(std::string data, MSPCOMMANDS cmd) {
    RmMultiWii result = RmMultiWii(cmd, static_cast<uint16_t>(data.size()));
    result._payload = data;
    result._sz = static_cast<uint16_t>(data.size());
    return result;
}

string RmMultiWii::getPayload() { return _payload; }

std::string RmMultiWii::encode(Crc32 crc) {
    std::vector<string> tokens;
    tokens.push_back(std::to_string(_cmd));
    tokens.push_back(Encoder::encodeUint16(_sz));
    std::string crc32 = "";
    if (_sz != 0) {
        crc32 = crc.calculate(getPayload());
    }
    tokens.push_back(crc32);

    std::string encoded = boost::join(tokens, _DELIMETER);
    if (getSize() > 0) {
        encoded.push_back(_DELIMETER_C);
        encoded += getPayload();
    }
    encoded.push_back(_DELIMETER_C);
    encoded.push_back(_TERMINATION_CHAR);

    return encoded;
}

RmMultiWii::~RmMultiWii() {}