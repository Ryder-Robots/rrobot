#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
#include <rrobot/protocols/curators/rrp/mspevent.hpp>
#include <rrobot/protocols/curators/rrp/rrpcurator.hpp>

using namespace rrobot;

using ::testing::Return;
using ::testing::_;

using json = nlohmann::json;

class TestRrpCurator : public ::testing::Test {
    protected:
     void SetUp() override {
         // Setup code
     }
 
     void TearDown() override {
         // Teardown code
     }
 };

 
 TEST(TestRrpCurator, TestGeneratedEvents) {
    msp_ident payload = msp_ident();
    payload.set_msp_version(MSP_VERSION::SKULD002);
    payload.set_multitype(MULTITYPE_T::QUADP);
    payload.set_version("1.0.0");
    payload.set_capability(2);

    EXPECT_EQ(MSP_VERSION::SKULD002, payload.get_msp_version());
    EXPECT_EQ(MULTITYPE_T::QUADP, payload.get_multitype());
    EXPECT_EQ("1.0.0", payload.get_version());
    EXPECT_EQ(2, payload.get_capability());
}

TEST(TestRrpCurator, TestSingleCurator) {
    // This is not an inbound event,  but a good test because it uses custom types
    json inbound = {
        {"msp_version", "SKULD002"},
        {"multitype", "QUADP"},
        {"version", "1.0.0"},
        {"capability", 2}
    };

    msp_ident_curator curator = msp_ident_curator();
    Event* event = curator.deserialize(inbound);
    msp_ident payload = event->getPayload<msp_ident>();

    EXPECT_EQ(MSP_VERSION::SKULD002, payload.get_msp_version());
    EXPECT_EQ(MULTITYPE_T::QUADP, payload.get_multitype());
    EXPECT_EQ("1.0.0", payload.get_version());
    EXPECT_EQ(2, payload.get_capability());
    delete(event); 
}

TEST(TestRrpCurator, TestSerialize) {
    msp_ident *payload = new msp_ident();
    payload->set_msp_version(MSP_VERSION::SKULD002);
    payload->set_multitype(MULTITYPE_T::QUADP);
    payload->set_version("1.0.0");
    payload->set_capability(2);
    Event event = Event(MSPCOMMANDS::MSP_IDENT, MSPDIRECTION::EXTERNAL_OUT, payload);

    msp_ident_curator curator = msp_ident_curator();
    json outbound = curator.serialize(&event);

    EXPECT_EQ("MSP_IDENT", outbound["command"]);
    EXPECT_EQ("SKULD002", outbound["payload"]["msp_version"]);
    EXPECT_EQ("QUADP", outbound["payload"]["multitype"]);
    EXPECT_EQ("1.0.0", outbound["payload"]["version"]);
    EXPECT_EQ(2, outbound["payload"]["capability"]);
}

TEST(TestRrpCurator, TestGeneratedDeserialize) {
    json inbound = {{"key", "test"}};
    msp_authkey_curator curator = msp_authkey_curator();
    Event* event = curator.deserialize(inbound);

    EXPECT_EQ(MSPCOMMANDS::MSP_AUTHKEY, event->getCommand());
    EXPECT_EQ(MSPDIRECTION::EXTERNAL_IN, event->getDirection());

    msp_authkey payload = event->getPayload<msp_authkey>();
    EXPECT_EQ("test", payload.get_key());
    delete(event);
}

TEST(TestRrpCurator, TestGeneratedSerialize) {
    msp_authkey *payload = new msp_authkey();
    payload->set_key("test");
    Event* event = new Event(MSPCOMMANDS::MSP_AUTHKEY, MSPDIRECTION::EXTERNAL_OUT, payload);

    msp_authkey_curator curator = msp_authkey_curator();
    json outbound = curator.serialize(event);

    EXPECT_EQ("MSP_AUTHKEY", outbound["command"]);
    EXPECT_EQ("test", outbound["payload"]["key"]);
    delete(event);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}