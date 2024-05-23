#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <fstream>
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

class TestScriptApp2Fixture : public Test
{
public:
	NiceMock<MockSSD> mockSsd;
	TestScript* tScript;
	TestScriptFactory fTestScriptFactory;
private:
	void SetUp() override
	{
		tScript = fTestScriptFactory.createScript(sScriptName, mockSsd);
	}

	string sScriptName = "testscriptapp2";
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

TEST_F(TestScriptApp2Fixture, TestDefaultReturnTrue)
{
	EXPECT_THAT(tScript->DoScript(), Eq(true));
}
TEST_F(TestScriptApp2Fixture, TestBodyCall)
{
	EXPECT_CALL(mockSsd, Read(2)).Times(1);
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

class SSDFixture : public testing::Test 
{
public:
	void SetUp() override 
	{
		fResultFile.open(sResultFileName);
	}

	void TearDown() override 
	{
		if (fResultFile.is_open()) 
		{
			fResultFile.close();
		}
	}

	SSD ssd;
	const int INVALID_DATA = 0x00000000;
	string sResultFileName = "result.txt";
	ifstream fResultFile;
};

TEST_F(SSDFixture, ReadLBANeverBeenWritten) 
{
	EXPECT_EQ(INVALID_DATA, ssd.Read(0));
}

TEST_F(SSDFixture, ReadLBANeverBeenWrittenResultFile) 
{
	ssd.Read(0);

	if (fResultFile) 
	{
		unsigned int nValue = -1;
		fResultFile >> hex >> nValue;

		EXPECT_EQ(INVALID_DATA, nValue);
	}
	else 
	{
		FAIL() << sResultFileName << " not exist.";
	}
}
