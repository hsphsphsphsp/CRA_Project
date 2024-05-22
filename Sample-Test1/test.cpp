#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../TeamProject_SSD/ssd.cpp"
#include "../TeamProject_SSD/testscript.cpp"

using namespace std;
using namespace testing;

class MockSSD : public SSD {
public:
	MOCK_METHOD(unsigned int, Read, (unsigned int), (override));
	MOCK_METHOD(void, Write, (unsigned int, unsigned int), (override));
};

class TestScriptApp1Fixture : public Test
{
public:
	NiceMock<MockSSD> mockSsd;
};

TEST_F(TestScriptApp1Fixture, TestScriptApp1_ConfirmCallFullWrite) {
	TestScriptApp1 testScriptApp1(&mockSsd);
	
	EXPECT_CALL(mockSsd, Write)
		.Times(mockSsd.nSize);
	testScriptApp1.DoScript();
}

TEST_F(TestScriptApp1Fixture, TestScriptApp1_ConfirmCallFullRead) {
	TestScriptApp1 testScriptApp1(&mockSsd);

	EXPECT_CALL(mockSsd, Read)
		.Times(mockSsd.nSize);
	testScriptApp1.DoScript();
}

TEST_F(TestScriptApp1Fixture, TestScriptApp1_FailReadVerify) {
	TestScriptApp1 testScriptApp1(&mockSsd);

	EXPECT_CALL(mockSsd, Read)
		.WillOnce(Return(0x0))
		.WillOnce(Return(0x0))
		.WillRepeatedly(Return(0xFF));

	EXPECT_THAT(testScriptApp1.DoScript(), Eq(false));
}

TEST(SSDTest, ReadLBANeverBeenWritten) {
	unsigned int address = 0x0;
	unsigned int ret = 0x00000000;
	SSD ssd;

	EXPECT_EQ(ret, ssd.Read(address));
}