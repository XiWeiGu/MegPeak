/**
 * MegPeaK is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2021-2023 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.
 */

#pragma once
#include "src/cpu/common.h"
#include "stdlib.h"

#if MEGPEAK_LOONGARCH

#define eor(i) "xvxor.v $xr" #i ", $xr" #i ", $xr" #i "\n"
// clang-format off
#define THROUGHPUT(cb, func)                                  \
    static int func##_throughput() {                          \
        asm volatile(                                         \
        UNROLL_CALL(10, eor)                                  \
        "1:\n"                                                \
        UNROLL_CALL(10, cb)                                   \
        "addi.d  %[RUNS],   %[RUNS],    -1\n"                 \
        "bnez    %[RUNS],   1b\n"                             \
        :                                                     \
        :[RUNS]"r"(megpeak::RUNS * 100)                       \
        :                                                     \
        );                                                    \
        return megpeak::RUNS * 100 * 10;                      \
    }

#define LATENCY(cb, func)                                  \
    static int func##_latency() {                          \
        asm volatile(                                      \
        "xvxor.v $xr0, $xr0, $xr0 \n"                      \
        "1:\n"                                             \
        UNROLL_CALL(10, cb)                                \
        "addi.d  %[RUNS],   %[RUNS],    -1\n"              \
        "bnez    %[RUNS],   1b\n"                          \
        :                                                  \
        :[RUNS]"r"(megpeak::RUNS * 100)                    \
        :                                                  \
        );                                                 \
        return megpeak::RUNS * 100 * 10;                   \
    }

