#include "h265_sps_3d_extension_writer.h"

bool WriteSps3dExtension(H265Sps3dExtensionParser::Sps3dExtensionState* sps_3d_extension, BitBufferWriter* bit_buffer) noexcept {

  // H265 sps_3d_extension() NAL Unit.
  // Section I.7.3.2.2.5 ("Sequence parameter set 3d extension syntax")
  // of the H.265 standard for a complete description.

  for (uint32_t d = 0; d <= 1; d++) {
    // iv_di_mc_enabled_flag[d]  u(1)
    if (!bit_buffer->WriteBits(sps_3d_extension->iv_di_mc_enabled_flag[d], 1)) {
      return false;
    }

    // iv_mv_scal_enabled_flag[d]  u(1)
    if (!bit_buffer->WriteBits(sps_3d_extension->iv_mv_scal_enabled_flag[d], 1)) {
      return false;
    }

    if (d == 0) {
      // log2_ivmc_sub_pb_size_minus3[d]  ue(v)
      if (!bit_buffer->WriteExponentialGolomb(
              sps_3d_extension->log2_ivmc_sub_pb_size_minus3)) {
        return false;
      }

      // iv_res_pred_enabled_flag[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->iv_res_pred_enabled_flag, 1)) {
        return false;
      }

      // depth_ref_enabled_flag[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->depth_ref_enabled_flag, 1)) {
        return false;
      }

      // vsp_mc_enabled_flag[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->vsp_mc_enabled_flag, 1)) {
        return false;
      }

      // dbbp_enabled_flag[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->dbbp_enabled_flag, 1)) {
        return false;
      }

    } else {
      // tex_mc_enabled_flag[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->tex_mc_enabled_flag, 1)) {
        return false;
      }

      // log2_texmc_sub_pb_size_minus3[d]  ue(v)
      if (!bit_buffer->WriteExponentialGolomb(
              sps_3d_extension->log2_texmc_sub_pb_size_minus3)) {
        return false;
      }

      // intra_contour_enabled_flag[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->intra_contour_enabled_flag, 1)) {
        return false;
      }

      // intra_dc_only_wedge_enabled_flag[[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->intra_dc_only_wedge_enabled_flag, 1)) {
        return false;
      }

      // cqt_cu_part_pred_enabled_flag[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->cqt_cu_part_pred_enabled_flag, 1)) {
        return false;
      }

      // inter_dc_only_enabled_flag[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->inter_dc_only_enabled_flag, 1)) {
        return false;
      }

      // skip_intra_enabled_flag[d]  u(1)
      if (!bit_buffer->WriteBits(sps_3d_extension->skip_intra_enabled_flag, 1)) {
        return false;
      }
    }
  }

  return true;
}
