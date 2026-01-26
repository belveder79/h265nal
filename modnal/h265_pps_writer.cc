#include "h265_pps_writer.h"
#include "h265_scaling_list_data_writer.h"
#include "h265_pps_scc_extension_writer.h"
#include "h265_pps_multilayer_extension_writer.h"

bool WritePps(H265PpsParser::PpsState* pps, BitBufferWriter* bit_buffer) noexcept {
    
  //uint32_t golomb_tmp;

  // H265 PPS NAL Unit (pic_parameter_set_rbsp()) parser.
  // Section 7.3.2.3 ("Picture parameter set data syntax") of the H.265
  // standard for a complete description.
  //auto pps = std::make_shared<PpsState>();

  // pps_pic_parameter_set_id  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(pps->pps_pic_parameter_set_id)) {
    return false;
  }

  // pps_seq_parameter_set_id  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(pps->pps_seq_parameter_set_id)) {
    return false;
  }

  // dependent_slice_segments_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(pps->dependent_slice_segments_enabled_flag, 1)) {
    return false;
  }

  // output_flag_present_flag  u(1)
  if (!bit_buffer->WriteBits(pps->output_flag_present_flag, 1)) {
    return false;
  }

  // num_extra_slice_header_bits  u(3)
  if (!bit_buffer->WriteBits(pps->num_extra_slice_header_bits, 3)) {
    return false;
  }

  // sign_data_hiding_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(pps->sign_data_hiding_enabled_flag, 1)) {
    return false;
  }

  // cabac_init_present_flag  u(1)
  if (!bit_buffer->WriteBits(pps->cabac_init_present_flag, 1)) {
    return false;
  }

  // num_ref_idx_l0_default_active_minus1  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          pps->num_ref_idx_l0_default_active_minus1)) {
    return false;
  }

  // num_ref_idx_l1_default_active_minus1  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          pps->num_ref_idx_l1_default_active_minus1)) {
    return false;
  }

  // init_qp_minus26  se(v)
  if (!bit_buffer->WriteSignedExponentialGolomb(pps->init_qp_minus26)) {
    return false;
  }

  // constrained_intra_pred_flag  u(1)
  if (!bit_buffer->WriteBits(pps->constrained_intra_pred_flag, 1)) {
    return false;
  }

  // transform_skip_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(pps->transform_skip_enabled_flag, 1)) {
    return false;
  }

  // cu_qp_delta_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(pps->cu_qp_delta_enabled_flag, 1)) {
    return false;
  }

  if (pps->cu_qp_delta_enabled_flag) {
    // diff_cu_qp_delta_depth  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(pps->diff_cu_qp_delta_depth)) {
      return false;
    }
  }

  // pps_cb_qp_offset  se(v)
  if (!bit_buffer->WriteSignedExponentialGolomb(pps->pps_cb_qp_offset)) {
    return false;
  }

  // pps_cr_qp_offset  se(v)
  if (!bit_buffer->WriteSignedExponentialGolomb(pps->pps_cr_qp_offset)) {
    return false;
  }

  // pps_slice_chroma_qp_offsets_present_flag  u(1)
  if (!bit_buffer->WriteBits(1, pps->pps_slice_chroma_qp_offsets_present_flag)) {
    return false;
  }

  // weighted_pred_flag  u(1)
  if (!bit_buffer->WriteBits(1, pps->weighted_pred_flag)) {
    return false;
  }

  // weighted_bipred_flag  u(1)
  if (!bit_buffer->WriteBits(1, pps->weighted_bipred_flag)) {
    return false;
  }

  // transquant_bypass_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(1, pps->transquant_bypass_enabled_flag)) {
    return false;
  }

  // tiles_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(1, pps->tiles_enabled_flag)) {
    return false;
  }

  // entropy_coding_sync_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(1, pps->entropy_coding_sync_enabled_flag)) {
    return false;
  }

  if (pps->tiles_enabled_flag) {
    // num_tile_columns_minus1  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(pps->num_tile_columns_minus1)) {
      return false;
    }
    // num_tile_rows_minus1  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(pps->num_tile_rows_minus1)) {
      return false;
    }
    // uniform_spacing_flag  u(1)
    if (!bit_buffer->WriteBits(1, pps->uniform_spacing_flag)) {
      return false;
    }

    if (!pps->uniform_spacing_flag) {
      for (uint32_t i = 0; i < pps->num_tile_columns_minus1; i++) {
        // column_width_minus1[i]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(pps->column_width_minus1[i])) {
          return false;
        }
      }
      for (uint32_t i = 0; i < pps->num_tile_rows_minus1; i++) {
        // row_height_minus1[i]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(pps->row_height_minus1[i])) {
          return false;
        }
      }
    }

    // loop_filter_across_tiles_enabled_flag u(1)
    if (!bit_buffer->WriteBits(pps->loop_filter_across_tiles_enabled_flag, 1)) {
      return false;
    }
  }

  // pps_loop_filter_across_slices_enabled_flag u(1)
  if (!bit_buffer->WriteBits(pps->pps_loop_filter_across_slices_enabled_flag, 1)) {
    return false;
  }

  // deblocking_filter_control_present_flag  u(1)
  if (!bit_buffer->WriteBits(pps->deblocking_filter_control_present_flag, 1)) {
    return false;
  }

  if (pps->deblocking_filter_control_present_flag) {
    // deblocking_filter_override_enabled_flag u(1)
    if (!bit_buffer->WriteBits(pps->deblocking_filter_override_enabled_flag, 1)) {
      return false;
    }

    // pps_deblocking_filter_disabled_flag  u(1)
    if (!bit_buffer->WriteBits(pps->pps_deblocking_filter_disabled_flag, 1)) {
      return false;
    }

    if (!pps->pps_deblocking_filter_disabled_flag) {
      // pps_beta_offset_div2  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps->pps_beta_offset_div2)) {
        return false;
      }

      // pps_tc_offset_div2  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pps->pps_tc_offset_div2)) {
        return false;
      }
    }
  }

  // pps_scaling_list_data_present_flag  u(1)
  if (!bit_buffer->WriteBits(pps->pps_scaling_list_data_present_flag, 1)) {
    return false;
  }

    if (pps->pps_scaling_list_data_present_flag) {
        if(!WriteScalingListData(pps->scaling_list_data.get(), bit_buffer))
        {
            return false;
        }
    }

  // lists_modification_present_flag  u(1)
  if (!bit_buffer->WriteBits(pps->lists_modification_present_flag, 1)) {
    return false;
  }

  // log2_parallel_merge_level_minus2  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          pps->log2_parallel_merge_level_minus2)) {
    return false;
  }

  // slice_segment_header_extension_present_flag  u(1)
  if (!bit_buffer->WriteBits(pps->slice_segment_header_extension_present_flag, 1)) {
    return false;
  }

  // pps_extension_present_flag  u(1)
  if (!bit_buffer->WriteBits(pps->pps_extension_present_flag, 1)) {
    return false;
  }

  if (pps->pps_extension_present_flag) {
    // pps_range_extension_flag  u(1)
    if (!bit_buffer->WriteBits(pps->pps_range_extension_flag, 1)) {
      return false;
    }

    // pps_multilayer_extension_flag  u(1)
    if (!bit_buffer->WriteBits(pps->pps_multilayer_extension_flag, 1)) {
      return false;
    }

    // pps_3d_extension_flag  u(1)
    if (!bit_buffer->WriteBits(pps->pps_3d_extension_flag, 1)) {
      return false;
    }

    // pps_scc_extension_flag  u(1)
    if (!bit_buffer->WriteBits(pps->pps_scc_extension_flag, 1)) {
      return false;
    }

    // pps_extension_4bits  u(4)
    if (!bit_buffer->WriteBits(pps->pps_extension_4bits, 4)) {
      return false;
    }
  }
