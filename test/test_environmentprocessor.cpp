#include <gtest/gtest.h>

class TestEnvironmentProcessor : public ::testing::Test {
    protected:
    void SetUp() override {
        // Setup code
    }
    
    void TearDown() override {
        // Teardown code
    }
};

TEST(TestEnvironmentProcessor, TestEnvironment) {
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}