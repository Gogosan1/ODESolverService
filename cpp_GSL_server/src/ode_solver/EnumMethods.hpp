#pragma once
enum class Method
{
    rk2,
    rk4,
    rkf45,
    rkck, // в ошибку выпадает
    rk8pd,
    rk1imp,
    rk2imp,
    rk4imp,
    bsimp,
    msadams,
    msbdf,
    unknown
};