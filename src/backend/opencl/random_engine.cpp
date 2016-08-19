/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/dim4.hpp>
#include <Array.hpp>
#include <kernel/random_engine.hpp>
#include <cassert>
#include <MersenneTwister.hpp>

using common::MtStateLength;
using common::MaxBlocks;
using common::MersenneN;

namespace opencl
{
    Array<uint> initMersenneState(const uintl seed, const Array<uint> tbl)
    {
        Array<uint> state = createEmptyArray<uint>(MtStateLength);
        kernel::initMersenneState(*state.get(), *tbl.get(), seed);
        return state;
    }

    template<typename T>
    Array<T> uniformDistribution(const af::dim4 &dims, const af_random_type type, const uintl &seed, uintl &counter)
    {
        Array<T> out = createEmptyArray<T>(dims);
        kernel::uniformDistribution<T>(*out.get(), out.elements(), type, seed, counter);
        return out;
    }

    template<typename T>
    Array<T> normalDistribution(const af::dim4 &dims, const af_random_type type, const uintl &seed, uintl &counter)
    {
        Array<T> out = createEmptyArray<T>(dims);
        kernel::normalDistribution<T>(*out.get(), out.elements(), type, seed, counter);
        return out;
    }

    template<typename T>
    Array<T> uniformDistribution(const af::dim4 &dims,
            Array<uint> pos, Array<uint> sh1, Array<uint> sh2, uint mask,
            Array<uint> recursion_table, Array<uint> temper_table, Array<uint> state)
    {
        Array<T> out = createEmptyArray<T>(dims);
        kernel::uniformDistribution<T>(
              *out.get(), out.elements(),
              *state.get(), *pos.get(),
              *sh1.get(), *sh2.get(),
              mask, *recursion_table.get(),
              *temper_table.get());
        return out;
    }

    template<typename T>
    Array<T> normalDistribution(const af::dim4 &dims,
            Array<uint> pos, Array<uint> sh1, Array<uint> sh2, uint mask,
            Array<uint> recursion_table, Array<uint> temper_table, Array<uint> state)
    {
        Array<T> out = createEmptyArray<T>(dims);
        kernel::normalDistribution<T>(
              *out.get(), out.elements(),
              *state.get(), *pos.get(),
              *sh1.get(), *sh2.get(),
              mask, *recursion_table.get(),
              *temper_table.get());
        return out;
    }

#define INSTANTIATE_UNIFORM(T)\
    template\
    Array<T> uniformDistribution<T>(const af::dim4 &dims, const af_random_type type, const uintl &seed, uintl &counter);\
    template\
    Array<T> uniformDistribution<T>(const af::dim4 &dims,\
            Array<uint> pos, Array<uint> sh1, Array<uint> sh2, uint mask,\
            Array<uint> recursion_table, Array<uint> temper_table, Array<uint> state);\

#define INSTANTIATE_NORMAL(T)\
    template\
    Array<T> normalDistribution<T>(const af::dim4 &dims, const af_random_type type, const uintl &seed, uintl &counter);\
    template\
    Array<T> normalDistribution<T>(const af::dim4 &dims,\
            Array<uint> pos, Array<uint> sh1, Array<uint> sh2, uint mask,\
            Array<uint> recursion_table, Array<uint> temper_table, Array<uint> state);\

#define COMPLEX_UNIFORM_DISTRIBUTION(T, TR)\
    template<>\
    Array<T> uniformDistribution(const af::dim4 &dims, const af_random_type type, const uintl &seed, uintl &counter)\
    {\
        Array<T> out = createEmptyArray<T>(dims);\
        size_t elements = out.elements()*2;\
        kernel::uniformDistribution<TR>(*out.get(), elements, type, seed, counter);\
        return out;\
    }\
    \
    template<>\
    Array<T> uniformDistribution(const af::dim4 &dims,\
            Array<uint> pos, Array<uint> sh1, Array<uint> sh2, uint mask,\
            Array<uint> recursion_table, Array<uint> temper_table, Array<uint> state)\
    {\
        Array<T> out = createEmptyArray<T>(dims);\
        size_t elements = out.elements()*2;\
        kernel::uniformDistribution<TR>(\
              *out.get(), elements,\
              *state.get(), *pos.get(),\
              *sh1.get(), *sh2.get(),\
              mask, *recursion_table.get(),\
              *temper_table.get());\
        return out;\
    }\

#define COMPLEX_NORMAL_DISTRIBUTION(T, TR)\
    template<>\
    Array<T> normalDistribution(const af::dim4 &dims, const af_random_type type, const uintl &seed, uintl &counter)\
    {\
        Array<T> out = createEmptyArray<T>(dims);\
        size_t elements = out.elements()*2;\
        kernel::normalDistribution<TR>(*out.get(), elements, type, seed, counter);\
        return out;\
    }\
    \
    template<>\
    Array<T> normalDistribution(const af::dim4 &dims,\
            Array<uint> pos, Array<uint> sh1, Array<uint> sh2, uint mask,\
            Array<uint> recursion_table, Array<uint> temper_table, Array<uint> state)\
    {\
        Array<T> out = createEmptyArray<T>(dims);\
        size_t elements = out.elements()*2;\
        kernel::normalDistribution<TR>(\
              *out.get(), elements,\
              *state.get(), *pos.get(),\
              *sh1.get(), *sh2.get(),\
              mask, *recursion_table.get(),\
              *temper_table.get());\
        return out;\
    }\

    INSTANTIATE_UNIFORM(float )
    INSTANTIATE_UNIFORM(double)
    INSTANTIATE_UNIFORM(int   )
    INSTANTIATE_UNIFORM(uint  )
    INSTANTIATE_UNIFORM(intl  )
    INSTANTIATE_UNIFORM(uintl )
    INSTANTIATE_UNIFORM(char  )
    INSTANTIATE_UNIFORM(uchar )
    INSTANTIATE_UNIFORM(short )
    INSTANTIATE_UNIFORM(ushort)

    INSTANTIATE_NORMAL(float )
    INSTANTIATE_NORMAL(double)

    COMPLEX_UNIFORM_DISTRIBUTION(cdouble, double)
    COMPLEX_UNIFORM_DISTRIBUTION(cfloat, float)

    COMPLEX_NORMAL_DISTRIBUTION(cdouble, double)
    COMPLEX_NORMAL_DISTRIBUTION(cfloat, float)

}
