#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rrobot/ai/saxprocessor.hpp>

using namespace rrobot;

class TestSaxProcessor : public ::testing::Test {
   protected:
    void SetUp() override {}

    void TearDown() override {
        // Teardown code
        unlink(_fname.c_str());
    }

    std::string _fname = "somefile.dat";
};

TEST_F(TestSaxProcessor, serialize) {
    saxprocessor sxp(_fname, false);
    sax_el el;
    el.tid = 1;
    el.v = dlib::vector<float, VECTOR_DIM>(0, 1, 0);
    el.cp = dlib::vector<float, VECTOR_DIM>(0, 0, 0);
    el.dp = dlib::vector<float, VECTOR_DIM>(3, 5, 0);
    el.acc = dlib::vector<float, VECTOR_DIM>(0.30, 0.40, 0.50);
    el.mag = dlib::vector<float, VECTOR_DIM>(0, 1, 0);
    el.gyro = dlib::vector<float, VECTOR_DIM>(1, 1, 0);

    sxp.serialize(el);
}

TEST_F(TestSaxProcessor, deserialize_1_rec) {
    saxprocessor sxp(_fname, false);
    sax_el el;
    el.tid = 1;
    el.v = dlib::vector<float, VECTOR_DIM>(0, 1, 0);
    el.cp = dlib::vector<float, VECTOR_DIM>(0, 0, 0);
    el.dp = dlib::vector<float, VECTOR_DIM>(3, 5, 0);
    el.acc = dlib::vector<float, VECTOR_DIM>(0.30, 0.40, 0.50);
    el.mag = dlib::vector<float, VECTOR_DIM>(0, 1, 0);
    el.gyro = dlib::vector<float, VECTOR_DIM>(1, 1, 0);

    sxp.serialize(el);
    sxp.set_isinput(true);
    sax_el e2 = sxp.deserialize();

    EXPECT_EQ(el.tid, e2.tid);

    EXPECT_EQ(el.v.x(), e2.v.x());
    EXPECT_EQ(el.v.y(), e2.v.y());
    EXPECT_EQ(el.v.z(), e2.v.z());

    EXPECT_EQ(el.cp.x(), e2.cp.x());
    EXPECT_EQ(el.cp.y(), e2.cp.y());
    EXPECT_EQ(el.cp.z(), e2.cp.z());

    EXPECT_EQ(el.dp.x(), e2.dp.x());
    EXPECT_EQ(el.dp.y(), e2.dp.y());
    EXPECT_EQ(el.dp.z(), e2.dp.z());

    EXPECT_EQ(el.acc.x(), e2.acc.x());
    EXPECT_EQ(el.acc.y(), e2.acc.y());
    EXPECT_EQ(el.acc.z(), e2.acc.z());

    EXPECT_EQ(el.mag.x(), e2.mag.x());
    EXPECT_EQ(el.mag.y(), e2.mag.y());
    EXPECT_EQ(el.mag.z(), e2.mag.z());

    EXPECT_EQ(el.gyro.x(), e2.gyro.x());
    EXPECT_EQ(el.gyro.y(), e2.gyro.y());
    EXPECT_EQ(el.gyro.z(), e2.gyro.z());
}


TEST_F(TestSaxProcessor, deserialize_2_rec) {
    saxprocessor sxp(_fname, false);
    sax_el el1;
    el1.tid = 1;
    el1.v = dlib::vector<float, VECTOR_DIM>(0, 1, 0);
    el1.cp = dlib::vector<float, VECTOR_DIM>(0, 0, 0);
    el1.dp = dlib::vector<float, VECTOR_DIM>(3, 5, 0);
    el1.acc = dlib::vector<float, VECTOR_DIM>(0.30, 0.40, 0.50);
    el1.mag = dlib::vector<float, VECTOR_DIM>(0, 1, 0);
    el1.gyro = dlib::vector<float, VECTOR_DIM>(1, 1, 0);
    sxp.serialize(el1);

    sax_el el2;
    el2.tid = 2;
    el2.v = dlib::vector<float, VECTOR_DIM>(1, 1, 0);
    el2.cp = dlib::vector<float, VECTOR_DIM>(1, 0, 0);
    el2.dp = dlib::vector<float, VECTOR_DIM>(4, 5, 0);
    el2.acc = dlib::vector<float, VECTOR_DIM>(1.30, 0.40, 0.50);
    el2.mag = dlib::vector<float, VECTOR_DIM>(1, 1, 0);
    el2.gyro = dlib::vector<float, VECTOR_DIM>(1, 1, 1);
    sxp.serialize(el2);


    sxp.set_isinput(true);
    sax_el e1 = sxp.deserialize();
    EXPECT_EQ(el1.tid, e1.tid);

    EXPECT_EQ(el1.v.x(), e1.v.x());
    EXPECT_EQ(el1.v.y(), e1.v.y());
    EXPECT_EQ(el1.v.z(), e1.v.z());

    EXPECT_EQ(el1.cp.x(), e1.cp.x());
    EXPECT_EQ(el1.cp.y(), e1.cp.y());
    EXPECT_EQ(el1.cp.z(), e1.cp.z());

    EXPECT_EQ(el1.dp.x(), e1.dp.x());
    EXPECT_EQ(el1.dp.y(), e1.dp.y());
    EXPECT_EQ(el1.dp.z(), e1.dp.z());

    EXPECT_EQ(el1.acc.x(), e1.acc.x());
    EXPECT_EQ(el1.acc.y(), e1.acc.y());
    EXPECT_EQ(el1.acc.z(), e1.acc.z());

    EXPECT_EQ(el1.mag.x(), e1.mag.x());
    EXPECT_EQ(el1.mag.y(), e1.mag.y());
    EXPECT_EQ(el1.mag.z(), e1.mag.z());

    EXPECT_EQ(el1.gyro.x(), e1.gyro.x());
    EXPECT_EQ(el1.gyro.y(), e1.gyro.y());
    EXPECT_EQ(el1.gyro.z(), e1.gyro.z());
    
    sax_el e2 = sxp.deserialize();
    EXPECT_EQ(el2.tid, e2.tid);

    EXPECT_EQ(el2.v.x(), e2.v.x());
    EXPECT_EQ(el2.v.y(), e2.v.y());
    EXPECT_EQ(el2.v.z(), e2.v.z());

    EXPECT_EQ(el2.cp.x(), e2.cp.x());
    EXPECT_EQ(el2.cp.y(), e2.cp.y());
    EXPECT_EQ(el2.cp.z(), e2.cp.z());

    EXPECT_EQ(el2.dp.x(), e2.dp.x());
    EXPECT_EQ(el2.dp.y(), e2.dp.y());
    EXPECT_EQ(el2.dp.z(), e2.dp.z());

    EXPECT_EQ(el2.acc.x(), e2.acc.x());
    EXPECT_EQ(el2.acc.y(), e2.acc.y());
    EXPECT_EQ(el2.acc.z(), e2.acc.z());

    EXPECT_EQ(el2.mag.x(), e2.mag.x());
    EXPECT_EQ(el2.mag.y(), e2.mag.y());
    EXPECT_EQ(el2.mag.z(), e2.mag.z());

    EXPECT_EQ(el2.gyro.x(), e2.gyro.x());
    EXPECT_EQ(el2.gyro.y(), e2.gyro.y());
    EXPECT_EQ(el2.gyro.z(), e2.gyro.z()); 
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}