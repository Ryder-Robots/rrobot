#include <rrobot/protocols/curators/rrp/msp_sensor_acc_curator.hpp>

using namespace rrobot;

Event* msp_sensor_acc_curator::deserialize(json in) {
    msp_sensor_acc* payload = new msp_sensor_acc();
    Event* event = new Event(MSP_SENSOR_ACC, _direction, payload);
    validate("available", in);
    validate("data", in);
    validate("x", in["data"]);
    validate("y", in["data"]);
    validate("z", in["data"]);
    dlib::vector<float, 3> data(in["data"]["x"], in["data"]["y"], in["data"]["z"]);
    payload->set_data(data);
    payload->set_available(in["available"]);
    return event;
}

Event* msp_sensor_acc_curator::deserialize(json in, MSPDIRECTION direction) { return deserialize(in); }

json msp_sensor_acc_curator::serialize(Event* in) {
    string command = getCommandString();
    msp_sensor_acc payload = in->getPayload<msp_sensor_acc>();
    json j = {{"command", command},
              {"payload",
               {{"available", payload.get_available()},
                {"data",
                 {
                     {"x", payload.get_data().x()},
                     {"y", payload.get_data().y()},
                     {"z", payload.get_data().z()},
                 }}}}};
    return j;
}