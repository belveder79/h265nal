#include "h265_pps_scc_extension_writer.h"


bool WritePpsSccExtension(H265PpsSccExtensionParser::PpsSccExtensionState* pps_scc_extension,
    BitBufferWriter* bit_buffer) noexcept {

  // H265 pps_scc_extension() NAL Unit.
  // Section 7.3.2.3.2 ("Picture parameter set screen extension syntax") of
  // the H.265 standard for a complete description.

  // pps_curr_pic_ref_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(pps_scc_extension->pps_curr_pic_ref_enabled_flag, 1)) {
    return false;
  }

  // residual_adaptive_colour_transform_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(pps_scc_extension->residual_adaptive_colour_transform_enabled_flag, 1)) {
    return false;
  }

  if (pps_scc_extension->residual_adaptive_colour_transform_enabled_flag) {
    // pps_slice_act_qp_offsets_present_flag  u(1)
    if (!bit_buffer->WriteBits(pps_scc_extension->pps_slice_act_qp_offsets_present_flag, 1)) {
      return false;
    }

    // pps_act_y_qp_offset_plus5  se(v)
    if (!bit_buffer->WriteSignedExponentialGolomb(
            pps_scc_extension->pps_act_y_qp_offset_plus5)) {
      return false;
    }

    // pps_act_cb_qp_offset_plus5  se(v)
    if (!bit_buffer->WriteSignedExponentialGolomb(
            pps_scc_extension->pps_act_cb_qp_offset_plus5)) {
      return false;
    }

    // pps_act_cr_qp_offset_plus3  se(v)
    if (!bit_buffer->WriteSignedExponentialGolomb(
            pps_scc_extension->pps_act_cr_qp_offset_plus3)) {
      return false;
    }
  }

  // pps_palette_predictor_initializer_present_flag  u(1)
  if (!bit_buffer->WriteBits(pps_scc_extension->pps_palette_predictor_initializer_present_flag, 1)) {
    return false;
  }

  if (pps_scc_extension->pps_palette_predictor_initializer_present_flag) {
    // pps_num_palette_predictor_initializer  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(
            pps_scc_extension->pps_num_palette_predictor_initializer)) {
      return false;
    }

    if (pps_scc_extension->pps_num_palette_predictor_initializer > 0) {
      // monochrome_palette_flag  u(1)
      if (!bit_buffer->WriteBits(pps_scc_extension->monochrome_palette_flag, 1)) {
        return false;
      }

      // luma_bit_depth_entry_minus8  ue(v)
      if (!bit_buffer->WriteExponentialGolomb(
              pps_scc_extension->luma_bit_depth_entry_minus8)) {
        return false;
      }

      if (pps_scc_extension->monochrome_palette_flag) {
        // chroma_bit_depth_entry_minus8  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(
                pps_scc_extension->chroma_bit_depth_entry_minus8)) {
          return false;
        }
      }

      uint32_t numComps = pps_scc_extension->monochrome_palette_flag ? 1 : 3;
      for (uint32_t comp = 0; comp < numComps; comp++) {
        for (uint32_t i = 0;
             i < pps_scc_extension->pps_num_palette_predictor_initializer;
             i++) {
          // pps_palette_predictor_initializers[comp][i]  u(v)
          // pps_palette_predictor_initializers[0][*] ->
          // luma_bit_depth_entry_minus8 + 8,
          // pps_palette_predictor_initializers[1,2][*] ->
          // chroma_bit_depth_entry_minus8 + 8

          uint32_t bit_depth =
              (comp == 0)
                  ? (pps_scc_extension->luma_bit_depth_entry_minus8 + 8)
                  : (pps_scc_extension->chroma_bit_depth_entry_minus8 + 8);
          if (!bit_buffer->WriteBits(pps_scc_extension->pps_palette_predictor_initializers[comp][i], bit_depth)) {
            return false;
          }
        }
      }
    }
  }

  return true;
}

