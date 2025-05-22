#include <rrobot/protocols/curators/multiwii/rmultiwii_curator.hpp>

using namespace rrobot;

Event* AbstractRmultiiCurator::deserialize(std::string in) {
    RmMultiWii multiwii =  RmMultiWii::createInstance(in, _crc);
    return deserializePayload(multiwii);
}


std::string AbstractRmultiiCurator::serialize(Event* in) {

    serializePayload(in);
    return (RmMultiWii::createInstance(serializePayload(in), in->getCommand())).encode(_crc);    
}


std::vector<std::string>  AbstractRmultiiCurator::decodeTokens(std::string payload) {
    std::vector<std::string> tokens;
    return boost::split(tokens, payload, boost::is_any_of(";"));
}

std::string  AbstractRmultiiCurator::encodeTokens(std::vector<std::string> tokens) {
    return boost::join(tokens, ";");
}