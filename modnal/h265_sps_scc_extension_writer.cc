#include "h265_sps_scc_extension_writer.h"

bool WriteSpsSccExtension(const H265SpsSccExtensionParser::SpsSccExtensionState* sps_scc_extension, uint32_t chroma_format_idc,
                          uint32_t bit_depth_luma_minus8, uint32_t bit_depth_chroma_minus8, BitBufferWriter* bit_buffer) noexcept
{

  // H265 sps_scc_extension() NAL Unit.
  // Section 7.3.2.2.3 ("Sequence parameter set screen content coding extension
  // syntax") of the H.265 standard for a complete description.

// sps_curr_pic_ref_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(sps_scc_extension->sps_curr_pic_ref_enabled_flag, 1)) {
    return false;
  }

  // palette_mode_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(sps_scc_extension->palette_mode_enabled_flag, 1)) {
    return false;
  }

  if (sps_scc_extension->palette_mode_enabled_flag) {
    // palette_max_size  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(
            sps_scc_extension->palette_max_size)) {
      return false;
    }

    // delta_palette_max_predictor_size  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(
            sps_scc_extension->delta_palette_max_predictor_size)) {
      return false;
    }

    // sps_palette_predictor_initializers_present_flag  u(1)
    if (!bit_buffer->WriteBits(
            sps_scc_extension
                   ->sps_palette_predictor_initializers_present_flag, 1)) {
      return false;
    }

    if (sps_scc_extension->sps_palette_predictor_initializers_present_flag) {
      // sps_num_palette_predictor_initializers_minus1  ue(v)
      if (!bit_buffer->WriteExponentialGolomb(
              sps_scc_extension
                  ->sps_num_palette_predictor_initializers_minus1)) {
        return false;
      }

      uint32_t numComps = (chroma_format_idc == 0) ? 1 : 3;
      uint32_t BitDepth_Y = 8 + bit_depth_luma_minus8;    // Eq 7-4
      uint32_t BitDepth_C = 8 + bit_depth_chroma_minus8;  // Eq 7-6

      for (uint32_t comp = 0; comp < numComps; comp++) {
        for (uint32_t i = 0;
             i <=
             sps_scc_extension->sps_num_palette_predictor_initializers_minus1;
             i++) {
          // sps_palette_predictor_initializers[i][j]  u(v)
          // sps_palette_predictor_initializers[0][*] -> BitDepth_Y
          // sps_palette_predictor_initializers[1,2][*] -> BitDepth_C
          uint32_t bit_depth = (comp == 0) ? BitDepth_Y : BitDepth_C;
          if (!bit_buffer->WriteBits(sps_scc_extension->sps_palette_predictor_initializers[comp][i], bit_depth)) {
            return false;
          }
        }
      }
    }
  }

  // motion_vector_resolution_control_idc  u(2)
  if (!bit_buffer->WriteBits(
          sps_scc_extension->motion_vector_resolution_control_idc, 2)) {
    return false;
  }

  // intra_boundary_filtering_disabled_flag  u(1)
  if (!bit_buffer->WriteBits(
          sps_scc_extension->intra_boundary_filtering_disabled_flag, 1)) {
    return false;
  }

  return true;
}
