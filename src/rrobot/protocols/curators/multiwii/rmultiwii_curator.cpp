#include <rrobot/protocols/curators/multiwii/rmultiwii_curator.hpp>

using namespace rrobot;

Event* AbstractRmultiiCurator::deserialize(std::string in) {
    RmMultiWii multiwii =  RmMultiWii::createInstance(in, _crc);
    return deserializePayload(multiwii);
}


std::string AbstractRmultiiCurator::serialize(Event* in) {
    return (RmMultiWii::createInstance(serializePayload(in), in->getCommand())).encode(_crc);    
}