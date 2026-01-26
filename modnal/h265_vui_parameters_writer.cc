#include "h265_vui_parameters_writer.h"
#include "h265_hrd_parameters_writer.h"

bool WriteVuiParameters(H265VuiParametersParser::VuiParametersState* vui,
    BitBufferWriter* bit_buffer) noexcept {
    
  // H265 vui_parameters() parser.
  // Section E.2.1 ("VUI parameters syntax") of the H.265 standard for
  // a complete description.

  // aspect_ratio_info_present_flag  u(1)
  if (!bit_buffer->WriteBits(vui->aspect_ratio_info_present_flag, 1)) {
    return false;
  }

  if (vui->aspect_ratio_info_present_flag) {
    // aspect_ratio_idc  u(8)
    if (!bit_buffer->WriteBits(vui->aspect_ratio_idc, 8)) {
      return false;
    }
    if (vui->aspect_ratio_idc == AR_EXTENDED_SAR) {
      // sar_width  u(16)
      if (!bit_buffer->WriteBits(vui->sar_width, 16)) {
        return false;
      }
      // sar_height  u(16)
      if (!bit_buffer->WriteBits(vui->sar_height, 16)) {
        return false;
      }
    }
  }

  // overscan_info_present_flag  u(1)
  if (!bit_buffer->WriteBits(vui->overscan_info_present_flag, 1)) {
    return false;
  }

  if (vui->overscan_info_present_flag) {
    // overscan_appropriate_flag  u(1)
    if (!bit_buffer->WriteBits(vui->overscan_appropriate_flag, 1)) {
      return false;
    }
  }

  // video_signal_type_present_flag  u(1)
  if (!bit_buffer->WriteBits(vui->video_signal_type_present_flag, 1)) {
    return false;
  }

  if (vui->video_signal_type_present_flag) {
    // video_format  u(3)
    if (!bit_buffer->WriteBits(vui->video_format, 3)) {
      return false;
    }
    // video_full_range_flag  u(1)
    if (!bit_buffer->WriteBits(vui->video_full_range_flag, 1)) {
      return false;
    }
    // colour_description_present_flag  u(1)
    if (!bit_buffer->WriteBits(vui->colour_description_present_flag, 1)) {
      return false;
    }
    if (vui->colour_description_present_flag) {
      // colour_primaries  u(8)
      if (!bit_buffer->WriteBits(vui->colour_primaries, 8)) {
        return false;
      }
      // transfer_characteristics  u(8)
      if (!bit_buffer->WriteBits(vui->transfer_characteristics, 8)) {
        return false;
      }
      // matrix_coeffs  u(8)
      if (!bit_buffer->WriteBits(vui->matrix_coeffs, 8)) {
        return false;
      }
    }
  }

  // chroma_loc_info_present_flag  u(1)
  if (!bit_buffer->WriteBits(vui->chroma_loc_info_present_flag, 1)) {
    return false;
  }
  if (vui->chroma_loc_info_present_flag) {
    // chroma_sample_loc_type_top_field  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(
            vui->chroma_sample_loc_type_top_field)) {
      return false;
    }

    // chroma_sample_loc_type_bottom_field  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(
            vui->chroma_sample_loc_type_bottom_field)) {
      return false;
    }
  }

  // neutral_chroma_indication_flag  u(1)
  if (!bit_buffer->WriteBits(vui->neutral_chroma_indication_flag, 1)) {
    return false;
  }

  // field_seq_flag  u(1)
  if (!bit_buffer->WriteBits(vui->field_seq_flag, 1)) {
    return false;
  }

  // frame_field_info_present_flag  u(1)
  if (!bit_buffer->WriteBits(vui->frame_field_info_present_flag, 1)) {
    return false;
  }

  // default_display_window_flag  u(1)
  if (!bit_buffer->WriteBits(vui->default_display_window_flag, 1)) {
    return false;
  }
  if (vui->default_display_window_flag) {
    // def_disp_win_left_offset ue(v)
    if (!bit_buffer->WriteExponentialGolomb(vui->def_disp_win_left_offset)) {
      return false;
    }

    // def_disp_win_right_offset  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(vui->def_disp_win_right_offset)) {
      return false;
    }

    // def_disp_win_top_offset  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(vui->def_disp_win_top_offset)) {
      return false;
    }

    // def_disp_win_bottom_offset  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(vui->def_disp_win_bottom_offset)) {
      return false;
    }
  }

  // vui_timing_info_present_flag  u(1)
  if (!bit_buffer->WriteBits(vui->vui_timing_info_present_flag, 1)) {
    return false;
  }
  if (vui->vui_timing_info_present_flag) {
    // vui_num_units_in_tick  u(32)
    if (!bit_buffer->WriteBits(vui->vui_num_units_in_tick, 32)) {
      return false;
    }
    // vui_time_scale  u(32)
    if (!bit_buffer->WriteBits(vui->vui_time_scale, 32)) {
      return false;
    }
    // vui_poc_proportional_to_timing_flag  u(1)
    if (!bit_buffer->WriteBits(vui->vui_poc_proportional_to_timing_flag, 1)) {
      return false;
    }

    // vui_hrd_parameters_present_flag  u(1)
    if (!bit_buffer->WriteBits(vui->vui_hrd_parameters_present_flag, 1)) {
      return false;
    }
    if (vui->vui_hrd_parameters_present_flag) {
      // hrd_parameters(1, sps_max_sub_layers_minus1)
      if(!WriteHrdParameters(vui->hrd_parameters.get(),
                             bit_buffer, 1, vui->sps_max_sub_layers_minus1)) {
          return false;
      }
    }
  }

  // bitstream_restriction_flag  u(1)
  if (!bit_buffer->WriteBits(vui->bitstream_restriction_flag, 1)) {
    return false;
  }
    if (vui->bitstream_restriction_flag) {
        // tiles_fixed_structure_flag u(1)
        if (!bit_buffer->WriteBits(vui->tiles_fixed_structure_flag, 1)) {
            return false;
        }
        // motion_vectors_over_pic_boundaries_flag  u(1)
        if (!bit_buffer->WriteBits(vui->motion_vectors_over_pic_boundaries_flag, 1)) {
            return false;
        }
        // restricted_ref_pic_lists_flag  u(1)
        if (!bit_buffer->WriteBits(vui->restricted_ref_pic_lists_flag, 1)) {
            return false;
        }
        // min_spatial_segmentation_idc  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(vui->min_spatial_segmentation_idc)) {
            return false;
        }
        
        // max_bytes_per_pic_denom  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(vui->max_bytes_per_pic_denom)) {
            return false;
        }
        
        // max_bits_per_min_cu_denom  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(vui->max_bits_per_min_cu_denom)) {
            return false;
        }
        
        // log2_max_mv_length_horizontal  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(
                                                vui->log2_max_mv_length_horizontal)) {
                                                    return false;
                                                }
        
        // log2_max_mv_length_vertical  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(vui->log2_max_mv_length_vertical)) {
            return false;
        }
    }

    return true;
}
