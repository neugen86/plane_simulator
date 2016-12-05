#include <gtest/gtest.h>

#include "algebra_test.h"
#include "concepts_test.h"
#include "scene_test.h"

GTEST_API_ int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
