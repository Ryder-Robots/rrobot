#include <rrobot/protocols/curators/rrp/msp_status_curator.hpp>

using namespace rrobot;

Event* msp_status_curator::deserialize(json in) { return nullptr; }

// ignore event, status is only produced on request
Event* msp_status_curator::deserialize(json in, MSPDIRECTION direction) { return deserialize(in); }

json msp_status_curator::serialize(Event* in) {
    string command = getCommandString();
    msp_status payload = in->getPayload<msp_status>();

    json j = {{"command", command}, {"payload", {
        {"current_set", payload.get_current_set()},
        {"cycletime", payload.get_cycletime()},
        {"flag", payload.get_flag()},
        {"i2c_errors_count", payload.get_i2c_errors_count()}
    }}};

    std::vector<json> sensors_json;
    auto i = 0;
    for (const auto& sensor : payload.get_sensors()) {
        sensors_json.push_back({
            {"name", payload.get_vnames().at(i++)},
            {"x", sensor.x()},
            {"y", sensor.y()},
            {"z", sensor.z()}
        });
    }
    j["payload"]["sensors"] = sensors_json;
    return j;
}