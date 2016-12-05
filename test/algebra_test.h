#ifndef ALGEBRA_TEST_H
#define ALGEBRA_TEST_H

#include <stdexcept>
#include <physics/algebra.h>

using namespace physics;

TEST(PointTest, creation)
{
    {
        const value_t v = constants::NegativeUnit;
        ASSERT_THROW(Point(v, v), std::invalid_argument);
    }
    {
        const value_t v = constants::PositiveUnit;
        ASSERT_NO_THROW(Point(v, v));
    }
}

TEST(PointTest, distance)
{
    const Point pt1, pt2(3., 4.);
    const value_t d = distance(pt1, pt2);

    ASSERT_FALSE(same(d, 6.));
    ASSERT_TRUE(same(d, 5.));
}

TEST(VectorTest, summation)
{
    const Vector v1(3., 0.);
    const Vector v2(0., 4.);
    const Vector v3 = v1 + v2;

    ASSERT_EQ(v3.rx(), 3.);
    ASSERT_EQ(v3.ry(), 4.);
}

TEST(VectorTest, multiplication)
{
    const Vector v1(3., 4.);
    const value_t scalar = 5;

    const Vector v2 = scalar * v1;
    const Vector v3 = v1 * scalar;

    const value_t rxResult = v1.rx() * scalar;
    const value_t ryResult = v1.ry() * scalar;

    ASSERT_EQ(v2.rx(), rxResult);
    ASSERT_EQ(v2.ry(), ryResult);

    ASSERT_EQ(v3.rx(), rxResult);
    ASSERT_EQ(v3.ry(), ryResult);
}

TEST(VectorTest, normalize)
{
    const value_t l = length(normalize(Vector(3., 4.)));
    ASSERT_TRUE(same(l, constants::PositiveUnit));
}

TEST(VectorTest, length)
{
    const value_t l = length(Vector(3., 4.));

    ASSERT_FALSE(same(l, 6.));
    ASSERT_TRUE(same(l, 5.));
}

#endif // ALGEBRA_TEST_H
