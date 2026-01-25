
#include "h265_sps_writer.h"

bool WriteSps(h265nal::H265SpsParser::SpsState* sps, h265nal::BitBufferWriter* bit_buffer) noexcept
{
  // sps_video_parameter_set_id  u(4)
  if (!bit_buffer->WriteBits(sps->sps_video_parameter_set_id, 4)) {
    return false;
  }

  // sps_max_sub_layers_minus1  u(3)
  if (!bit_buffer->WriteBits(sps->sps_max_sub_layers_minus1, 3)) {
    return false;
  }

  // sps_temporal_id_nesting_flag  u(1)
  if (!bit_buffer->WriteBits(sps->sps_temporal_id_nesting_flag, 1)) {
    return false;
  }

#ifdef CHECK
  // profile_tier_level(1, sps_max_sub_layers_minus1)
  sps->profile_tier_level = H265ProfileTierLevelParser::ParseProfileTierLevel(
      bit_buffer, true, sps->sps_max_sub_layers_minus1);
  if (sps->profile_tier_level == nullptr) {
    return false;
  }
#else
    std::cout << "UNIMPLEMENTED!" << std::endl;
#endif

  // sps_seq_parameter_set_id  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(sps->sps_seq_parameter_set_id)) {
    return false;
  }

  // chroma_format_idc  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(sps->chroma_format_idc)) {
    return false;
  }

  if (sps->chroma_format_idc == 3) {
    // separate_colour_plane_flag  u(1)
    if (!bit_buffer->WriteBits(sps->separate_colour_plane_flag, 1)) {
      return false;
    }
  }

  // pic_width_in_luma_samples  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(sps->pic_width_in_luma_samples)) {
    return false;
  }

  // pic_height_in_luma_samples  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(sps->pic_height_in_luma_samples)) {
    return false;
  }

  // conformance_window_flag  u(1)
  if (!bit_buffer->WriteBits(sps->conformance_window_flag, 1)) {
    return false;
  }

  if (sps->conformance_window_flag) {
    // conf_win_left_offset  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(sps->conf_win_left_offset)) {
      return false;
    }

    // conf_win_right_offset  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(sps->conf_win_right_offset)) {
      return false;
    }

    // conf_win_top_offset  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(sps->conf_win_top_offset)) {
      return false;
    }

    // conf_win_bottom_offset  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(sps->conf_win_bottom_offset)) {
      return false;
    }
  }

  // bit_depth_luma_minus8  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(sps->bit_depth_luma_minus8)) {
    return false;
  }

  // bit_depth_chroma_minus8  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(sps->bit_depth_chroma_minus8)) {
    return false;
  }

  // log2_max_pic_order_cnt_lsb_minus4  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          sps->log2_max_pic_order_cnt_lsb_minus4)) {
    return false;
  }

  // sps_sub_layer_ordering_info_present_flag  u(1)
  if (!bit_buffer->WriteBits(sps->sps_sub_layer_ordering_info_present_flag, 1)) {
    return false;
  }

  for (uint32_t i = (sps->sps_sub_layer_ordering_info_present_flag
                         ? 0
                         : sps->sps_max_sub_layers_minus1);
       i <= sps->sps_max_sub_layers_minus1; i++) {
    // sps_max_dec_pic_buffering_minus1[i]  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(sps->sps_max_dec_pic_buffering_minus1[i])) {
      return false;
    }

    // sps_max_num_reorder_pics[i]  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(sps->sps_max_num_reorder_pics[i])) {
      return false;
    }

    // sps_max_latency_increase_plus1[i]  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(sps->sps_max_latency_increase_plus1[i])) {
      return false;
    }
  }

  // log2_min_luma_coding_block_size_minus3  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          sps->log2_min_luma_coding_block_size_minus3)) {
    return false;
  }

  // log2_diff_max_min_luma_coding_block_size  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          sps->log2_diff_max_min_luma_coding_block_size)) {
    return false;
  }

  // log2_min_luma_transform_block_size_minus2  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          sps->log2_min_luma_transform_block_size_minus2)) {
    return false;
  }

  // log2_diff_max_min_luma_transform_block_size  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          sps->log2_diff_max_min_luma_transform_block_size)) {
    return false;
  }

  // max_transform_hierarchy_depth_inter  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          sps->max_transform_hierarchy_depth_inter)) {
    return false;
  }

  // max_transform_hierarchy_depth_intra  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          sps->max_transform_hierarchy_depth_intra)) {
    return false;
  }

  // scaling_list_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(sps->scaling_list_enabled_flag, 1)) {
    return false;
  }

  if (sps->scaling_list_enabled_flag) {
    // sps_scaling_list_data_present_flag  u(1)
    if (!bit_buffer->WriteBits(sps->sps_scaling_list_data_present_flag, 1)) {
      return false;
    }
#ifdef CHECK
    if (sps->sps_scaling_list_data_present_flag) {
      // scaling_list_data()
      sps->scaling_list_data =
          H265ScalingListDataParser::ParseScalingListData(bit_buffer);
      if (sps->scaling_list_data == nullptr) {
        return false;
      }
    }
#else
      std::cout << "UNIMPLEMENTED!" << std::endl;
#endif
  }

  // amp_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(sps->amp_enabled_flag, 1)) {
    return false;
  }

  // sample_adaptive_offset_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(sps->sample_adaptive_offset_enabled_flag, 1)) {
    return false;
  }

  // pcm_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(sps->pcm_enabled_flag, 1)) {
    return false;
  }

  if (sps->pcm_enabled_flag) {
    // pcm_sample_bit_depth_luma_minus1  u(4)
    if (!bit_buffer->WriteBits(sps->pcm_sample_bit_depth_luma_minus1, 4)) {
      return false;
    }

    // pcm_sample_bit_depth_chroma_minus1  u(4)
    if (!bit_buffer->WriteBits(sps->pcm_sample_bit_depth_chroma_minus1, 4)) {
      return false;
    }

    // log2_min_pcm_luma_coding_block_size_minus3  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(
            sps->log2_min_pcm_luma_coding_block_size_minus3)) {
      return false;
    }

    // log2_diff_max_min_pcm_luma_coding_block_size  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(
            sps->log2_diff_max_min_pcm_luma_coding_block_size)) {
      return false;
    }

    // pcm_loop_filter_disabled_flag  u(1)
    if (!bit_buffer->WriteBits(sps->pcm_loop_filter_disabled_flag, 1)) {
      return false;
    }
  }

  // num_short_term_ref_pic_sets
  if (!bit_buffer->WriteExponentialGolomb(sps->num_short_term_ref_pic_sets)) {
    return false;
  }
