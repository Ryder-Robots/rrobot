#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <rrobot/protocols/curators/encoder.hpp>

using namespace rrobot;

class TestEncoder : public ::testing::Test {
    protected:
     void SetUp() override {
         // Setup code
     }
 
     void TearDown() override {
         // Teardown code
     }
};

TEST_F(TestEncoder, shouldEncodeDecodeInt32) {
    std::string v = Encoder::encodeUint32(32);
    uint32_t v2 = Encoder::decodeInt32(v);

    EXPECT_EQ(32, v2);
}


// 
// {0x78, 0x56, 0x34, 0x12}
TEST_F(TestEncoder, shouldEncodeInt32) {

    std::string v = Encoder::encodeUint32(305419896);
    uint32_t i = Encoder::decodeInt32(v);
    EXPECT_EQ(305419896, i);
}


TEST_F(TestEncoder, shouldEncodeFloat) {
    std::string v = Encoder::encodeFloat(300.45f);
    float f = Encoder::decodeFloat(v);
    EXPECT_EQ(300.45f, f);
}

TEST_F(TestEncoder, shouldEncodeUint16) {
    std::string v = Encoder::encodeUint16(34556);
    uint16_t f = Encoder::dncodeUint16(v);
    EXPECT_EQ(34556, f);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
