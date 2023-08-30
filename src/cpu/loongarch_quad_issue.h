/**
 * MegPeaK is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2021-2023 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied.
 */

#pragma once
#include "src/cpu/common.h"
#include "stdlib.h"

#if MEGPEAK_LOONGARCH

#define eor(i) "xvxor.v $xr" #i ", $xr" #i ", $xr" #i "\n"

#define LATENCY_MEM(cb, func)                                               \
    static int func##_latency() {                                           \
        const float* a_ptr = reinterpret_cast<float*>(get_mem_align64());   \
        const size_t run_times = megpeak::RUNS;                             \
        asm volatile(                                                       \
        "xvxor.v $xr0, $xr0, $xr0 \n"                                       \
        "1:\n"                                                              \
        UNROLL_CALL(20, cb)                                                 \
        "addi.d %[RUNS],    %[RUNS],    -1 \n"                              \
        "bnez   %[RUNS],    1b \n"                                          \
        :                                                                   \
        : [RUNS]"r"(run_times), [a_ptr]"r"(a_ptr)                           \
        : "memory"                                                          \
        );                                                                  \
        return megpeak::RUNS * 20;                                          \
    }

/**
 * @brief 测试xvldx2和xvfmadd.dx2是否可以四发射 (无数据依赖）
 *
 *      以3A5000为例，其频率2.5GHZ
 *      xvld的throught为0.5 cycle, xvfmadd.d的throught为0.5 cycle.
 *      xvldx2+xvfmadd.dx2的throught为1.13 cycle，基本表明支持四发射
 */
static int xvldx2_xvfmadd_dx2_throught() {
    const float* a_ptr = reinterpret_cast<float*>(get_mem_align64());
    size_t run_times = megpeak::RUNS;
    asm volatile(
        UNROLL_CALL(20, eor)
        "1: \n"
        "xvld   $xr0,   %[a_ptr],   0   \n"
        "xvld   $xr1,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr2,   $xr2,   $xr2,   $xr2    \n"
        "xvfmadd.d  $xr3,   $xr3,   $xr3,   $xr3    \n"

        "xvld   $xr4,   %[a_ptr],   0   \n"
        "xvld   $xr5,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr6,   $xr6,   $xr6,   $xr6    \n"
        "xvfmadd.d  $xr7,   $xr7,   $xr7,   $xr7    \n"

        "xvld   $xr8,   %[a_ptr],   0   \n"
        "xvld   $xr9,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr10,   $xr10,   $xr10,   $xr10    \n"
        "xvfmadd.d  $xr11,   $xr11,   $xr11,   $xr11    \n"

        "xvld   $xr12,   %[a_ptr],   0   \n"
        "xvld   $xr13,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr14,   $xr14,   $xr14,   $xr14    \n"
        "xvfmadd.d  $xr15,   $xr15,   $xr15,   $xr15    \n"

        "xvld   $xr16,   %[a_ptr],   0   \n"
        "xvld   $xr17,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr18,   $xr18,   $xr18,   $xr18    \n"
        "xvfmadd.d  $xr19,   $xr19,   $xr19,   $xr19    \n"

        "addi.d %[RUNS],    %[RUNS],    -1 \n"
        "bnez   %[RUNS],    1b \n"
        :
        : [RUNS]"r"(run_times), [a_ptr]"r"(a_ptr)
        : "memory"
        );
        return megpeak::RUNS * 5;
}
#define cb(i) "xvld   $xr0,   %[a_ptr],   0   \n" \
              "xvld   $xr0,   %[a_ptr],   0   \n" \
              "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n" \
              "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n"
LATENCY_MEM(cb, xvldx2_xvfmadd_dx2)
#undef cb

/**
 * @brief 测试xvldx2和xvfmadd.dx2是否可以四发射 (有数据依赖）
 *
 *      以3A5000为例，其频率2.5GHZ
 *      xvld的throught为0.5 cycle, xvfmadd.d的throught为0.5 cycle.
 */
