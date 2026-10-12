#include <gtest/gtest.h>

#include "marker_array.h"

TEST(MarkerArrayTest, NewArrayIsEmpty) {
    MarkerArray array(5);
    EXPECT_EQ(array.size(), 5);
    EXPECT_EQ(array.countMarked(), 0);
    for (int i = 0; i < array.size(); i++) {
        EXPECT_TRUE(array.isFree(i));
    }
}

TEST(MarkerArrayTest, TryMarkMarksFreeCell) {
    MarkerArray array(3);
    EXPECT_TRUE(array.tryMark(1, 7));
    EXPECT_FALSE(array.isFree(1));
    EXPECT_EQ(array.countMarkedBy(7), 1);
}

TEST(MarkerArrayTest, TryMarkFailsOnOccupiedCell) {
    MarkerArray array(3);
    ASSERT_TRUE(array.tryMark(0, 1));
    EXPECT_FALSE(array.tryMark(0, 2));
    EXPECT_EQ(array.countMarkedBy(1), 1);
    EXPECT_EQ(array.countMarkedBy(2), 0);
}

TEST(MarkerArrayTest, CountMarkedByCountsOnlyOwnCells) {
    MarkerArray array(5);
    array.tryMark(0, 1);
    array.tryMark(1, 2);
    array.tryMark(2, 1);
    array.tryMark(4, 3);
    EXPECT_EQ(array.countMarkedBy(1), 2);
    EXPECT_EQ(array.countMarkedBy(2), 1);
    EXPECT_EQ(array.countMarkedBy(3), 1);
    EXPECT_EQ(array.countMarkedBy(4), 0);
}

TEST(MarkerArrayTest, ClearMarksByMarkerKeepsOthers) {
    MarkerArray array(4);
    array.tryMark(0, 1);
    array.tryMark(1, 2);
    array.tryMark(2, 1);

    array.clearMarksBy(1);

    EXPECT_EQ(array.countMarkedBy(1), 0);
    EXPECT_EQ(array.countMarkedBy(2), 1);
    EXPECT_TRUE(array.isFree(0));
    EXPECT_FALSE(array.isFree(1));
    EXPECT_TRUE(array.isFree(2));
}

TEST(MarkerArrayTest, ToStringShowsArrayContent) {
    MarkerArray array(3);
    array.tryMark(1, 5);
    EXPECT_EQ(array.toString(), "0 5 0");
}
