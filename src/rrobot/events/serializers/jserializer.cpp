#include <rrobot/events/serializers/jserializer.hpp>

using namespace rrobot;

Jserializer::Jserializer() {
    VALID_MSPCOMMANDS;
    _validMspCommands = VALID_MSPCOMMANDS_KEYS;
    CURATOR_MAP_INIT;
    _curatorMap = CURATOR_MAP_KEYS;
}

Jserializer::~Jserializer() {
    for (const auto& [key, value] : _curatorMap) {
       delete(_curatorMap.at(key));
    }
}

json Jserializer::serialize(Event* event) {
    RrpCuratorAbstract* curator = _curatorMap.at(event->getCommand());
    return curator->serialize(event);
}

Event* Jserializer::deserialize(json j) {
    return deserialize(j, MSPDIRECTION::EXTERNAL_IN);
}

Event* Jserializer::deserialize(json j, MSPDIRECTION direction) {
    if(!(
        j.contains("command") && (_validMspCommands.find(j["command"]) != _validMspCommands.end())
    )) {
        throw MissingRequiredAttributeException("command is required attributes");
    }

    Event* event = nullptr;
    RrpCuratorAbstract* curator = _curatorMap.at(_validMspCommands.at(j["command"]));
    if (j.contains("payload")) {   
        json payload = j["payload"];
        event = curator->deserialize(payload, direction);
    } else {
        event = curator->deserialize(direction);
    }

    return event;    
}