#pragma once
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../TeamProject_SSD/ssd.cpp"
#include "../TeamProject_SSD/testscript.cpp"
using namespace testing;

class MockSsd : public Ssd {
public:
	MOCK_METHOD(unsigned int, Read, (unsigned int), (override));
	MOCK_METHOD(void, Write, (unsigned int, unsigned int), (override));
};

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(TestScriptApp2, DefaultCallTest)
{
	MockSsd mSsd;
	TestScript* tScript;
	string sScriptName = "testscriptapp2";
	TestScriptFactory fTestScriptFactory;

	tScript = fTestScriptFactory.createScript(sScriptName,mSsd);
	EXPECT_CALL(mSsd, Read(2)).Times(1);
	tScript->doScript();
}