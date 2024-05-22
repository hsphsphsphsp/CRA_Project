#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../TeamProject_SSD/ssd.cpp"
#include "../TeamProject_SSD/testscript.cpp"

using namespace std;
using namespace testing;

class MockSsd : public Ssd {
public:
	MOCK_METHOD(unsigned int, Read, (unsigned int), (override));
	MOCK_METHOD(void, Write, (unsigned int, unsigned int), (override));
};

TEST(TestScriptApp1, TestScriptApp1_ConfirmCallFullWrite) {
	NiceMock<MockSsd> mockSsd;
	TestScriptApp1 testScriptApp1(&mockSsd);
	
	EXPECT_CALL(mockSsd, Write)
		.Times(mockSsd.nSize);
	testScriptApp1.DoScript();
}
TEST(TestScriptApp1, TestScriptApp1_ConfirmCallFullRead) {
	NiceMock<MockSsd> mockSsd;
	TestScriptApp1 testScriptApp1(&mockSsd);

	EXPECT_CALL(mockSsd, Read)
		.Times(mockSsd.nSize);
	testScriptApp1.DoScript();
}
TEST(TestScriptApp1, TestScriptApp1_FailReadVerify) {
	NiceMock<MockSsd> mockSsd;
	TestScriptApp1 testScriptApp1(&mockSsd);

	EXPECT_CALL(mockSsd, Read)
		.WillOnce(Return(0x0))
		.WillOnce(Return(0x0))
		.WillRepeatedly(Return(0xFF));

	EXPECT_THAT(testScriptApp1.DoScript(), Eq(false));
}