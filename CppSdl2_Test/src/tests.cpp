#include <gtest/gtest.h>

class Test : public ::testing::Test {
protected:

	Test() {}

	~Test() override {}

	void SetUp() override {}

	void TearDown() override {}
};

TEST_F(Test, someTest) {
	// Given.
	

	// Then.
	EXPECT_TRUE(true);
	EXPECT_EQ(0, 0);
}
