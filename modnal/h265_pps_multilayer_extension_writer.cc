#include "h265_pps_multilayer_extension_writer.h"

bool WritePpsMultilayerExtension(const H265PpsMultilayerExtensionParser::PpsMultilayerExtensionState* pps_multilayer_extension,
    BitBufferWriter* bit_buffer) noexcept {

  // H265 pps_multilayer_extension() NAL Unit.
  // Section F.7.3.2.3.4 ("Picture parameter set multilayer extension syntax")
  // of the H.265 standard for a complete description.

  // poc_reset_info_present_flag  u(1)
  if (!bit_buffer->WriteBits(
          pps_multilayer_extension->poc_reset_info_present_flag, 1)) {
    return false;
  }

  // pps_infer_scaling_list_flag  u(1)
  if (!bit_buffer->WriteBits(
          pps_multilayer_extension->pps_infer_scaling_list_flag, 1)) {
    return false;
  }

  if (pps_multilayer_extension->pps_infer_scaling_list_flag) {
    // pps_scaling_list_ref_layer_id  u(6)
    if (!bit_buffer->WriteBits(
            pps_multilayer_extension->pps_scaling_list_ref_layer_id, 6)) {
      return false;
    }
  }

  // num_ref_loc_offsets  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          pps_multilayer_extension->num_ref_loc_offsets)) {
    return false;
  }

  for (uint32_t i = 0; i < pps_multilayer_extension->num_ref_loc_offsets; i++) {
    // ref_loc_offset_layer_id[i]  u(6)
    if (!bit_buffer->WriteBits(pps_multilayer_extension->ref_loc_offset_layer_id[i], 6)) {
      return false;
    }

    // scaled_ref_layer_offset_present_flag[i]  u(1)
    if (!bit_buffer->WriteBits(pps_multilayer_extension->scaled_ref_layer_offset_present_flag[i], 1)) {
      return false;
    }

    if (pps_multilayer_extension->scaled_ref_layer_offset_present_flag.back()) {
      // TODO(chema): fix this: should be a dictionary
      // scaled_ref_layer_left_offset[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->scaled_ref_layer_left_offset[i])) {
        return false;
      }

      // scaled_ref_layer_top_offset[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->scaled_ref_layer_top_offset[i])) {
        return false;
      }

      // scaled_ref_layer_right_offset[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->scaled_ref_layer_right_offset[i])) {
        return false;
      }

      // scaled_ref_layer_bottom_offset[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->scaled_ref_layer_bottom_offset[i])) {
        return false;
      }
    }

    // ref_region_offset_present_flag[i]  u(1)
    if (!bit_buffer->WriteBits(pps_multilayer_extension->ref_region_offset_present_flag[i], 1)) {
      return false;
    }

    if (pps_multilayer_extension->ref_region_offset_present_flag.back()) {
      // ref_region_left_offset[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->ref_region_left_offset[i])) {
        return false;
      }

      // ref_region_top_offset[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->ref_region_top_offset[i])) {
        return false;
      }

      // ref_region_right_offset[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->ref_region_right_offset[i])) {
        return false;
      }

      // ref_region_bottom_offset[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->ref_region_bottom_offset[i])) {
        return false;
      }
    }

    // resample_phase_set_present_flag[i]  u(1)
    if (!bit_buffer->WriteBits(pps_multilayer_extension->resample_phase_set_present_flag[i], 1)) {
      return false;
    }

    if (pps_multilayer_extension->resample_phase_set_present_flag.back()) {
      // phase_hor_luma[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->phase_hor_luma[i])) {
        return false;
      }

      // phase_ver_luma[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->phase_ver_luma[i])) {
        return false;
      }

      // phase_hor_chroma_plus8[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->phase_hor_chroma_plus8[i])) {
        return false;
      }

      // phase_ver_chroma_plus8[ref_loc_offset_layer_id[i]]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps_multilayer_extension->phase_ver_chroma_plus8[i])) {
        return false;
      }
    }
  }

  // colour_mapping_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(
          pps_multilayer_extension->colour_mapping_enabled_flag, 1)) {
    return false;
  }

  return true;
}
