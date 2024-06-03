#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../DeviceDriver/DeviceDriver.cpp"

using namespace testing;

class MockFlashMemoryDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DriverTextFixture : public testing::Test {
public:
	long address = 0x01;
	int data = 7;
	MockFlashMemoryDevice device;

	DeviceDriver driver{ &device };
};

TEST_F(DriverTextFixture, Read) {
	EXPECT_CALL(device, read)
		.Times(5);

	EXPECT_EQ(driver.read(address), 0);
}

TEST_F(DriverTextFixture, Read2) {
	constexpr int returnValure = 7;
	EXPECT_CALL(device, read(address))
		.Times(AtLeast(5))
		.WillRepeatedly(Return(returnValure));

	EXPECT_EQ(driver.read(address), returnValure);
}

TEST_F(DriverTextFixture, ReadFail) {
	constexpr int returnValure = 7;
	EXPECT_CALL(device, read(address))
		.WillOnce(Return(5))
		.WillRepeatedly(Return(returnValure));
	
	EXPECT_THROW(driver.read(address), ReadFailException);
}

TEST_F(DriverTextFixture, Write) {
	EXPECT_CALL(device, read(address))
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(device, write(address, _));

	driver.write(address, data);
	EXPECT_TRUE(true);
}

TEST_F(DriverTextFixture, WriteFail) {
	EXPECT_CALL(device, read(address))
		.WillRepeatedly(Return(0xF0));

	EXPECT_THROW(driver.write(address, data), WriteFailException);
}