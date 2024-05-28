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

	EXPECT_CALL(mockSSD, GetSSDSize())
		.WillRepeatedly(Return(100));

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
	DoScriptCommand cmd(&mockSSD, "testscriptapp1");
	
	EXPECT_CALL(mockSSD, GetSSDSize())
		.WillRepeatedly(Return(1));

	EXPECT_CALL(mockSSD, Read)
		.Times(AtLeast(1));

	cmd.execute();
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
	EXPECT_EQ(DEFAULT_READ_VALUE, ssd.Read(0));
}

TEST_F(SSDFixture, Read_CreateResultFile)
{
	ssd.Read(0);

	ifstream fin(sResultFileName);
	EXPECT_EQ(fin.is_open(), true);
}

TEST_F(SSDFixture, DISABLED_Read_ReadAfterWriteNormalValue)
{
	unsigned int nValue = 0xB622AABB;

	ssd.Write(LBA_0, nValue);
	EXPECT_EQ(nValue, ssd.Read(LBA_0));
}

TEST_F(SSDFixture, Read_InvalidLBA)
{
	EXPECT_THROW(ssd.Read(INVALID_LBA), exception);
}

TEST_F(SSDFixture, Write_InvalidLBA)
{
	unsigned int nValue = 0x1122AABB;
	EXPECT_THROW(ssd.Write(INVALID_LBA, nValue), exception);
}

TEST_F(SSDFixture, DISABLED_Write_OverwriteData)
{
	unsigned int nValue = 0xB622AABB;
	unsigned int nNewValue = 0xFFFFFFFF;

	ssd.Write(LBA_0, nValue);
	ssd.Write(LBA_0, nNewValue);

	EXPECT_THAT(nValue, Ne(ssd.Read(LBA_0)));
	EXPECT_THAT(nNewValue, Eq(ssd.Read(LBA_0)));
}

