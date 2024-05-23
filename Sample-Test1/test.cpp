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

TEST_F(SSDFixture, WriteSDDNormalTest) {
	bool isFirstWrite = true;
	SSD ssd;
	ifstream fin;
	ofstream fout;
	string index, value;
	unordered_map<unsigned int, unsigned int> umExpectedDataSet;
	unordered_map<unsigned int, unsigned int> umActualDataSet;

	remove("nand.txt");

	umExpectedDataSet.insert({ 0, 0x1122AABB });
	umExpectedDataSet.insert({ 3, 0x11CCAABB });

	fout.open("nand.txt");

	for (const auto& pair : umExpectedDataSet) {
		fout << pair.first << " " << "0x" << hex << pair.second << endl;
	}

	fout.close();

	umExpectedDataSet.insert({ 17, 0x11AAFF44 });
	ssd.Write(17, 0x11AAFF44);

	fin.open("nand.txt");
	while (!fin.eof())
	{
		fin >> index >> value;
		umActualDataSet.insert({ stoi(index), stoi(value, nullptr, 16) });
	}
	fin.close();

	if (umExpectedDataSet.size() != umActualDataSet.size()) {
		cout << "Err: Size is different" << endl;
		FAIL();
	}

	for (const auto& pair : umExpectedDataSet) {
		auto it = umActualDataSet.find(pair.first);
		if (it == umActualDataSet.end() || it->second != pair.second) {
			cout << "Err: ActualDataSet has wrong value" << endl;
			FAIL();
		}
	}
}