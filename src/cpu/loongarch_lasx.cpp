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
void megpeak::loongarch_lasx() {
    if (is_supported(SIMDType::LASX)) {
        benchmark(xvld_throughput, xvld_latency, "xvld", 8);
        benchmark(xvst_throughput, xvst_latency, "xvst", 8);
        benchmark(xvldx_throughput, xvldx_latency, "xvldx", 8);
        benchmark(xvstx_throughput, xvstx_latency, "xvstx", 8);
        benchmark(xvldrepl_b_throughput, xvldrepl_b_latency, "xvldrepl.b", 8);
        benchmark(xvldrepl_h_throughput, xvldrepl_h_latency, "xvldrepl.h", 8);
        benchmark(xvldrepl_w_throughput, xvldrepl_w_latency, "xvldrepl.w", 8);
        benchmark(xvldrepl_d_throughput, xvldrepl_d_latency, "xvldrepl.d", 8);
        benchmark(xvstelm_b_throughput, xvstelm_b_latency, "xvstelm.b", 8);
        benchmark(xvstelm_h_throughput, xvstelm_h_latency, "xvstelm.h", 8);
        benchmark(xvstelm_w_throughput, xvstelm_w_latency, "xvstelm.w", 8);
        benchmark(xvstelm_d_throughput, xvstelm_d_latency, "xvstelm.d", 8);
        benchmark(xvfadd_s_throughput, xvfadd_s_latency, "xvfadd.s", 8);
        benchmark(xvfadd_d_throughput, xvfadd_d_latency, "xvfadd.d", 4);
        benchmark(xvfsub_s_throughput, xvfsub_s_latency, "xvfsub.s", 8);
        benchmark(xvfsub_d_throughput, xvfsub_d_latency, "xvfsub.d", 4);
        benchmark(xvfadd_s_throughput, xvfadd_s_latency, "xvfadd.s", 8);
        benchmark(xvfadd_d_throughput, xvfadd_d_latency, "xvfadd.d", 4);
        benchmark(xvfmul_s_throughput, xvfmul_s_latency, "xvfmul.s", 8);
        benchmark(xvfmul_d_throughput, xvfmul_d_latency, "xvfmul.d", 4);
        benchmark(xvfdiv_s_throughput, xvfdiv_s_latency, "xvfdiv.s", 8);
        benchmark(xvfdiv_d_throughput, xvfdiv_d_latency, "xvfdiv.d", 4);
        benchmark(xvfmadd_s_throughput, xvfmadd_s_latency, "xvfmadd.s", 8 * 2);
        benchmark(xvfmadd_d_throughput, xvfmadd_d_latency, "xvfmadd.d", 4 * 2);
        benchmark(xvfmsub_s_throughput, xvfmsub_s_latency, "xvfmsub.s", 8 * 2);
        benchmark(xvfmsub_d_throughput, xvfmsub_d_latency, "xvfmsub.d", 4 * 2);
        benchmark(xvfnmadd_s_throughput, xvfnmadd_s_latency, "xvfnmadd.s", 8 * 3);
        benchmark(xvfnmadd_d_throughput, xvfnmadd_d_latency, "xvfnmadd.d", 4 * 3);
        benchmark(xvfnmsub_s_throughput, xvfnmsub_s_latency, "xvfnmsub.s", 8 * 3);
        benchmark(xvfnmsub_d_throughput, xvfnmsub_d_latency, "xvfnmsub.d", 4 * 3);
        benchmark(xvfmax_s_throughput, xvfmax_s_latency, "xvfmax.s", 8);
        benchmark(xvfmax_d_throughput, xvfmax_d_latency, "xvfmax.d", 4);
        benchmark(xvfmin_s_throughput, xvfmin_s_latency, "xvfmin.s", 8);
        benchmark(xvfmin_d_throughput, xvfmin_d_latency, "xvfmin.d", 4);
        benchmark(xvfmaxa_s_throughput, xvfmaxa_s_latency, "xvfmaxa.s", 8);
        benchmark(xvfmaxa_d_throughput, xvfmaxa_d_latency, "xvfmaxa.d", 4);
        benchmark(xvfmina_s_throughput, xvfmina_s_latency, "xvfmina.s", 8);
        benchmark(xvfmina_d_throughput, xvfmina_d_latency, "xvfmina.d", 4);
        benchmark(xvflogb_s_throughput, xvflogb_s_latency, "xvflogb.s", 8);
        benchmark(xvflogb_d_throughput, xvflogb_d_latency, "xvflogb.d", 4);
        benchmark(xvfclass_s_throughput, xvfclass_s_latency, "xvfclass.s", 8);
        benchmark(xvfclass_d_throughput, xvfclass_d_latency, "xvfclass.d", 4);
        benchmark(xvfsqrt_s_throughput, xvfsqrt_s_latency, "xvfsqrt.s", 8);
        benchmark(xvfsqrt_d_throughput, xvfsqrt_d_latency, "xvfsqrt.d", 4);
        benchmark(xvfrecip_s_throughput, xvfrecip_s_latency, "xvfrecip.s", 8);
        benchmark(xvfrecip_d_throughput, xvfrecip_d_latency, "xvfrecip.d", 4);
        benchmark(xvfrsqrt_s_throughput, xvfrsqrt_s_latency, "xvfrsqrt.s", 8 * 2);
        benchmark(xvfrsqrt_d_throughput, xvfrsqrt_d_latency, "xvfrsqrt.d", 4 * 2);
        // Vector integer arithmetic instructions
        benchmark(xvadd_b_throughput, xvadd_b_latency, "xvadd.b", 32);
        benchmark(xvadd_h_throughput, xvadd_h_latency, "xvadd.h", 16);
        benchmark(xvadd_w_throughput, xvadd_w_latency, "xvadd.w", 8);
        benchmark(xvadd_d_throughput, xvadd_d_latency, "xvadd.d", 4);
        benchmark(xvsub_b_throughput, xvsub_b_latency, "xvsub.b", 32);
        benchmark(xvsub_h_throughput, xvsub_h_latency, "xvsub.h", 16);
        benchmark(xvsub_w_throughput, xvsub_w_latency, "xvsub.w", 8);
        benchmark(xvsub_d_throughput, xvsub_d_latency, "xvsub.d", 4);
        benchmark(xvaddi_bu_throughput, xvaddi_bu_latency, "xvaddi.bu", 32);
        benchmark(xvaddi_hu_throughput, xvaddi_hu_latency, "xvaddi.hu", 16);
        benchmark(xvaddi_wu_throughput, xvaddi_wu_latency, "xvaddi.wu", 8);
        benchmark(xvaddi_du_throughput, xvaddi_du_latency, "xvaddi.du", 4);
        benchmark(xvsubi_bu_throughput, xvsubi_bu_latency, "xvsubi.bu", 32);
        benchmark(xvsubi_hu_throughput, xvsubi_hu_latency, "xvsubi.hu", 16);
        benchmark(xvsubi_wu_throughput, xvsubi_wu_latency, "xvsubi.wu", 8);
        benchmark(xvsubi_du_throughput, xvsubi_du_latency, "xvsubi.du", 4);
        benchmark(xvneg_b_throughput, xvneg_b_latency, "xvneg.b", 32);
        benchmark(xvneg_h_throughput, xvneg_h_latency, "xvneg.h", 16);
        benchmark(xvneg_w_throughput, xvneg_w_latency, "xvneg.w", 8);
        benchmark(xvneg_d_throughput, xvneg_d_latency, "xvneg.d", 4);
        benchmark(xvsadd_b_throughput, xvsadd_b_latency, "xvsadd.b", 32);
        benchmark(xvsadd_h_throughput, xvsadd_h_latency, "xvsadd.h", 16);
        benchmark(xvsadd_w_throughput, xvsadd_w_latency, "xvsadd.w", 8);
        benchmark(xvsadd_d_throughput, xvsadd_d_latency, "xvsadd.d", 4);
        benchmark(xvsadd_bu_throughput, xvsadd_bu_latency, "xvsadd.bu", 32);
        benchmark(xvsadd_hu_throughput, xvsadd_hu_latency, "xvsadd.hu", 16);
        benchmark(xvsadd_wu_throughput, xvsadd_wu_latency, "xvsadd.wu", 8);
        benchmark(xvsadd_du_throughput, xvsadd_du_latency, "xvsadd.du", 4);
        benchmark(xvssub_b_throughput, xvssub_b_latency, "xvssub.b", 32);
        benchmark(xvssub_h_throughput, xvssub_h_latency, "xvssub.h", 16);
        benchmark(xvssub_w_throughput, xvssub_w_latency, "xvssub.w", 8);
        benchmark(xvssub_d_throughput, xvssub_d_latency, "xvssub.d", 4);
        benchmark(xvssub_bu_throughput, xvssub_bu_latency, "xvssub.bu", 32);
        benchmark(xvssub_hu_throughput, xvssub_hu_latency, "xvssub.hu", 16);
        benchmark(xvssub_wu_throughput, xvssub_wu_latency, "xvssub.wu", 8);
        benchmark(xvssub_du_throughput, xvssub_du_latency, "xvssub.du", 4);
        benchmark(xvhaddw_h_b_throughput, xvhaddw_h_b_latency, "xvhaddw.h.b", 32);
        benchmark(xvhaddw_w_h_throughput, xvhaddw_w_h_latency, "xvhaddw.w.h", 16);
        benchmark(xvhaddw_d_w_throughput, xvhaddw_d_w_latency, "xvhaddw.d.w", 8);
        benchmark(xvhaddw_q_d_throughput, xvhaddw_q_d_latency, "xvhaddw.q.d", 4);
        benchmark(xvhaddw_hu_bu_throughput, xvhaddw_hu_bu_latency, "xvhaddw.hu.bu", 32);
        benchmark(xvhaddw_wu_hu_throughput, xvhaddw_wu_hu_latency, "xvhaddw.wu.hu", 16);
        benchmark(xvhaddw_du_wu_throughput, xvhaddw_du_wu_latency, "xvhaddw.du.wu", 8);
        benchmark(xvhaddw_qu_du_throughput, xvhaddw_qu_du_latency, "xvhaddw.qu.du", 4);
        benchmark(xvhsubw_h_b_throughput, xvhsubw_h_b_latency, "xvhsubw.h.b", 32);
        benchmark(xvhsubw_w_h_throughput, xvhsubw_w_h_latency, "xvhsubw.w.h", 16);
        benchmark(xvhsubw_d_w_throughput, xvhsubw_d_w_latency, "xvhsubw.d.w", 8);
        benchmark(xvhsubw_q_d_throughput, xvhsubw_q_d_latency, "xvhsubw.q.d", 4);
        benchmark(xvhsubw_hu_bu_throughput, xvhsubw_hu_bu_latency, "xvhsubw.hu.bu", 32);
        benchmark(xvhsubw_wu_hu_throughput, xvhsubw_wu_hu_latency, "xvhsubw.wu.hu", 16);
        benchmark(xvhsubw_du_wu_throughput, xvhsubw_du_wu_latency, "xvhsubw.du.wu", 8);
        benchmark(xvhsubw_qu_du_throughput, xvhsubw_qu_du_latency, "xvhsubw.qu.du", 4);
    }
}
#else
void megpeak::loongarch_lasx() {}
#endif
