#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../TeamProject_SSD/ssd.cpp"

class MockSSD : public SSD {
public:
	MOCK_METHOD(unsigned int, Read, (unsigned int), (override));
	MOCK_METHOD(void, Write, (unsigned int, unsigned int), (override));
};

TEST(SSDTest, ReadLBANeverBeenWritten) {
	unsigned int address = 0x0;
	unsigned int ret = 0x00000000;
	SSD ssd;

	EXPECT_EQ(ret, ssd.Read(address));
}