#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <rrobot/handlers/uihandler.hpp>
#include <rrobot/events/serializers/jserializer.hpp>
#include <rrobot/environment/environmentProcessor.hpp>
#include <rrobot/state/state_manager.hpp>

using namespace std;
using namespace rrobot;

namespace fs = std::filesystem;

using ::testing::Return;
using ::testing::_;
using ::testing::AnyNumber;

Environment createEnv() {
    const fs::path filepath = "manifests/virtual.json";
    std::ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    return EnviromentProcessor::createEnvironment(manifest);
}


class TestUiHandler : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }

    Jserializer serializer = Jserializer();
    StateManager smg;
    Environment env = createEnv();
};

class MockExternal : public External {
    public:
    MockExternal() {
        json inbound;
        inbound["command"] = "MSP_SET_MOTOR_HBRIDGE";
        json payload;
        payload["motor1"] = 500;
        payload["motor2"] = 600;

        payload["motor3"] = 500;
        payload["motor4"] = 600;

        // set polarity of motors
        payload["in"] = 0b01010101;

        inbound["payload"] = payload;

        // second message
        json inbound2;
        inbound2["command"] = "MSP_SET_MOTOR_HBRIDGE";
        json payload2;
        payload2["motor1"] = 500;
        payload2["motor2"] = 600;

        payload2["motor3"] = 500;
        payload2["motor4"] = 600;

        // set polarity of motors
        payload2["in"] = 0b01010101;

        inbound2["payload"] = payload;

        // test an ident request.
        json msp_ident;
        msp_ident["command"] = "MSP_IDENT";
        _response = inbound.dump() + string(1, 0x1E) + inbound2.dump() + string(1, 0x1E) + msp_ident.dump() +  string(1, 0x1E);
    }

    virtual ssize_t recv_rr(void* buffer, size_t bufsz) override {
        memcpy(buffer, &(_response.c_str()[_pointer++]), sizeof(char));
        return 1;
    }

    virtual ssize_t send_rr(const void *buf, size_t bufsz) {
        _outbound = string(reinterpret_cast<const char*>(buf));
        return bufsz;
    }

    virtual ssize_t available() {
        return _response.size() - _pointer;
    }

    virtual void close_rr() override {};

    MOCK_METHOD(int, accept_rr, (), (override));

    string _response;
    string _outbound = "";

    MOCK_METHOD(int, shutdown_rr, (), (override));

    private:
    int _pointer = 0;   
};

class MockExternal2 : public External {
    public:
    MOCK_METHOD(ssize_t,recv_rr, (void*, size_t), (override));
    MOCK_METHOD(ssize_t, send_rr, (const void*, size_t), (override));
    MOCK_METHOD(void, close_rr, (), (override));
    MOCK_METHOD(int, accept_rr, (), (override));
    MOCK_METHOD(ssize_t, available, (), (override));
    MOCK_METHOD(int, shutdown_rr, (), (override));
};

TEST_F(TestUiHandler, TestInit) {
    MockExternal external;
    UiHandler ui(external, serializer, smg, env);
    EXPECT_EQ(true, true);
}

TEST_F(TestUiHandler, TestInBoundEvents1) {
    MockExternal external;
    UiHandler ui(external, serializer, smg, env);

    EXPECT_EQ(true, ui.available());
    Event* e = ui.produce(smg);
    EXPECT_TRUE(e->hasPayload());
    msp_set_motor_hbridge payload1 = e->getPayload<msp_set_motor_hbridge>();
    EXPECT_EQ(500, payload1.get_motor1());
    EXPECT_EQ(600, payload1.get_motor2());
    EXPECT_EQ(500, payload1.get_motor3());
    EXPECT_EQ(600, payload1.get_motor4());
    EXPECT_EQ(0b01010101, payload1.get_in());

    delete(e);
}

TEST_F(TestUiHandler, TestInBoundEvents2) {
    MockExternal external;
    UiHandler ui(external, serializer, smg, env);

    EXPECT_EQ(true, ui.available());
    Event* e = ui.produce(smg);
    EXPECT_TRUE(e->hasPayload());
    msp_set_motor_hbridge payload = e->getPayload<msp_set_motor_hbridge>();
    EXPECT_EQ(500, payload.get_motor1());
    EXPECT_EQ(600, payload.get_motor2());
    EXPECT_EQ(500, payload.get_motor3());
    EXPECT_EQ(600, payload.get_motor4());
    EXPECT_EQ(true, e->hasPayload());
    EXPECT_EQ(0b01010101, payload.get_in());

    delete(e);
}

TEST_F(TestUiHandler, TestInBoundEvents3) {
    MockExternal2 external;
    UiHandler ui(external, serializer, smg, env);

    EXPECT_CALL(external, recv_rr(_, _)).WillOnce(Return(-1));
    EXPECT_EQ(false, ui.available());
    EXPECT_THROW({ui.produce(smg);}, NetworkIOException);
}

TEST_F(TestUiHandler, TestOutBoundEvents) {
    msp_set_motor_hbridge* payload = new  msp_set_motor_hbridge();
    payload->set_motor1(500);
    payload->set_motor2(600);
    payload->set_motor3(700);
    payload->set_motor4(800);
    payload->set_in(0b10101010);
    Event* event = new Event(MSPCOMMANDS::MSP_SET_MOTOR_HBRIDGE, MSPDIRECTION::EXTERNAL_IN , payload);
    MockExternal external;

    UiHandler ui(external, serializer, smg, env);
    ui.consume(smg, event);
    json j = json::parse(external._outbound.substr(0, external._outbound.length() - 1));
    EXPECT_EQ(500, j["payload"]["motor1"]);
    EXPECT_EQ(600, j["payload"]["motor2"]);
    EXPECT_EQ(700, j["payload"]["motor3"]);
    EXPECT_EQ(800, j["payload"]["motor4"]);
    EXPECT_EQ(0b10101010, j["payload"]["in"]);
    EXPECT_EQ("MSP_SET_MOTOR_HBRIDGE", j["command"]);
    delete(event);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}