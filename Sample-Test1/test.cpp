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

class SSDFixture : public testing::Test {
public:
	SSD ssd;
	const int INVALID_DATA = 0x00000000;
};

TEST_F(SSDFixture, ReadLBANeverBeenWritten) {
	unsigned int nAddr = 0x0;

	EXPECT_EQ(INVALID_DATA, ssd.Read(nAddr));
}

TEST_F(SSDFixture, ReadLBANeverBeenWrittenResultFile) {
	unsigned int nAddr = 0x0;
	string sFileName = "result.txt";
	
	ssd.Read(nAddr);

	std::ifstream inFile(sFileName);

	if (inFile) {
		unsigned int nValue = -1;
		inFile >> std::hex >> nValue;

		EXPECT_EQ(INVALID_DATA, nValue);
	}
	else {
		FAIL() << sFileName << " not exist.";
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