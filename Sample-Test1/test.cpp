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
	TestScriptFactory fTestScriptFactory;
};

TEST_F(TestScriptApp1Fixture, TestScriptApp1_ConfirmCallFullWrite) {
	TestScriptApp1 testScriptApp1(&mockSsd);
	
	EXPECT_CALL(mockSsd, Write)
		.Times(testScriptApp1.nSSDSize);
	testScriptApp1.DoScript();
}

TEST_F(TestScriptApp1Fixture, TestScriptApp1_ConfirmCallFullRead) {
	TestScriptApp1 testScriptApp1(&mockSsd);

	EXPECT_CALL(mockSsd, Read)
		.Times(testScriptApp1.nSSDSize);
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

TEST_F(TestScriptApp1Fixture, TestScriptApp1_Factory) {
	TestScript* pTestScriptApp1 = fTestScriptFactory.createScript("testscriptapp1", mockSsd);

	EXPECT_CALL(mockSsd, Read)
		.WillOnce(Return(0x0))
		.WillRepeatedly(Return(0xFF));

	EXPECT_THAT(pTestScriptApp1->DoScript(), Eq(false));

	EXPECT_CALL(mockSsd, Read)
		.WillRepeatedly(Return(0));

	EXPECT_THAT(pTestScriptApp1->DoScript(), Eq(true));
}

TEST(TestScriptApp2, TestDefaultReturnTrue)
{
	MockSSD mSsd;
	TestScript* tScript;
	string sScriptName = "testscriptapp2";
	TestScriptFactory fTestScriptFactory;

	tScript = fTestScriptFactory.createScript(sScriptName, mSsd);
	EXPECT_THAT(tScript->DoScript(), Eq(true));
}
TEST(TestScriptApp2, TestBodyCall)
{
	MockSSD mSsd;
	TestScript* tScript;
	string sScriptName = "testscriptapp2";
	TestScriptFactory fTestScriptFactory;

	tScript = fTestScriptFactory.createScript(sScriptName, mSsd);
	EXPECT_CALL(mSsd, Read(2)).Times(1);
	tScript->DoScript();
}

TEST(TestScriptApp2, TestScriptFactoryNull)
{
	MockSSD mSsd;
	TestScript* tScript;
	string sScriptName = "testscriptapp33";
	TestScriptFactory fTestScriptFactory;

	tScript = fTestScriptFactory.createScript(sScriptName, mSsd);
	EXPECT_THAT(tScript, Eq(nullptr));
}

class SSDFixture : public testing::Test {
public:
	SSD ssd;

	const int INVALID_DATA = 0x00000000;
};

TEST_F(SSDFixture, ReadLBANeverBeenWritten) {
	unsigned int nAddr = 0x0;

	EXPECT_EQ(INVALID_DATA, ssd.Read(nAddr));
}