#ifdef CHECK
  for (uint32_t i = 0; i < sps->num_short_term_ref_pic_sets; i++) {
    uint32_t max_num_negative_pics = 0;
    if (!sps->getMaxNumNegativePics(&max_num_negative_pics)) {
      return false;
    }
    // st_ref_pic_set(i)
    auto st_ref_pic_set_item = H265StRefPicSetParser::ParseStRefPicSet(
        bit_buffer, i, sps->num_short_term_ref_pic_sets, &(sps->st_ref_pic_set),
        max_num_negative_pics);
    if (st_ref_pic_set_item == nullptr) {
      // not enough bits for the st_ref_pic_set
      return false;
    }
    sps->st_ref_pic_set.push_back(std::move(st_ref_pic_set_item));
  }
#else
    std::cout << "UNIMPLEMENTED!" << std::endl;
#endif

  // long_term_ref_pics_present_flag  u(1)
  if (!bit_buffer->WriteBits(sps->long_term_ref_pics_present_flag, 1)) {
    return false;
  }

  if (sps->long_term_ref_pics_present_flag) {
    // num_long_term_ref_pics_sps  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(sps->num_long_term_ref_pics_sps)) {
      return false;
    }

    for (uint32_t i = 0; i < sps->num_long_term_ref_pics_sps; i++) {
      // lt_ref_pic_poc_lsb_sps[i] u(v)  log2_max_pic_order_cnt_lsb_minus4 + 4
      if (!bit_buffer->WriteBits(sps->lt_ref_pic_poc_lsb_sps[i], sps->log2_max_pic_order_cnt_lsb_minus4 + 4)) {
        return false;
      }

      // used_by_curr_pic_lt_sps_flag[i]  u(1)
      if (!bit_buffer->WriteBits(sps->used_by_curr_pic_lt_sps_flag[i], 1)) {
        return false;
      }

    }
  }

  // sps_temporal_mvp_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(sps->sps_temporal_mvp_enabled_flag, 1)) {
    return false;
  }

  // strong_intra_smoothing_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(sps->strong_intra_smoothing_enabled_flag, 1)) {
    return false;
  }

  // vui_parameters_present_flag  u(1)
  if (!bit_buffer->WriteBits(sps->vui_parameters_present_flag, 1)) {
    return false;
  }