static int xvldx2_xvfmadd_dx2_dep_throught() {
    const float* a_ptr = reinterpret_cast<float*>(get_mem_align64());
    size_t run_times = megpeak::RUNS;
    asm volatile(
        UNROLL_CALL(20, eor)
        "1: \n"
        "xvld   $xr0,   %[a_ptr],   0   \n"
        "xvld   $xr1,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n"
        "xvfmadd.d  $xr1,   $xr1,   $xr1,   $xr1    \n"
#if 0
        /* 取两组数据，运算，throught为1.10 cycle，表明支持4发射。*/
        "xvld   $xr2,   %[a_ptr],   0   \n"
        "xvld   $xr3,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr2,   $xr2,   $xr2,   $xr2    \n"
        "xvfmadd.d  $xr3,   $xr3,   $xr3,   $xr3    \n"

        "xvld   $xr4,   %[a_ptr],   0   \n"
        "xvld   $xr5,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr4,   $xr4,   $xr4,   $xr4    \n"
        "xvfmadd.d  $xr5,   $xr5,   $xr5,   $xr5    \n"

        "xvld   $xr6,   %[a_ptr],   0   \n"
        "xvld   $xr7,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr6,   $xr6,   $xr6,   $xr6    \n"
        "xvfmadd.d  $xr7,   $xr7,   $xr7,   $xr7    \n"

        "xvld   $xr8,   %[a_ptr],   0   \n"
        "xvld   $xr9,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr8,   $xr8,   $xr8,   $xr8    \n"
        "xvfmadd.d  $xr9,   $xr9,   $xr9,   $xr9    \n"
#endif
#if 1
        /* 取两组数据，运算，throught为1.10 cycle，表明支持4发射。*/
        "xvld   $xr0,   %[a_ptr],   0   \n"
        "xvld   $xr1,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n"
        "xvfmadd.d  $xr1,   $xr1,   $xr1,   $xr1    \n"
        /* 假依赖，可以寄存器重命名解决 */
        "xvld   $xr0,   %[a_ptr],   0   \n"
        "xvld   $xr1,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n"
        "xvfmadd.d  $xr1,   $xr1,   $xr1,   $xr1    \n"

        "xvld   $xr0,   %[a_ptr],   0   \n"
        "xvld   $xr1,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n"
        "xvfmadd.d  $xr1,   $xr1,   $xr1,   $xr1    \n"

        "xvld   $xr0,   %[a_ptr],   0   \n"
        "xvld   $xr1,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n"
        "xvfmadd.d  $xr1,   $xr1,   $xr1,   $xr1    \n"
#endif
        "addi.d %[RUNS],    %[RUNS],    -1 \n"
        "bnez   %[RUNS],    1b \n"
        :
        : [RUNS]"r"(run_times), [a_ptr]"r"(a_ptr)
        : "memory"
        );
        return megpeak::RUNS * 5;
}
#define cb(i) "xvld   $xr0,   %[a_ptr],   0   \n" \
              "xvld   $xr0,   %[a_ptr],   0   \n" \
              "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n" \
              "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n"
LATENCY_MEM(cb, xvldx2_xvfmadd_dx2_dep)
#undef cb

/**
 * @brief 测试xvldrepl.dx2和xvfmadd.dx2是否可以四发射 (无数据依赖）
 *
 *      以3A5000为例，其频率2.5GHZ
 *      xvldrepl.d的throught为0.5 cycle, xvfmadd.d的throught为0.5 cycle.
 *      xvldrepl.dx2+xvfmadd.dx2的throught为1.10 cycle，基本表明支持四发射
 */
static int xvldrepl_dx2_xvfmadd_dx2_throught() {
    const float* a_ptr = reinterpret_cast<float*>(get_mem_align64());
    size_t run_times = megpeak::RUNS;
    asm volatile(
        UNROLL_CALL(20, eor)
        "1: \n"
        "xvldrepl.d   $xr0,   %[a_ptr],   0   \n"
        "xvldrepl.d   $xr1,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr2,   $xr2,   $xr2,   $xr2    \n"
        "xvfmadd.d  $xr3,   $xr3,   $xr3,   $xr3    \n"

        "xvldrepl.d   $xr4,   %[a_ptr],   0   \n"
        "xvldrepl.d   $xr5,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr6,   $xr6,   $xr6,   $xr6    \n"
        "xvfmadd.d  $xr7,   $xr7,   $xr7,   $xr7    \n"

        "xvldrepl.d   $xr8,   %[a_ptr],   0   \n"
        "xvldrepl.d   $xr9,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr10,   $xr10,   $xr10,   $xr10    \n"
        "xvfmadd.d  $xr11,   $xr11,   $xr11,   $xr11    \n"

        "xvldrepl.d   $xr12,   %[a_ptr],   0   \n"
        "xvldrepl.d   $xr13,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr14,   $xr14,   $xr14,   $xr14    \n"
        "xvfmadd.d  $xr15,   $xr15,   $xr15,   $xr15    \n"

        "xvldrepl.d   $xr16,   %[a_ptr],   0   \n"
        "xvldrepl.d   $xr17,   %[a_ptr],   0   \n"
        "xvfmadd.d  $xr18,   $xr18,   $xr18,   $xr18    \n"
        "xvfmadd.d  $xr19,   $xr19,   $xr19,   $xr19    \n"

        "addi.d %[RUNS],    %[RUNS],    -1 \n"
        "bnez   %[RUNS],    1b \n"
        :
        : [RUNS]"r"(run_times), [a_ptr]"r"(a_ptr)
        : "memory"
        );
        return megpeak::RUNS * 5;
}
#define cb(i) "xvldrepl.d   $xr0,   %[a_ptr],   0   \n" \
              "xvldrepl.d   $xr0,   %[a_ptr],   0   \n" \
              "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n" \
              "xvfmadd.d  $xr0,   $xr0,   $xr0,   $xr0    \n"
LATENCY_MEM(cb, xvldrepl_dx2_xvfmadd_dx2)
#undef cb

static void loongarch_quad_issue() {
    using namespace megpeak;
    benchmark(xvldx2_xvfmadd_dx2_throught, xvldx2_xvfmadd_dx2_latency, "xvldx2_xvfmadd_dx2", 4,
              "Test xvld xvld xvfmadd.d xvfmadd.d quad issue");
    benchmark(xvldx2_xvfmadd_dx2_dep_throught, xvldx2_xvfmadd_dx2_dep_latency, "xvldx2_xvfmadd_dx2_dep", 4,
              "Test xvld xvld xvfmadd.d xvfmadd.d quad issue (with data dependency)");
    benchmark(xvldrepl_dx2_xvfmadd_dx2_throught, xvldrepl_dx2_xvfmadd_dx2_latency, "xvldrepl_dx2_xvfmadd_dx2", 4,
              "Test xvldrepl.d xvldrepl.d xvfmadd.d xvfmadd.d quad issue");
}
#endif
