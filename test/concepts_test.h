#ifndef CONCEPTS_TEST_H
#define CONCEPTS_TEST_H

#include <stdexcept>
#include "physics/concepts.h"

using namespace physics;

TEST(ObjectTest, creation)
{
    {
        const types::value_t v = constants::Zero;
        ASSERT_THROW(Object(v, v), std::invalid_argument);
        ASSERT_THROW(Object(Point(), v, v), std::invalid_argument);
    }
    {
        const types::value_t v = constants::PositiveUnit;
        ASSERT_NO_THROW(Object(v, v));
        ASSERT_NO_THROW(Object(Point(), v, v));
    }
}


#endif // CONCEPTS_TEST_H
