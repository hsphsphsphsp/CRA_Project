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

class TestScriptAppFixture : public Test
{
public:
	const string SCRIPT_APP1 = "testscriptapp1";
	const string SCRIPT_APP2 = "testscriptapp2";

	NiceMock<MockSSD> mockSSD;
	TestScriptFactory fTestScriptFactory;
	TestScript* pTestScript;
protected:
	void MakeScript(string scriptName)
	{
		pTestScript = fTestScriptFactory.createScript(scriptName, mockSSD);
	}
	
};

TEST_F(TestScriptAppFixture, TestScriptApp1_ConfirmCallFullWrite) {
	MakeScript(SCRIPT_APP1);

	EXPECT_CALL(mockSSD, Write)
		.Times(pTestScript->GetSSDSize());

	pTestScript->DoScript();
}

TEST_F(TestScriptAppFixture, TestScriptApp1_ConfirmCallFullRead) {
	MakeScript(SCRIPT_APP1);

	EXPECT_CALL(mockSSD, Read)
		.Times(pTestScript->GetSSDSize());

	pTestScript->DoScript();
}

TEST_F(TestScriptAppFixture, TestScriptApp1_FailReadVerify) {
	MakeScript(SCRIPT_APP1);

	EXPECT_CALL(mockSSD, Read)
		.WillOnce(Return(0x0))
		.WillOnce(Return(0x0))
		.WillRepeatedly(Return(0xFF));

	EXPECT_THAT(pTestScript->DoScript(), Eq(false));
}

TEST_F(TestScriptAppFixture, TestDefaultReturnTrue)
{
	MakeScript(SCRIPT_APP2);
	EXPECT_THAT(pTestScript->DoScript(), Eq(true));
}
TEST_F(TestScriptAppFixture, TestBodyCall)
{
	MakeScript(SCRIPT_APP2);
	EXPECT_CALL(mockSSD, Read(2)).Times(1);
	pTestScript->DoScript();
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