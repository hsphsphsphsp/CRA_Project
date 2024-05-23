#include "test.h"

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

TEST_F(TestScriptAppFixture, TestScriptFactoryNull)
{
	MakeScript(SCRIPT_INVALID);
	EXPECT_THAT(pTestScript, Eq(nullptr));
}

TEST(TestScript, TestShellCallTestScript)
{
	NiceMock<MockSSD> mockSSD;
	ShellTestApp shellTestApp(&mockSSD);
	
	EXPECT_CALL(mockSSD, Read)
		.Times(AtLeast(1));

	shellTestApp.DoScript("testscriptapp1");
}

TEST_F(TestScriptAppFixture, TestScriptApp2_CheckWriteAgingExecutedCount)
{
	MakeScript(SCRIPT_APP2);
	unsigned int nValue = 0xAAAABBBB;
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
		EXPECT_CALL(mockSSD, Write(nLba, nValue)).Times(30);
	nValue = 0x12345678;
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
		EXPECT_CALL(mockSSD, Write(nLba, nValue)).Times(1);
	pTestScript->DoScript();
}

TEST_F(TestScriptAppFixture, TestScriptApp2_CheckWhenVerifySuccess)
{
	MakeScript(SCRIPT_APP2);
	unsigned int nValue = 0x12345678;
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
	{
		EXPECT_CALL(mockSSD, Read).WillRepeatedly(Return(nValue));
	}
	EXPECT_THAT(pTestScript->DoScript(), Eq(true));
}

TEST_F(TestScriptAppFixture, TestScriptApp2_CheckWhenVerifyFail)
{
	MakeScript(SCRIPT_APP2);
	unsigned int nValue = 0xAAAABBBB;
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
	{
		EXPECT_CALL(mockSSD, Read(nLba)).WillRepeatedly(Return(nValue));
	}
	EXPECT_THAT(pTestScript->DoScript(), Eq(false));
}

TEST_F(SSDFixture, Read_LBANeverBeenWritten)
{
	EXPECT_EQ(INVALID_DATA, ssd.Read(0));
}

TEST_F(SSDFixture, Read_CreateResultFile)
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

TEST_F(SSDFixture, Read_ReadAfterWrite)
{
	unsigned int nAddr = 0;
	unsigned int nData = 0xB622AABB;

	ssd.Write(nAddr, nData);
	EXPECT_EQ(nData, ssd.Read(nAddr));
}

TEST_F(SSDFixture, Read_InvalidLBA)
{
	EXPECT_THROW(ssd.Read(INVALID_LBA), exception);
}

TEST_F(SSDFixture, Write_InvalidLBA)
{
	unsigned int nData = 0x1122AABB;
	EXPECT_THROW(ssd.Write(INVALID_LBA, nData), exception);
}

TEST_F(SSDFixture, Write_OverwriteData)
{
	unsigned int nAddr = 0;
	unsigned int nData = 0xB622AABB;
	unsigned int nNewData = 0xFFFFFFFF;

	ssd.Write(nAddr, nData);
	ssd.Write(nAddr, nNewData);

	EXPECT_THAT(nData, Ne(ssd.Read(nAddr)));
	EXPECT_THAT(nNewData, Eq(ssd.Read(nAddr)));
}

TEST_F(SSDFixture, WriteSDDNormalTest) {
	SSD ssd;
	ifstream fin;
	ofstream fout;
	string strIndex, strValue;
	unordered_map<unsigned int, unsigned int> umExpectedDataSet;
	unordered_map<unsigned int, unsigned int> umActualDataSet;

	umExpectedDataSet.insert({ 0, 0x1122AABB });
	umExpectedDataSet.insert({ 3, 0x11CCAABB });

	fout.open("nand.txt");

	for (const auto& pair : umExpectedDataSet) {
		fout << dec << pair.first << " " << "0x" << uppercase << hex << setw(8) << setfill('0') << pair.second << endl;
	}

	fout.close();

	umExpectedDataSet.insert({ 17, 0x11AAFF44 });
	ssd.Write(17, 0x11AAFF44);

	fin.open("nand.txt");
	while (!fin.eof())
	{
		fin >> strIndex >> strValue;
		umActualDataSet.insert({ stoi(strIndex), stoi(strValue, nullptr, 16) });
	}
	fin.close();

	EXPECT_EQ(umExpectedDataSet.size(), umActualDataSet.size());

	for (const auto& pair : umExpectedDataSet) {
		auto it = umActualDataSet.find(pair.first);
		if (it == umActualDataSet.end() || it->second != pair.second) {
			cout << "Err: ActualDataSet has wrong value" << endl;
			FAIL();
		}
	}
}