#include "gtest/gtest.h"
#include "qudottest.h"

TEST(ShorTest, factor15test) {
    ASSERT_TRUE(qudot::shorTest("../resources/compiled/shor/shor15.qudotc", 15, 7, 8, 4));
}

TEST(ShorTest, factor21test) {
    ASSERT_TRUE(qudot::shorTest("../resources/compiled/shor/shor21.qudotc", 21, 11, 9, 5));
}

TEST(ShorTest, factor33test) {
    ASSERT_TRUE(qudot::shorTest("../resources/compiled/shor/shor33.qudotc", 33, 26, 11, 6));
}

TEST(ShorTest, factor35test) {
    ASSERT_TRUE(qudot::shorTest("../resources/compiled/shor/shor35.qudotc", 35, 11, 11, 6));
}

TEST(ShorTest, factor77test) {
    ASSERT_TRUE(qudot::shorTest("../resources/compiled/shor/shor77.qudotc", 77, 69, 13, 7));
}