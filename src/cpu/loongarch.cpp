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

#include "src/cpu/common.h"
#include "src/cpu/loongarch.h"
#include "src/cpu/loongarch_utils.h"

#if MEGPEAK_LOONGARCH
void megpeak::loongarch() {
#if 0
    benchmark(add_w_throughput, add_w_latency, "add.w", 1);
    benchmark(add_d_throughput, add_d_latency, "add.d", 1);
    benchmark(sub_w_throughput, sub_w_latency, "sub.w", 1);
    benchmark(sub_d_throughput, sub_d_latency, "sub.d", 1);
    benchmark(addi_w_throughput, addi_w_latency, "addi.w", 1);
    benchmark(addi_d_throughput, addi_d_latency, "addi.d", 1);
    benchmark(addu16i_d_throughput, addu16i_d_latency, "addu16i.d", 1);
    benchmark(alsl_w_throughput, alsl_w_latency, "alsl.w", 1);
    benchmark(alsl_d_throughput, alsl_d_latency, "alsl.d", 1);
    benchmark(alsl_wu_throughput, alsl_wu_latency, "alsl.wu", 1);
    benchmark(lu12i_w_throughput, lu12i_w_latency, "lu12i.w", 1);
    benchmark(lu32i_d_throughput, lu32i_d_latency, "lu32i.d", 1);
    benchmark(lu52i_d_throughput, lu52i_d_latency, "lu52i.d", 1);
    benchmark(slt_throughput, slt_latency, "slt", 1);
    benchmark(sltu_throughput, sltu_latency, "sltu", 1);
    benchmark(slti_throughput, slti_latency, "slti", 1);
    benchmark(sltui_throughput, sltui_latency, "sltui", 1);
#endif
}
#else
void megpeak::loongarch() {
}
#endif
