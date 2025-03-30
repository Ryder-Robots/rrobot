#include <rrobot/environment/environmentProcessor.hpp>

using namespace rrobot;

Environment EnviromentProcessor::createEnvironment(json manifest) {

    CHwModel hwModel = createHwModel(manifest);
    CRrSerial mc = createMc(manifest);
    RrVersion version;
    CQueues queues = createQueues(manifest);
    CServer server = createServer(manifest);
    CLogging logging = createLogging(manifest);

    return Environment(hwModel, mc, version, queues, server, logging);
}

CHwModel EnviromentProcessor::createHwModel(json manifest) {
    vector<string> keys = {"multitype", "mspversion", "capability"};
    verify(manifest, keys, "hwmodel");

    VALID_MULTITYPE_KEYS_INIT;
    VALID_MSP_VERSION_KEYS_INIT;

    MULTITYPE_T multiType = VALID_MULTITYPE_KEYS.at(manifest["hwmodel"]["multitype"]);
    MSP_VERSION mspVersion = VALID_MSP_VERSION_KEYS.at(manifest["hwmodel"]["mspversion"]);
    uint32_t capability = static_cast<uint32_t>(manifest["hwmodel"]["capability"]);

    return CHwModel(multiType, mspVersion, capability);
}

CRrSerial EnviromentProcessor::createMc(json manifest) {
    vector<string> keys = {"port", "baud", "charsize", "flow_control", "stop_bits", "parity"};
    verify(manifest, keys, "mc");

    VALID_BAUDRATES_INIT;
    VALID_CHAR_SZ_INIT;
    VALID_FLOW_CNT_INIT;
    VALID_STOPBIT_INIT;
    VALID_PARITY_INIT;

    LibSerial::BaudRate baudRate = VALID_BAUDRATES.at(manifest["mc"]["baud"]);
    LibSerial::CharacterSize charsize = VALID_CHAR_SZ.at(manifest["mc"]["charsize"]);
    LibSerial::FlowControl flowControl = VALID_FLOW_CNT.at(manifest["mc"]["flow_control"]);
    LibSerial::StopBits stopBits = VALID_STOPBIT.at(manifest["mc"]["stop_bits"]);
    LibSerial::Parity parity = VALID_PARITY.at(manifest["mc"]["parity"]);

    return CRrSerial(manifest["mc"]["port"], baudRate, charsize, flowControl, stopBits, parity);
}

CQueues EnviromentProcessor::createQueues(json manifest) {
    vector<string> keys = {"limit", "thread_wait_time", "thread_process_time", "thread_time_out"};
    verify(manifest, keys, "queues");
    return CQueues(manifest["queues"]["limit"], 
        manifest["queues"]["thread_wait_time"],  
        manifest["queues"]["thread_process_time"],
        manifest["queues"]["thread_time_out"]);
}

CServer EnviromentProcessor::createServer(json manifest) {
    vector<string> keys = {"port", "maxconnections"};
    verify(manifest, keys, "server");
    return CServer(manifest["server"]["port"], manifest["server"]["maxconnections"]);
}

CLogging EnviromentProcessor::createLogging(json manifest) {
    vector<string> keys = {"logLevel"};
    verify(manifest, keys, "logging");

    VALID_LOGLEVELS;
    dlib::log_level logLevel = VALID_LOGLEVELS_KEYS.at(manifest["logging"]["logLevel"]);
    return CLogging(logLevel);
}

bool EnviromentProcessor::verify(json manifest, vector<string> keys, string container) {
    if (!manifest.contains(container)) {
        throw MissingRequiredAttributeException("missing required attribute for " + container);
    }
    for (auto key : keys) {
        if (!manifest[container].contains(key)) {
            throw MissingRequiredAttributeException("missing required attribute " + key + " for " + container);
        }
    }
    return true;
}