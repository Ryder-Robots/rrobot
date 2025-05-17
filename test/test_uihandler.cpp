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
        // set destination point <0,10,0>
        json inbound;
        inbound["command"] = "MSP_VECTOR";
        json payload;
        payload["x"] = 0;
        payload["y"] = 100;
        payload["z"] = 0;


        // second message
        json inbound2;
        inbound["command"] = "MSP_VECTOR";
        json payload2;
        payload2["x"] = 200;
        payload2["y"] = 100;
        payload2["z"] = 0;

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

TEST_F(TestUiHandler, DISABLED_TestInBoundEvents1) {
    MockExternal external;
    UiHandler ui(external, serializer, smg, env);

    EXPECT_EQ(true, ui.available());
    Event* e = ui.produce(smg);
    EXPECT_TRUE(e->hasPayload());
    msp_vector payload1 = e->getPayload<msp_vector>();
    EXPECT_EQ(0, payload1.get_x());
    EXPECT_EQ(100, payload1.get_y());
    EXPECT_EQ(0, payload1.get_z());

    delete(e);
}

TEST_F(TestUiHandler, DISABLED_TestInBoundEvents2) {
    MockExternal external;
    UiHandler ui(external, serializer, smg, env);

    EXPECT_EQ(true, ui.available());
    Event* e = ui.produce(smg);
    EXPECT_TRUE(e->hasPayload());
    msp_vector payload1 = e->getPayload<msp_vector>();
    EXPECT_EQ(0, payload1.get_x());
    EXPECT_EQ(100, payload1.get_y());
    EXPECT_EQ(0, payload1.get_z());

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
    msp_vector* payload = new  msp_vector();
    payload->set_x(500);
    payload->set_y(600);
    payload->set_z(0);

    Event* event = new Event(MSPCOMMANDS::MSP_VECTOR, MSPDIRECTION::EXTERNAL_IN , payload);
    MockExternal external;

    UiHandler ui(external, serializer, smg, env);
    ui.consume(smg, event);
    json j = json::parse(external._outbound.substr(0, external._outbound.length() - 1));
    EXPECT_EQ(500, j["payload"]["x"]);
    EXPECT_EQ(600, j["payload"]["y"]);
    EXPECT_EQ(0, j["payload"]["z"]);
    EXPECT_EQ("MSP_VECTOR", j["command"]);
    delete(event);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}