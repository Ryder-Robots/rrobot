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
    uint8_t data[] = {101, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E};
    RmMultiWii multiWii = RmMultiWii::createInstance(data, crc);

    ASSERT_EQ(MSPCOMMANDS::MSP_STATUS, multiWii.getCommand());
    ASSERT_EQ(0, multiWii.getSize());
    
    const uint8_t* data2 = reinterpret_cast<const uint8_t*>(multiWii.encode(crc).c_str());

    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(data[i], data2[i]);
    }
 }

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