#define THROUGHPUT_MEM(cb, func)                                            \
    static int func##_throughput() {                                        \
        const float* a_ptr = reinterpret_cast<float*>(get_mem_align64());   \
        const size_t run_times = megpeak::RUNS;                             \
        asm volatile(                                                       \
        UNROLL_CALL(20, eor)                                                \
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

// Vector memory access instructions
#define cb(i) "xvld     $xr" #i ",  %[a_ptr],   0 \n"
THROUGHPUT_MEM(cb, xvld)
#undef cb
#define cb(i) "xvld     $xr0,   %[a_ptr],   0 \n"
LATENCY_MEM(cb, xvld)
#undef cb

#define cb(i) "xvst     $xr" #i ",  %[a_ptr],   0 \n"
THROUGHPUT_MEM(cb, xvst)
#undef cb
#define cb(i) "xvst     $xr0,       %[a_ptr],   0 \n"
LATENCY_MEM(cb, xvst)
#undef cb

#define cb(i) "xvldx     $xr" #i ",  %[a_ptr],   $zero \n"
THROUGHPUT_MEM(cb, xvldx)
#undef cb
#define cb(i) "xvldx     $xr0,   %[a_ptr],   $zero \n"
LATENCY_MEM(cb, xvldx)
#undef cb

#define cb(i) "xvstx     $xr" #i ",  %[a_ptr],   $zero \n"
THROUGHPUT_MEM(cb, xvstx)
#undef cb
#define cb(i) "xvstx     $xr0,       %[a_ptr],   $zero \n"
LATENCY_MEM(cb, xvstx)
#undef cb

#define cb(i) "xvldrepl.b     $xr" #i ",  %[a_ptr],   0 \n"
THROUGHPUT_MEM(cb, xvldrepl_b)
#undef cb
#define cb(i) "xvldrepl.b     $xr0,   %[a_ptr],   0 \n"
LATENCY_MEM(cb, xvldrepl_b)
#undef cb

#define cb(i) "xvldrepl.h     $xr" #i ",  %[a_ptr],   0 \n"
THROUGHPUT_MEM(cb, xvldrepl_h)
#undef cb
#define cb(i) "xvldrepl.h     $xr0,   %[a_ptr],   0 \n"
LATENCY_MEM(cb, xvldrepl_h)
#undef cb

#define cb(i) "xvldrepl.w     $xr" #i ",  %[a_ptr],   0 \n"
THROUGHPUT_MEM(cb, xvldrepl_w)
#undef cb
#define cb(i) "xvldrepl.w     $xr0,   %[a_ptr],   0 \n"
LATENCY_MEM(cb, xvldrepl_w)
#undef cb

#define cb(i) "xvldrepl.d     $xr" #i ",  %[a_ptr],   0 \n"
THROUGHPUT_MEM(cb, xvldrepl_d)
#undef cb
#define cb(i) "xvldrepl.d     $xr0,   %[a_ptr],   0 \n"
LATENCY_MEM(cb, xvldrepl_d)
#undef cb

#define cb(i) "xvstelm.b    $xr" #i ",  %[a_ptr],   0,  0 \n"
THROUGHPUT_MEM(cb, xvstelm_b)
#undef cb
#define cb(i) "xvstelm.b    $xr0,  %[a_ptr],   0,  0 \n"
LATENCY_MEM(cb, xvstelm_b)
#undef cb

#define cb(i) "xvstelm.h    $xr" #i ",  %[a_ptr],   0,  0 \n"
THROUGHPUT_MEM(cb, xvstelm_h)
#undef cb
#define cb(i) "xvstelm.h    $xr0,  %[a_ptr],   0,  0 \n"
LATENCY_MEM(cb, xvstelm_h)
#undef cb

#define cb(i) "xvstelm.w    $xr" #i ",  %[a_ptr],   0,  0 \n"
THROUGHPUT_MEM(cb, xvstelm_w)
#undef cb
#define cb(i) "xvstelm.w    $xr0,  %[a_ptr],   0,  0 \n"
LATENCY_MEM(cb, xvstelm_w)
#undef cb

#define cb(i) "xvstelm.d    $xr" #i ",  %[a_ptr],   0,  0 \n"
THROUGHPUT_MEM(cb, xvstelm_d)
#undef cb
#define cb(i) "xvstelm.d    $xr0,  %[a_ptr],   0,  0 \n"
LATENCY_MEM(cb, xvstelm_d)
#undef cb

// Vector integer arithmetic instructions
#define cb(i) "xvadd.b    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvadd_b)
#undef cb
#define cb(i) "xvadd.b    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvadd_b)
#undef cb

#define cb(i) "xvadd.h    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvadd_h)
#undef cb
#define cb(i) "xvadd.h    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvadd_h)
#undef cb

#define cb(i) "xvadd.w    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvadd_w)
#undef cb
#define cb(i) "xvadd.w    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvadd_w)
#undef cb

#define cb(i) "xvadd.d    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvadd_d)
#undef cb
#define cb(i) "xvadd.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvadd_d)
#undef cb

#define cb(i) "xvsub.b    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsub_b)
#undef cb
#define cb(i) "xvsub.b    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsub_b)
#undef cb

#define cb(i) "xvsub.h    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsub_h)
#undef cb
#define cb(i) "xvsub.h    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsub_h)
#undef cb

#define cb(i) "xvsub.w    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsub_w)
#undef cb
#define cb(i) "xvsub.w    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsub_w)
#undef cb

#define cb(i) "xvsub.d    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsub_d)
#undef cb
#define cb(i) "xvsub.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsub_d)
#undef cb

#define cb(i) "xvaddi.bu    $xr" #i",    $xr" #i ",  0 \n"
THROUGHPUT(cb, xvaddi_bu)
#undef cb
#define cb(i) "xvaddi.bu    $xr0,   $xr0,   0 \n"
LATENCY(cb, xvaddi_bu)
#undef cb

#define cb(i) "xvaddi.hu    $xr" #i",    $xr" #i ",  0 \n"
THROUGHPUT(cb, xvaddi_hu)
#undef cb
#define cb(i) "xvaddi.hu    $xr0,   $xr0,   0 \n"
LATENCY(cb, xvaddi_hu)
#undef cb

#define cb(i) "xvaddi.wu    $xr" #i",    $xr" #i ",  0 \n"
THROUGHPUT(cb, xvaddi_wu)
#undef cb
#define cb(i) "xvaddi.wu    $xr0,   $xr0,  0 \n"
LATENCY(cb, xvaddi_wu)
#undef cb

#define cb(i) "xvaddi.du    $xr" #i",    $xr" #i ",  0 \n"
THROUGHPUT(cb, xvaddi_du)
#undef cb
#define cb(i) "xvaddi.du    $xr0,   $xr0,   0 \n"
LATENCY(cb, xvaddi_du)
#undef cb

#define cb(i) "xvsubi.bu    $xr" #i",    $xr" #i ",  0 \n"
THROUGHPUT(cb, xvsubi_bu)
#undef cb
#define cb(i) "xvsubi.bu    $xr0,   $xr0,   0 \n"
LATENCY(cb, xvsubi_bu)
#undef cb

#define cb(i) "xvsubi.hu    $xr" #i",    $xr" #i ",  0 \n"
THROUGHPUT(cb, xvsubi_hu)
#undef cb
#define cb(i) "xvsubi.hu    $xr0,   $xr0,   0 \n"
LATENCY(cb, xvsubi_hu)
#undef cb

#define cb(i) "xvsubi.wu    $xr" #i",    $xr" #i ",  0 \n"
THROUGHPUT(cb, xvsubi_wu)
#undef cb
#define cb(i) "xvsubi.wu    $xr0,   $xr0,  0 \n"
LATENCY(cb, xvsubi_wu)
#undef cb

#define cb(i) "xvsubi.du    $xr" #i",    $xr" #i ",  0 \n"
THROUGHPUT(cb, xvsubi_du)
#undef cb
#define cb(i) "xvsubi.du    $xr0,   $xr0,   0 \n"
LATENCY(cb, xvsubi_du)
#undef cb

#define cb(i) "xvneg.b      $xr" #i ",      $xr" #i " \n"
THROUGHPUT(cb, xvneg_b)
#undef cb
#define cb(i) "xvneg.b      $xr0,   $xr0 \n"
LATENCY(cb, xvneg_b)
#undef cb

#define cb(i) "xvneg.h      $xr" #i ",      $xr" #i " \n"
THROUGHPUT(cb, xvneg_h)
#undef cb
#define cb(i) "xvneg.h      $xr0,   $xr0 \n"
LATENCY(cb, xvneg_h)
#undef cb

#define cb(i) "xvneg.w      $xr" #i ",      $xr" #i " \n"
THROUGHPUT(cb, xvneg_w)
#undef cb
#define cb(i) "xvneg.w      $xr0,   $xr0 \n"
LATENCY(cb, xvneg_w)
#undef cb

#define cb(i) "xvneg.d      $xr" #i ",      $xr" #i " \n"
THROUGHPUT(cb, xvneg_d)
#undef cb
#define cb(i) "xvneg.d      $xr0,   $xr0 \n"
LATENCY(cb, xvneg_d)
#undef cb

#define cb(i) "xvsadd.b    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsadd_b)
#undef cb
#define cb(i) "xvsadd.b    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsadd_b)
#undef cb

#define cb(i) "xvsadd.h    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsadd_h)
#undef cb
#define cb(i) "xvsadd.h    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsadd_h)
#undef cb

#define cb(i) "xvsadd.w    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsadd_w)
#undef cb
#define cb(i) "xvsadd.w    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsadd_w)
#undef cb

#define cb(i) "xvsadd.d    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsadd_d)
#undef cb
#define cb(i) "xvsadd.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsadd_d)
#undef cb

#define cb(i) "xvsadd.bu    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsadd_bu)
#undef cb
#define cb(i) "xvsadd.bu    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsadd_bu)
#undef cb

#define cb(i) "xvsadd.hu    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsadd_hu)
#undef cb
#define cb(i) "xvsadd.hu    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsadd_hu)
#undef cb

#define cb(i) "xvsadd.wu    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsadd_wu)
#undef cb
#define cb(i) "xvsadd.wu    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsadd_wu)
#undef cb

#define cb(i) "xvsadd.du    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvsadd_du)
#undef cb
#define cb(i) "xvsadd.du    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvsadd_du)
#undef cb

#define cb(i) "xvssub.b    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvssub_b)
#undef cb
#define cb(i) "xvssub.b    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvssub_b)
#undef cb

#define cb(i) "xvssub.h    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvssub_h)
#undef cb
#define cb(i) "xvssub.h    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvssub_h)
#undef cb

#define cb(i) "xvssub.w    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvssub_w)
#undef cb
#define cb(i) "xvssub.w    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvssub_w)
#undef cb

#define cb(i) "xvssub.d    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvssub_d)
#undef cb
#define cb(i) "xvssub.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvssub_d)
#undef cb

#define cb(i) "xvssub.bu    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvssub_bu)
#undef cb
#define cb(i) "xvssub.bu    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvssub_bu)
#undef cb

#define cb(i) "xvssub.hu    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvssub_hu)
#undef cb
#define cb(i) "xvssub.hu    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvssub_hu)
#undef cb

#define cb(i) "xvssub.wu    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvssub_wu)
#undef cb
#define cb(i) "xvssub.wu    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvssub_wu)
#undef cb

#define cb(i) "xvssub.du    $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvssub_du)
#undef cb
#define cb(i) "xvssub.du    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvssub_du)
#undef cb

#define cb(i) "xvhaddw.h.b  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhaddw_h_b)
#undef cb
#define cb(i) "xvhaddw.h.b  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhaddw_h_b)
#undef cb

#define cb(i) "xvhaddw.w.h  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhaddw_w_h)
#undef cb
#define cb(i) "xvhaddw.w.h  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhaddw_w_h)
#undef cb

#define cb(i) "xvhaddw.d.w  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhaddw_d_w)
#undef cb
#define cb(i) "xvhaddw.d.w  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhaddw_d_w)
#undef cb

#define cb(i) "xvhaddw.q.d  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhaddw_q_d)
#undef cb
#define cb(i) "xvhaddw.q.d  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhaddw_q_d)
#undef cb

#define cb(i) "xvhaddw.hu.bu  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhaddw_hu_bu)
#undef cb
#define cb(i) "xvhaddw.hu.bu  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhaddw_hu_bu)
#undef cb

#define cb(i) "xvhaddw.wu.hu  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhaddw_wu_hu)
#undef cb
#define cb(i) "xvhaddw.wu.hu  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhaddw_wu_hu)
#undef cb

#define cb(i) "xvhaddw.du.wu  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhaddw_du_wu)
#undef cb
#define cb(i) "xvhaddw.du.wu  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhaddw_du_wu)
#undef cb

#define cb(i) "xvhaddw.qu.du  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhaddw_qu_du)
#undef cb
#define cb(i) "xvhaddw.qu.du  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhaddw_qu_du)
#undef cb

#define cb(i) "xvhsubw.h.b  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhsubw_h_b)
#undef cb
#define cb(i) "xvhsubw.h.b  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhsubw_h_b)
#undef cb

#define cb(i) "xvhsubw.w.h  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhsubw_w_h)
#undef cb
#define cb(i) "xvhsubw.w.h  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhsubw_w_h)
#undef cb

#define cb(i) "xvhsubw.d.w  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhsubw_d_w)
#undef cb
#define cb(i) "xvhsubw.d.w  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhsubw_d_w)
#undef cb

#define cb(i) "xvhsubw.q.d  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhsubw_q_d)
#undef cb
#define cb(i) "xvhsubw.q.d  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhsubw_q_d)
#undef cb

#define cb(i) "xvhsubw.hu.bu  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhsubw_hu_bu)
#undef cb
#define cb(i) "xvhsubw.hu.bu  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhsubw_hu_bu)
#undef cb

#define cb(i) "xvhsubw.wu.hu  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhsubw_wu_hu)
#undef cb
#define cb(i) "xvhsubw.wu.hu  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhsubw_wu_hu)
#undef cb

#define cb(i) "xvhsubw.du.wu  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhsubw_du_wu)
#undef cb
#define cb(i) "xvhsubw.du.wu  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhsubw_du_wu)
#undef cb

#define cb(i) "xvhsubw.qu.du  $xr" #i",    $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvhsubw_qu_du)
#undef cb
#define cb(i) "xvhsubw.qu.du  $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvhsubw_qu_du)
#undef cb

// Vector floating-point arithmetic instructions
#define cb(i) "xvfadd.s    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfadd_s)
#undef cb
#define cb(i) "xvfadd.s    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfadd_s)
#undef cb

#define cb(i) "xvfadd.d    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfadd_d)
#undef cb
#define cb(i) "xvfadd.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfadd_d)
#undef cb

#define cb(i) "xvfsub.s    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfsub_s)
#undef cb
#define cb(i) "xvfsub.s    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfsub_s)
#undef cb

#define cb(i) "xvfsub.d    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfsub_d)
#undef cb
#define cb(i) "xvfsub.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfsub_d)
#undef cb

#define cb(i) "xvfmul.s    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmul_s)
#undef cb
#define cb(i) "xvfmul.s    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmul_s)
#undef cb

#define cb(i) "xvfmul.d    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmul_d)
#undef cb
#define cb(i) "xvfmul.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmul_d)
#undef cb

#define cb(i) "xvfdiv.s    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfdiv_s)
#undef cb
#define cb(i) "xvfdiv.s    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfdiv_s)
#undef cb

#define cb(i) "xvfdiv.d    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfdiv_d)
#undef cb
#define cb(i) "xvfdiv.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfdiv_d)
#undef cb

#define cb(i) "xvfmadd.s    $xr" #i ",   $xr" #i ",  $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmadd_s)
#undef cb
#define cb(i) "xvfmadd.s    $xr0,   $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmadd_s)
#undef cb

#define cb(i) "xvfmadd.d    $xr" #i ",   $xr" #i ",  $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmadd_d)
#undef cb
#define cb(i) "xvfmadd.d    $xr0,   $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmadd_d)
#undef cb

#define cb(i) "xvfmsub.s    $xr" #i ",   $xr" #i ",  $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmsub_s)
#undef cb
#define cb(i) "xvfmsub.s    $xr0,   $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmsub_s)
#undef cb

#define cb(i) "xvfmsub.d    $xr" #i ",   $xr" #i ",  $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmsub_d)
#undef cb
#define cb(i) "xvfmsub.d    $xr0,   $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmsub_d)
#undef cb

#define cb(i) "xvfnmadd.s    $xr" #i ",   $xr" #i ",  $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfnmadd_s)
#undef cb
#define cb(i) "xvfnmadd.s    $xr0,   $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfnmadd_s)
#undef cb

#define cb(i) "xvfnmadd.d    $xr" #i ",   $xr" #i ",  $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfnmadd_d)
#undef cb
#define cb(i) "xvfnmadd.d    $xr0,   $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfnmadd_d)
#undef cb

#define cb(i) "xvfnmsub.s    $xr" #i ",   $xr" #i ",  $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfnmsub_s)
#undef cb
#define cb(i) "xvfnmsub.s    $xr0,   $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfnmsub_s)
#undef cb

#define cb(i) "xvfnmsub.d    $xr" #i ",   $xr" #i ",  $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfnmsub_d)
#undef cb
#define cb(i) "xvfnmsub.d    $xr0,   $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfnmsub_d)
#undef cb

#define cb(i) "xvfmax.s    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmax_s)
#undef cb
#define cb(i) "xvfmax.s    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmax_s)
#undef cb

#define cb(i) "xvfmax.d    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmax_d)
#undef cb
#define cb(i) "xvfmax.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmax_d)
#undef cb

#define cb(i) "xvfmin.s    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmin_s)
#undef cb
#define cb(i) "xvfmin.s    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmin_s)
#undef cb

#define cb(i) "xvfmin.d    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmin_d)
#undef cb
#define cb(i) "xvfmin.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmin_d)
#undef cb

#define cb(i) "xvfmaxa.s    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmaxa_s)
#undef cb
#define cb(i) "xvfmaxa.s    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmaxa_s)
#undef cb

#define cb(i) "xvfmaxa.d    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmaxa_d)
#undef cb
#define cb(i) "xvfmaxa.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmaxa_d)
#undef cb

#define cb(i) "xvfmina.s    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmina_s)
#undef cb
#define cb(i) "xvfmina.s    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmina_s)
#undef cb

#define cb(i) "xvfmina.d    $xr" #i ",   $xr" #i ",  $xr" #i " \n"
THROUGHPUT(cb, xvfmina_d)
#undef cb
#define cb(i) "xvfmina.d    $xr0,   $xr0,   $xr0 \n"
LATENCY(cb, xvfmina_d)
#undef cb

#define cb(i) "xvflogb.s    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvflogb_s)
#undef cb
#define cb(i) "xvflogb.s    $xr0,   $xr0 \n"
LATENCY(cb, xvflogb_s)
#undef cb

#define cb(i) "xvflogb.d    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvflogb_d)
#undef cb
#define cb(i) "xvflogb.d    $xr0,   $xr0 \n"
LATENCY(cb, xvflogb_d)
#undef cb

#define cb(i) "xvfclass.s    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvfclass_s)
#undef cb
#define cb(i) "xvfclass.s    $xr0,   $xr0 \n"
LATENCY(cb, xvfclass_s)
#undef cb

#define cb(i) "xvfclass.d    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvfclass_d)
#undef cb
#define cb(i) "xvfclass.d    $xr0,   $xr0 \n"
LATENCY(cb, xvfclass_d)
#undef cb

#define cb(i) "xvfsqrt.s    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvfsqrt_s)
#undef cb
#define cb(i) "xvfsqrt.s    $xr0,   $xr0 \n"
LATENCY(cb, xvfsqrt_s)
#undef cb

#define cb(i) "xvfsqrt.d    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvfsqrt_d)
#undef cb
#define cb(i) "xvfsqrt.d    $xr0,   $xr0 \n"
LATENCY(cb, xvfsqrt_d)
#undef cb

#define cb(i) "xvfrecip.s    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvfrecip_s)
#undef cb
#define cb(i) "xvfrecip.s    $xr0,   $xr0 \n"
LATENCY(cb, xvfrecip_s)
#undef cb

#define cb(i) "xvfrecip.d    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvfrecip_d)
#undef cb
#define cb(i) "xvfrecip.d    $xr0,   $xr0 \n"
LATENCY(cb, xvfrecip_d)
#undef cb

#define cb(i) "xvfrsqrt.s    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvfrsqrt_s)
#undef cb
#define cb(i) "xvfrsqrt.s    $xr0,   $xr0 \n"
LATENCY(cb, xvfrsqrt_s)
#undef cb

#define cb(i) "xvfrsqrt.d    $xr" #i ",   $xr" #i " \n"
THROUGHPUT(cb, xvfrsqrt_d)
#undef cb
#define cb(i) "xvfrsqrt.d    $xr0,   $xr0 \n"
LATENCY(cb, xvfrsqrt_d)
#undef cb

#endif /* MEGPEAK_LOONGARCH */
