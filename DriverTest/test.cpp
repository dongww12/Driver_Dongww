#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../DeviceDriver/DeviceDriver.cpp"

using namespace testing;

class StubFlashMemoryDevice : public FlashMemoryDevice {
public:
	unsigned char read(long address) override {
		return 0;
	}
	void write(long address, unsigned char data) override {
		return;
	}
};

class MockFlashMemoryDevice : public StubFlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
};

class DriverTextFixture : public testing::Test {
public:
	long address = 0x01;
	int data = 7;
	MockFlashMemoryDevice device;
};

TEST_F(DriverTextFixture, Read) {
	EXPECT_CALL(device, read(address))
		.Times(AtLeast(5));

	DeviceDriver driver(&device);
	int actual = driver.read(address);
	int expected = 0;
	EXPECT_EQ(actual, expected);
}

TEST_F(DriverTextFixture, Read2) {
	EXPECT_CALL(device, read(address))
		.Times(AtLeast(5))
		.WillRepeatedly(Return(7));

	DeviceDriver driver(&device);
	int actual = driver.read(address);
	int expected = 7;
	EXPECT_EQ(actual, expected);
}

TEST_F(DriverTextFixture, ReadFail) {
	EXPECT_CALL(device, read(address))
		.WillOnce(Return(5))
		.WillRepeatedly(Return(7));

	DeviceDriver driver(&device);
	EXPECT_THROW(driver.read(address), ReadFailException);
}

TEST_F(DriverTextFixture, Write) {
	EXPECT_CALL(device, read(address))
		.WillRepeatedly(Return(0xFF));

	DeviceDriver driver(&device);
	driver.write(address, data);
	EXPECT_TRUE(true);
}

TEST_F(DriverTextFixture, WriteFail) {
	EXPECT_CALL(device, read(address))
		.WillRepeatedly(Return(0xF0));

	DeviceDriver driver(&device);
	EXPECT_THROW(driver.write(address, data), WriteFailException);
}