TEST_F(SSDFixture, Write_VerifyWriteFunctionWithRawFileData)
{
	ifstream fin;
	ofstream fout;
	string sIndex, sValue;
	unordered_map<unsigned int, unsigned int> umExpectedDataSet;
	unordered_map<unsigned int, unsigned int> umActualDataSet;

	umExpectedDataSet.insert({ 0, 0x1122AABB });
	umExpectedDataSet.insert({ 3, 0x11CCAABB });
	umExpectedDataSet.insert({ 7, 0xFFCCAABB });

	fout.open(sNANDFileName);

	for (const auto& pair : umExpectedDataSet) {
		fout << dec << pair.first << " " << "0x" << uppercase << hex << setw(8) << setfill('0') << pair.second << endl;
	}

	fout.close();

	umExpectedDataSet.insert({ 17, 0x11AAFF44 });
	ssd.Write(17, 0x11AAFF44);
	ssd.Flush();

	fin.open(sNANDFileName);
	while (!fin.eof())
	{
		fin >> sIndex >> sValue;
		umActualDataSet.insert({ stoi(sIndex), stoul(sValue, nullptr, 16) });
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

TEST_F(SSDFixture, DISABLED_Erase_EraseAfterWriteNormalValue)
{
	unsigned int nLBA1 = 0;
	unsigned int nData1 = 0xB622AABB;
	unsigned int nLBA2 = 5;
	unsigned int nData2 = 0xFF15DDCC;

	ssd.Write(nLBA1, nData1);
	ssd.Write(nLBA2, nData2);
	EXPECT_EQ(nData1, ssd.Read(nLBA1));
	ssd.Erase(nLBA1, 1);
	EXPECT_EQ(DEFAULT_READ_VALUE, ssd.Read(nLBA1));
	EXPECT_EQ(nData2, ssd.Read(nLBA2));
}

TEST_F(SSDFixture, DISABLED_Erase_EraseRangeAfterWriteNormalValue)
{
	unsigned int nLBA1 = 0;
	unsigned int nData1 = 0xB622AABB;
	unsigned int nLBA2 = 5;
	unsigned int nData2 = 0xFF15DDCC;

	ssd.Write(nLBA1, nData1);
	ssd.Write(nLBA2, nData2);
	EXPECT_EQ(nData1, ssd.Read(nLBA1));
	ssd.Erase(nLBA1, 6);
	EXPECT_EQ(DEFAULT_READ_VALUE, ssd.Read(nLBA2));
}

TEST_F(SSDFixture, CommandBuffer_SimpleWriteCommandIssueTest)
{
	unsigned int nValue = 0xB622AABB;
	tuple<int, unsigned int, unsigned int> tExpected = { W, LBA_0, nValue };

	ssd.Write(LBA_0, nValue);

	nCmdBuffer = LoadCmdBuffer();
	auto it = nCmdBuffer.begin();
	tuple<int, unsigned int, unsigned int> tActual = { it->first.first , it->first.second, it->second };
	
	EXPECT_EQ(tExpected, tActual);
}

TEST_F(SSDFixture, CommandBuffer_SimpleEraseCommandIssueTest)
{
	unsigned int nSize = 5;
	tuple<int, unsigned int, unsigned int> tExpected = { E, LBA_0, nSize };

	ssd.Erase(LBA_0, nSize);
	
	nCmdBuffer = LoadCmdBuffer();
	auto it = nCmdBuffer.begin();
	tuple<int, unsigned int, unsigned int> tActual = { it->first.first , it->first.second, it->second };

	EXPECT_EQ(tExpected, tActual);
}

TEST_F(SSDFixture, CommandBuffer_IgnorePreviousWriteWhenWriteIssued)
{
	unsigned int nFirstWriteValue = 0x00000001;
	unsigned int nSecondWriteValue = 0xFFFFFFFF;

	ssd.Write(LBA_0, nFirstWriteValue);
	ssd.Write(LBA_0, nSecondWriteValue);

	nCmdBuffer = LoadCmdBuffer();

	EXPECT_EQ(nCmdBuffer.size(), 1);

	unsigned int nExpectedValue = nCmdBuffer[{ W, LBA_0 }];
	EXPECT_EQ(nExpectedValue, nSecondWriteValue);
}

TEST_F(SSDFixture, CommandBuffer_ReadFromCommandBuffer)
{
	unsigned int nValue = 0xFFFFFFFF;

	ssd.Write(LBA_0, nValue);
	unsigned int nEpectedValue = ssd.Read(LBA_0);

	EXPECT_EQ(nValue, nEpectedValue);

	//Do not create nand.txt file because writed at command buffer (buffer.txt)
	ifstream fin(sNANDFileName);
	EXPECT_EQ(fin.is_open(), false);
}

TEST_F(SSDFixture, CommandBuffer_RemoveWriteCommandWhenEraseIssued)
{
	ssd.Write(0, 0xFFFFFFFF);
	ssd.Write(1, 0xFFFFFFFF);
	ssd.Write(2, 0xFFFFFFFF);
	ssd.Erase(0, 2);

	nCmdBuffer = LoadCmdBuffer();

	EXPECT_EQ(nCmdBuffer.size(), 2);
}

TEST_F(SSDFixture, CommandBuffer_MergeEraseCommand)
{
	ssd.Write(20, 0xFFFFFFFF);
	ssd.Erase(10, 2);
	ssd.Erase(12, 3);

	nCmdBuffer = LoadCmdBuffer();

	EXPECT_EQ(nCmdBuffer.size(), 2);
}

TEST_F(SSDFixture, CommandBuffer_MergeEraseCommandReverse)
{
	ssd.Write(20, 0xFFFFFFFF);
	ssd.Erase(12, 3);
	ssd.Erase(10, 2);

	nCmdBuffer = LoadCmdBuffer();

	EXPECT_EQ(nCmdBuffer.size(), 2);
}

TEST_F(SSDFixture, CommandBuffer_MergeEraseCommandDoNotMerge)
{
	ssd.Write(20, 0xFFFFFFFF);
	ssd.Erase(10, 1);
	ssd.Erase(12, 3);

	nCmdBuffer = LoadCmdBuffer();

	EXPECT_EQ(nCmdBuffer.size(), 3);
}

TEST_F(SSDFixture, CommandBuffer_MergeSeveralEraseCommand)
{
	ssd.Write(20, 0xFFFFFFFF);
	ssd.Erase(10, 1);
	ssd.Erase(12, 3);
	ssd.Erase(15, 2);
	ssd.Erase(19, 2);

	nCmdBuffer = LoadCmdBuffer();

	EXPECT_EQ(nCmdBuffer.size(), 3);
}

TEST_F(ShellTestAppFixture, writeSuccessTest) {
	WriteCommand cmd(&mSsd, LBA, DATA);

	EXPECT_CALL(mSsd, Write(LBA, DATA))
		.Times(1);
	
	cmd.execute();
}

TEST_F(ShellTestAppFixture, writeOverLbaFailTest) {
	WriteCommand cmd(&mSsd, MAX_LBA_NUM, DATA);

	EXPECT_CALL(mSsd, Write(MAX_LBA_NUM, DATA))
		.WillOnce(testing::Throw(ERREXCEPTION));

	try {
		cmd.execute();
		FAIL();
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}
}

TEST_F(ShellTestAppFixture, writeInvalidDataFailTest) {
	WriteCommand cmd(&mSsd, LBA, INVALID_DATA);

	EXPECT_CALL(mSsd, Write(LBA, INVALID_DATA))
		.WillOnce(testing::Throw(ERREXCEPTION));

	try {
		cmd.execute();
		FAIL();
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}
}

TEST_F(ShellTestAppFixture, readSuccessTest) {
	ReadCommand cmd(&mSsd, LBA);

	EXPECT_CALL(mSsd, Read(LBA))
		.Times(1);

	cmd.execute();
}

TEST_F(ShellTestAppFixture, readInvalidDataFailTest) {
	ReadCommand cmd(&mSsd, MAX_LBA_NUM);

	EXPECT_CALL(mSsd, Read(MAX_LBA_NUM))
		.WillOnce(testing::Throw(ERREXCEPTION));

	try {
		cmd.execute();
		FAIL();
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}
}

TEST_F(ShellTestAppFixture, eraseSuccessTest) {
	EraseCommand cmd(&mSsd, LBA, SIZE);

	EXPECT_CALL(mSsd, Erase(LBA, SIZE))
		.Times(1);

	cmd.execute();
}

TEST_F(ShellTestAppFixture, DISABLED_exitTest) {
	ExitCommand cmd(&mSsd);

	cmd.execute();
}

TEST_F(ShellTestAppFixture, helpTest) {
	HelpCommand cmd(&mSsd);

	EXPECT_CALL(mSsd, GetSSDSize())
		.Times(1)
		.WillRepeatedly(Return(MAX_LBA_NUM));

	cmd.execute();
}

TEST_F(ShellTestAppFixture, fullReadSuccessTest) {
	FullReadCommand cmd(&mSsd);

	EXPECT_CALL(mSsd, GetSSDSize())
		.WillRepeatedly(Return(MAX_LBA_NUM));
	EXPECT_CALL(mSsd, Read(_))
		.Times(MAX_LBA_NUM);

	cmd.execute();
}

TEST_F(ShellTestAppFixture, fullReadFailTest) {
	FullReadCommand cmd(&mSsd);

	EXPECT_CALL(mSsd, GetSSDSize())
		.WillRepeatedly(Return(MAX_LBA_NUM));
	EXPECT_CALL(mSsd, Read(_))
		.Times(1)
		.WillOnce(testing::Throw(ERREXCEPTION));
	
	try {
		cmd.execute();
		FAIL();
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}
}

TEST_F(ShellTestAppFixture, fullWriteSuccessTest) {
	FullWriteCommand cmd(&mSsd, DATA);

	EXPECT_CALL(mSsd, GetSSDSize())
		.WillRepeatedly(Return(MAX_LBA_NUM));
	EXPECT_CALL(mSsd, Write(_, DATA))
		.Times(MAX_LBA_NUM);

	cmd.execute();
}

TEST_F(ShellTestAppFixture, fullWriteFailTest) {
	FullWriteCommand cmd(&mSsd, INVALID_DATA);

	EXPECT_CALL(mSsd, GetSSDSize())
		.WillRepeatedly(Return(MAX_LBA_NUM));
	EXPECT_CALL(mSsd, Write(_, INVALID_DATA))
		.Times(1)
		.WillOnce(testing::Throw(ERREXCEPTION));

	try {
		cmd.execute();
		FAIL();
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}
}
