#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../TeamProject_SSD/ShellTestApp.cpp"

class MockSSD : public SSD {
public:
    MOCK_METHOD(unsigned int, Read, (unsigned int), (override));
    MOCK_METHOD(void, Write, (unsigned int, unsigned int), (override));
};

class MockTestScript : public ITestScript {
public:
    MOCK_METHOD(bool, DoScript, (), (override));
};

class ShellTestAppFixture : public testing::Test {
public:
    void SetUp() override {
        pApp = new ShellTestApp(&mSsd);
    }

    ShellTestApp* pApp;
    MockSSD mSsd;
    MockTestScript mTestScript;

    const unsigned int LBA = 0;
    const unsigned int MAX_LBA_NUM = 100; //todo get MAX_LBA_NUM from SSD
    const unsigned int DATA = 0xFFFFFFFF;
    const unsigned int INVALID_DATA = 0X00000000;
    const std::string TESTSCRIPT1 = "testscriptapp2";
    const std::string TESTSCRIPT2 = "testscriptapp1";
};

TEST_F(ShellTestAppFixture, writeSuccessTest) {
    EXPECT_CALL(mSsd, Write(LBA, DATA))
        .Times(3);
    pApp->write(LBA, DATA);
}

TEST_F(ShellTestAppFixture, writeOverLbaFailTest) {
    exception err;
    EXPECT_CALL(mSsd, Write(MAX_LBA_NUM, DATA))
        .WillOnce(testing::Throw(err));

    pApp->Write(MAX_LBA_NUM, DATA);
}

TEST_F(ShellTestAppFixture, writeInvalidDataFailTest) {
    exception err;
    EXPECT_CALL(mSsd, Write(LBA, INVALID_DATA))
        .WillOnce(testing::Throw(err));

    pApp->Write(LBA, INVALID_DATA);
}
