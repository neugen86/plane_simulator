#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "utils/types.h"

namespace physics
{
namespace constants
{
static const types::value_t Zero(0.);
static const types::value_t PositiveUnit(1.);
static const types::value_t NegativeUnit(-1.);
static const types::value_t DefaultMass(PositiveUnit);
static const types::value_t DefaultRadius(PositiveUnit);
} // namespace constants
} // namespace physics

#endif // CONSTANTS_H
