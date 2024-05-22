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
	void SetUp() override
	{
		pTestScript = fTestScriptFactory.createScript(sScriptApp1, mockSSD);
	}

	NiceMock<MockSSD> mockSSD;
	TestScriptFactory fTestScriptFactory;
	TestScript* pTestScript;

	string sScriptApp1 = "testscriptapp1";
};

TEST_F(TestScriptApp1Fixture, TestScriptApp1_ConfirmCallFullWrite) {
	EXPECT_CALL(mockSSD, Write)
		.Times(pTestScript->GetSSDSize());
	pTestScript->DoScript();
}

TEST_F(TestScriptApp1Fixture, TestScriptApp1_ConfirmCallFullRead) {
	EXPECT_CALL(mockSSD, Read)
		.Times(pTestScript->GetSSDSize());
	pTestScript->DoScript();
}

TEST_F(TestScriptApp1Fixture, TestScriptApp1_FailReadVerify) {
	EXPECT_CALL(mockSSD, Read)
		.WillOnce(Return(0x0))
		.WillOnce(Return(0x0))
		.WillRepeatedly(Return(0xFF));

	EXPECT_THAT(pTestScript->DoScript(), Eq(false));
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