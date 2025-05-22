#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include <nlohmann/json.hpp>
#include <rrobot/protocols/curators/multiwii/rmultiwii.hpp>
#include <rrobot/protocols/curators/multiwii/rmmsp_sensor_mag_curator.hpp>

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
    string data = "101;0;0;";
    data.push_back(RmMultiWii::_TERMINATION_CHAR);
    RmMultiWii multiWii = RmMultiWii::createInstance(data, crc);

    ASSERT_EQ(MSPCOMMANDS::MSP_STATUS, multiWii.getCommand());
    ASSERT_EQ(0, multiWii.getSize());

    const string data2 = multiWii.encode(crc);
    std::vector<string> tokens;
    boost::split(tokens, data2, boost::is_any_of(";"));
    ASSERT_EQ(std::to_string(MSPCOMMANDS::MSP_STATUS), tokens.at(0));
    ASSERT_EQ(std::to_string(0), tokens.at(1));
    ASSERT_EQ("", tokens.at(2));
}

TEST_F(TestMultiWii, shouldEncodeRequest) {
        Crc32 crc = Crc32();
        msp_sensor_mag mag;
        mag.set_available(1);
        mag.set_data(dlib::vector<float, 3>(0, 1, 0));
        RmMspSensorMagCurator curator;

        RmMultiWii multiWii = RmMultiWii::createInstance(curator.serializem(mag), MSPCOMMANDS::MSP_SENSOR_MAG);
        ASSERT_EQ(MSPCOMMANDS::MSP_SENSOR_MAG, multiWii.getCommand());
        ASSERT_EQ(28, multiWii.getSize());

        mag = curator.deserializem(multiWii);

        ASSERT_EQ(1, mag.get_available());
        ASSERT_FLOAT_EQ(0, mag.get_data().x());
        ASSERT_FLOAT_EQ(1, mag.get_data().y());
        ASSERT_FLOAT_EQ(0, mag.get_data().z());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
