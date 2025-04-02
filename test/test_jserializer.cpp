#include <gtest/gtest.h>

#include <rrobot/events/serializers/jserializer.hpp>

using namespace rrobot;

class TestJserializer : public ::testing::Test {
    protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};

TEST(TestJserializer, TestSerialize) {
    Jserializer jserializer = Jserializer();

    msp_authkey *mspAuthKey = new msp_authkey();
    mspAuthKey->set_key("test");
    Event *event = new Event(MSPCOMMANDS::MSP_AUTHKEY, MSPDIRECTION::EXTERNAL_IN, mspAuthKey);
    json out = jserializer.serialize(event);

    EXPECT_EQ("MSP_AUTHKEY", out["command"]);
    EXPECT_EQ("test", out["payload"]["key"]);
    delete(event);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}