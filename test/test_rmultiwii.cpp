#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <rrobot/protocols/curators/multiwii/rmultiwii.hpp>

using namespace rrobot;
using json = nlohmann::json;

namespace fs = std::filesystem;

class TestMultiWii : public ::testing::Test {
    protected:
     void SetUp() override {
         // Setup code
     }
 
     void TearDown() override {
         // Teardown code
     }
 };

 TEST_F(TestMultiWii, shouldEncodeStatus) {
    Crc32 crc = Crc32();
    string data;
    data.append(1, static_cast<char>(101));
    data.append(6, static_cast<char>(0x00));
    data.append(1, static_cast<char>(0x1E));

    RmMultiWii multiWii = RmMultiWii::createInstance(data, crc);

    ASSERT_EQ(MSPCOMMANDS::MSP_STATUS, multiWii.getCommand());
    ASSERT_EQ(0, multiWii.getSize());
    
    const uint8_t* data2 = reinterpret_cast<const uint8_t*>(multiWii.encode(crc).c_str());

    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(data[i], data2[i]);
    }
 }

 TEST_F(TestMultiWii, shouldEncodeRequest) {
    Crc32 crc = Crc32();
    std::string data = "";

    RmMultiWii multiWii = RmMultiWii::createInstance(data, MSPCOMMANDS::MSP_SENSOR_MAG);
    ASSERT_EQ(MSPCOMMANDS::MSP_SENSOR_MAG, multiWii.getCommand());
    ASSERT_EQ(0, multiWii.getSize());

    const uint8_t* data2 = reinterpret_cast<const uint8_t*>(multiWii.encode(crc).c_str());
    ASSERT_EQ(MSPCOMMANDS::MSP_SENSOR_MAG, data2[0]);
    ASSERT_EQ(0x00, data2[1]);
    ASSERT_EQ(0x00, data2[2]);
    ASSERT_EQ(0x00, data2[3]);
    ASSERT_EQ(0x00, data2[4]);
    ASSERT_EQ(0x00, data2[5]);
    ASSERT_EQ(0x00, data2[6]);
    ASSERT_EQ(0x1E, data2[7]);
 }

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