#ifdef CHECK
  if (pps->pps_multilayer_extension_flag) {
    // pps_multilayer_extension() // specified in Annex F
    pps->pps_multilayer_extension =
        H265PpsMultilayerExtensionParser::ParsePpsMultilayerExtension(
            bit_buffer);
  }

  if (pps->pps_scc_extension_flag) {
    // pps_range_extension()
    pps->pps_scc_extension =
        H265PpsSccExtensionParser::ParsePpsSccExtension(bit_buffer);
    if (pps->pps_scc_extension == nullptr) {
      return false;
    }
  }
#else
    if (pps->pps_multilayer_extension_flag) {
      // pps_multilayer_extension() // specified in Annex F
          if(!WritePpsMultilayerExtension(pps->pps_multilayer_extension.get(),
                                          bit_buffer)) {
              return false;
          }
    }

    if (pps->pps_scc_extension_flag) {
      // pps_range_extension()
          if(!WritePpsSccExtension(pps->pps_scc_extension.get(), bit_buffer)) {
        return false;
      }
    }
#endif

  if (pps->pps_extension_4bits) {
    while (more_rbsp_data(bit_buffer)) {
      // pps_extension_data_flag  u(1)
      if (!bit_buffer->WriteBits(pps->pps_extension_data_flag, 1)) {
        return false;
      }
    }
  }

  wbsp_trailing_bits(bit_buffer);

  return true;
}