#ifdef CHECK
  if (sps->vui_parameters_present_flag) {
    // vui_parameters()
    sps->vui_parameters = H265VuiParametersParser::ParseVuiParameters(
        bit_buffer, sps->sps_max_sub_layers_minus1);
    if (sps->vui_parameters == nullptr) {
      return false;
    }
  }
#else
    std::cout << "UNIMPLEMENTED!" << std::endl;
#endif

  // sps_extension_present_flag  u(1)
  if (!bit_buffer->WriteBits(sps->sps_extension_present_flag, 1)) {
    return false;
  }

  if (sps->sps_extension_present_flag) {
    // sps_range_extension_flag  u(1)
    if (!bit_buffer->WriteBits(sps->sps_range_extension_flag, 1)) {
      return false;
    }

    // sps_multilayer_extension_flag  u(1)
    if (!bit_buffer->WriteBits(sps->sps_multilayer_extension_flag, 1)) {
      return false;
    }

    // sps_3d_extension_flag  u(1)
    if (!bit_buffer->WriteBits(sps->sps_3d_extension_flag, 1)) {
      return false;
    }

    // sps_scc_extension_flag  u(1)
    if (!bit_buffer->WriteBits(sps->sps_scc_extension_flag, 1)) {
      return false;
    }

    // sps_extension_4bits  u(4)
    if (!bit_buffer->WriteBits(sps->sps_extension_4bits, 4)) {
      return false;
    }
  }
#ifdef CHECK
  if (sps->sps_range_extension_flag) {
    // sps_range_extension()
    sps->sps_range_extension =
        H265SpsRangeExtensionParser::ParseSpsRangeExtension(bit_buffer);
    if (sps->sps_range_extension == nullptr) {
      return false;
    }
  }
    
  if (sps->sps_multilayer_extension_flag) {
    // sps_multilayer_extension() // specified in Annex F
    sps->sps_multilayer_extension =
        H265SpsMultilayerExtensionParser::ParseSpsMultilayerExtension(
            bit_buffer);
    if (sps->sps_multilayer_extension == nullptr) {
      return false;
    }
  }

  if (sps->sps_3d_extension_flag) {
    // sps_3d_extension() // specified in Annex I
    sps->sps_3d_extension =
        H265Sps3dExtensionParser::ParseSps3dExtension(bit_buffer);
    if (sps->sps_3d_extension == nullptr) {
      return false;
    }
  }

  if (sps->sps_scc_extension_flag) {
    // sps_scc_extension()
    sps->sps_scc_extension = H265SpsSccExtensionParser::ParseSpsSccExtension(
        bit_buffer, sps->chroma_format_idc, sps->bit_depth_luma_minus8,
        sps->bit_depth_chroma_minus8);
    if (sps->sps_scc_extension == nullptr) {
      return false;
    }
  }
#else
    std::cout << "UNIMPLEMENTED!" << std::endl;
#endif

  if (sps->sps_extension_4bits) {
    while (more_rbsp_data(bit_buffer)) {
      // sps_extension_data_flag  u(1)
      if (!bit_buffer->WriteBits(sps->sps_extension_data_flag, 1)) {
        return false;
      }
    }
  }

  wbsp_trailing_bits(bit_buffer);

  return true;
}
