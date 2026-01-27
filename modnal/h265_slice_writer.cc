#include "h265_slice_writer.h"
#include "h265_pred_weight_table_writer.h"
#include "h265_st_ref_pic_set_writer.h"

bool WriteSliceSegmentHeader(H265SliceSegmentHeaderParser::SliceSegmentHeaderState* slice_segment_header,
                             H265SpsParser::SpsState* sps, BitBufferWriter* bit_buffer) noexcept
{
    // H265 slice segment header (slice_segment_layer_rbsp()) NAL Unit.
    // Section 7.3.6.1 ("General slice segment header syntax") of the H.265
    // standard for a complete description.
    
    // first_slice_segment_in_pic_flag  u(1)
    if (!bit_buffer->WriteBits(
                               slice_segment_header->first_slice_segment_in_pic_flag, 1)) {
                                   return false;
                               }
    
    if (slice_segment_header->nal_unit_type >= BLA_W_LP &&
        slice_segment_header->nal_unit_type <= RSV_IRAP_VCL23) {
        // no_output_of_prior_pics_flag  u(1)
        if (!bit_buffer->WriteBits(
                                   slice_segment_header->no_output_of_prior_pics_flag,1)) {
                                       return false;
                                   }
    }
    
    // slice_pic_parameter_set_id  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(
                                            slice_segment_header->slice_pic_parameter_set_id)) {
                                                return false;
                                            }
    
    if (!slice_segment_header->first_slice_segment_in_pic_flag) {
        if (slice_segment_header->dependent_slice_segments_enabled_flag) {
            // dependent_slice_segment_flag  u(1)
            if (!bit_buffer->WriteBits(
                                       slice_segment_header->dependent_slice_segment_flag, 1)) {
                                           return false;
                                       }
        }
        
        size_t PicSizeInCtbsY = sps->getPicSizeInCtbsY();
        size_t slice_segment_address_len = static_cast<size_t>(
                                                               std::ceil(std::log2(static_cast<float>(PicSizeInCtbsY))));
        
        // range: 0 to PicSizeInCtbsY - 1
        // slice_segment_address  u(v)
        if (!bit_buffer->WriteBits(slice_segment_header->slice_segment_address, slice_segment_address_len)) {
            return false;
        }
    }
    
    if (!slice_segment_header->dependent_slice_segment_flag) {
        for (uint32_t i = 0; i < slice_segment_header->num_extra_slice_header_bits;
             i++) {
            // slice_reserved_flag[i]  u(1)
            if (!bit_buffer->WriteBits(slice_segment_header->slice_reserved_flag[i], 1)) {
                return false;
            }
        }
        
        // slice_type  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(slice_segment_header->slice_type)) {
            return false;
        }
        
        if (slice_segment_header->output_flag_present_flag) {
            // pic_output_flag  u(1)
            if (!bit_buffer->WriteBits(slice_segment_header->pic_output_flag, 1)) {
                return false;
            }
        }
        
        if (slice_segment_header->separate_colour_plane_flag == 1) {
            // colour_plane_id  u(2)
            if (!bit_buffer->WriteBits(slice_segment_header->colour_plane_id, 2)) {
                return false;
            }
        }
        
        if (slice_segment_header->nal_unit_type != IDR_W_RADL &&
            slice_segment_header->nal_unit_type != IDR_N_LP) {
            // length of the slice_pic_order_cnt_lsb syntax element is
            // log2_max_pic_order_cnt_lsb_minus4 + 4 bits. The value of the
            // slice_pic_order_cnt_lsb shall be in the range of 0 to
            // MaxPicOrderCntLsb - 1, inclusive.
            size_t slice_pic_order_cnt_lsb_len =
            slice_segment_header->log2_max_pic_order_cnt_lsb_minus4 + 4;
            // slice_pic_order_cnt_lsb  u(v)
            if (!bit_buffer->WriteBits(slice_segment_header->slice_pic_order_cnt_lsb, slice_pic_order_cnt_lsb_len)) {
                return false;
            }
            
            // short_term_ref_pic_set_sps_flag  u(1)
            if (!bit_buffer->WriteBits(
                                       slice_segment_header->short_term_ref_pic_set_sps_flag, 1)) {
                                           return false;
                                       }
            
            if (!slice_segment_header->short_term_ref_pic_set_sps_flag) {
                
                // use ST RPS defined directly in the slice segment header
                // st_ref_pic_set(num_short_term_ref_pic_sets)
                uint32_t max_num_negative_pics = 0;
                if (!sps->getMaxNumNegativePics(&max_num_negative_pics)) {
                    return false;
                }
                if(!WriteStRefPicSet(slice_segment_header->st_ref_pic_set.get(),
                                     bit_buffer)) {
                    return false;
                }
                
            } else if (slice_segment_header->num_short_term_ref_pic_sets > 1) {
                
                // use one of the ST RPS defined in the SPS
                // Ceil(Log2(num_short_term_ref_pic_sets));
                size_t short_term_ref_pic_set_idx_len =
                static_cast<size_t>(std::ceil(std::log2(static_cast<float>(
                                                                           slice_segment_header->num_short_term_ref_pic_sets))));
                // short_term_ref_pic_set_idx  u(v)
                if (!bit_buffer->WriteBits(slice_segment_header->short_term_ref_pic_set_idx, short_term_ref_pic_set_idx_len)) {
                    return false;
                }
                
            }
            
            if (slice_segment_header->long_term_ref_pics_present_flag) {
                if (slice_segment_header->num_long_term_ref_pics_sps > 0) {
                    // num_long_term_sps  ue(v)
                    if (!bit_buffer->WriteExponentialGolomb(
                                                            slice_segment_header->num_long_term_sps)) {
                                                                return false;
                                                            }
                }
                
                // num_long_term_pics  ue(v)
                if (!bit_buffer->WriteExponentialGolomb(
                                                        slice_segment_header->num_long_term_pics)) {
                                                            return false;
                                                        }
                
                for (uint32_t i = 0; i < slice_segment_header->num_long_term_sps +
                     slice_segment_header->num_long_term_pics;
                     i++) {
                    if (i < slice_segment_header->num_long_term_sps) {
                        if (slice_segment_header->num_long_term_ref_pics_sps > 1) {
                            // lt_idx_sps[i]  u(v)
                            // number of bits used to represent lt_idx_sps[i] is equal to
                            // Ceil(Log2(num_long_term_ref_pics_sps)).
                            size_t lt_idx_sps_len =
                            static_cast<size_t>(std::ceil(std::log2(static_cast<float>(
                                                                                       slice_segment_header->num_long_term_ref_pics_sps))));
                            if (!bit_buffer->WriteBits(slice_segment_header->lt_idx_sps[i], lt_idx_sps_len)) {
                                return false;
                            }
                        }
                        
                    } else {
                        // poc_lsb_lt[i]  u(v)
                        // length of the poc_lsb_lt[i] syntax element is
                        // log2_max_pic_order_cnt_lsb_minus4 + 4 bits. [...]
                        // value of lt_idx_sps[i] shall be in the range of 0 to
                        // num_long_term_ref_pics_sps - 1, inclusive.
                        size_t poc_lsb_lt_len =
                        slice_segment_header->log2_max_pic_order_cnt_lsb_minus4 + 4;
                        // slice_pic_order_cnt_lsb  u(v)
                        if (!bit_buffer->WriteBits(slice_segment_header->poc_lsb_lt[i], poc_lsb_lt_len)) {
                            return false;
                        }
                        
                        // used_by_curr_pic_lt_flag[i]  u(1)
                        if (!bit_buffer->WriteBits(slice_segment_header->used_by_curr_pic_lt_flag[i], 1)) {
                            return false;
                        }
                    }
                    
                    // delta_poc_msb_present_flag[i]  u(1)
                    if (!bit_buffer->WriteBits(slice_segment_header->delta_poc_msb_present_flag[i], 1)) {
                        return false;
                    }
                    
                    if (slice_segment_header->delta_poc_msb_present_flag[i]) {
                        // delta_poc_msb_cycle_lt[i]  ue(v)
                        if (!bit_buffer->WriteExponentialGolomb(slice_segment_header->delta_poc_msb_cycle_lt[i])) {
                            return false;
                        }
                    }
                }
            }
            
            if (slice_segment_header->sps_temporal_mvp_enabled_flag) {
                // slice_temporal_mvp_enabled_flag  u(1)
                if (!bit_buffer->WriteBits(
                                           slice_segment_header->slice_temporal_mvp_enabled_flag, 1)) {
                                               return false;
                                           }
            }
        }
        
        if (slice_segment_header->sample_adaptive_offset_enabled_flag) {
            // slice_sao_luma_flag  u(1)
            if (!bit_buffer->WriteBits(slice_segment_header->slice_sao_luma_flag, 1)) {
                return false;
            }
            
            if (slice_segment_header->ChromaArrayType != 0) {
                // slice_sao_chroma_flag  u(1)
                if (!bit_buffer->WriteBits(
                                           slice_segment_header->slice_sao_chroma_flag, 1)) {
                                               return false;
                                           }
            }
        }
        
        if (slice_segment_header->slice_type == SliceType_P ||
            slice_segment_header->slice_type == SliceType_B) {
            // num_ref_idx_active_override_flag  u(1)
            if (!bit_buffer->WriteBits(
                                       slice_segment_header->num_ref_idx_active_override_flag, 1)) {
                                           return false;
                                       }
            
            if (slice_segment_header->num_ref_idx_active_override_flag) {
                // num_ref_idx_l0_active_minus1  ue(v)
                if (!bit_buffer->WriteExponentialGolomb(
                                                        slice_segment_header->num_ref_idx_l0_active_minus1)) {
                                                            return false;
                                                        }
                
                if (slice_segment_header->slice_type == SliceType_B) {
                    // num_ref_idx_l1_active_minus1  ue(v)
                    if (!bit_buffer->WriteExponentialGolomb(
                                                            slice_segment_header->num_ref_idx_l1_active_minus1)) {
                                                                return false;
                                                            }
                }
            }
            /*C
             slice_segment_header->lists_modification_present_flag =
             pps->lists_modification_present_flag;
             // TODO(chemag): calculate NumPicTotalCurr support (page 99)
             slice_segment_header->NumPicTotalCurr = 0;
             if (slice_segment_header->lists_modification_present_flag &&
             slice_segment_header->NumPicTotalCurr > 1) {
             // ref_pic_lists_modification()
             // TODO(chemag): add support for ref_pic_lists_modification()
             #ifdef FPRINT_ERRORS
             fprintf(stderr,
             "error: unimplemented ref_pic_lists_modification in "
             "slice_header\n");
             #endif  // FPRINT_ERRORS
             }
             */
            if (slice_segment_header->slice_type == SliceType_B) {
                // mvd_l1_zero_flag  u(1)
                if (!bit_buffer->WriteBits(slice_segment_header->mvd_l1_zero_flag, 1)) {
                    return false;
                }
            }
            
            //C slice_segment_header->cabac_init_present_flag = pps->cabac_init_present_flag;
            if (slice_segment_header->cabac_init_present_flag) {
                // cabac_init_flag  u(1)
                if (!bit_buffer->WriteBits(slice_segment_header->cabac_init_flag, 1)) {
                    return false;
                }
            }
            
            if (slice_segment_header->slice_temporal_mvp_enabled_flag) {
                if (slice_segment_header->slice_type == SliceType_B) {
                    // collocated_from_l0_flag  u(1)
                    if (!bit_buffer->WriteBits(
                                               slice_segment_header->collocated_from_l0_flag, 1)) {
                                                   return false;
                                               }
                }
                if ((slice_segment_header->collocated_from_l0_flag &&
                     slice_segment_header->num_ref_idx_l0_active_minus1 > 0) ||
                    (!slice_segment_header->collocated_from_l0_flag &&
                     slice_segment_header->num_ref_idx_l1_active_minus1 > 0)) {
                    // collocated_ref_idx  ue(v)
                    if (!bit_buffer->WriteExponentialGolomb(
                                                            slice_segment_header->collocated_ref_idx)) {
                                                                return false;
                                                            }
                }
            }
            
            //C slice_segment_header->weighted_pred_flag = pps->weighted_pred_flag;
            //C slice_segment_header->weighted_bipred_flag = pps->weighted_bipred_flag;
            if ((slice_segment_header->weighted_pred_flag &&
                 slice_segment_header->slice_type == SliceType_P) ||
                (slice_segment_header->weighted_bipred_flag &&
                 slice_segment_header->slice_type == SliceType_B)) {
                // pred_weight_table()
                if(!WritePredWeightTable(slice_segment_header->pred_weight_table.get(),
                                         bit_buffer)) {
                    return false;
                }
            }
            
            // five_minus_max_num_merge_cand  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(
                                                    slice_segment_header->five_minus_max_num_merge_cand)) {
                                                        return false;
                                                    }
            
            if (slice_segment_header->motion_vector_resolution_control_idc == 2) {
                // use_integer_mv_flag  u(1)
                if (!bit_buffer->WriteBits(slice_segment_header->use_integer_mv_flag, 1)) {
                    return false;
                }
            }
        }
        // slice_qp_delta  se(v)
        if (!bit_buffer->WriteSignedExponentialGolomb(
                                                      slice_segment_header->slice_qp_delta)) {
                                                          return false;
                                                      }
        
        if (slice_segment_header->pps_slice_chroma_qp_offsets_present_flag) {
            // slice_cb_qp_offset  se(v)
            if (!bit_buffer->WriteSignedExponentialGolomb(
                                                          slice_segment_header->slice_cb_qp_offset)) {
                                                              return false;
                                                          }
            
            // slice_cr_qp_offset  se(v)
            if (!bit_buffer->WriteSignedExponentialGolomb(
                                                          slice_segment_header->slice_cr_qp_offset)) {
                                                              return false;
                                                          }
        }
        
        if (slice_segment_header->pps_slice_act_qp_offsets_present_flag) {
            // slice_act_y_qp_offset  se(v)
            if (!bit_buffer->WriteSignedExponentialGolomb(
                                                          slice_segment_header->slice_act_y_qp_offset)) {
                                                              return false;
                                                          }
            
            // slice_act_cb_qp_offset  se(v)
            if (!bit_buffer->WriteSignedExponentialGolomb(
                                                          slice_segment_header->slice_act_cb_qp_offset)) {
                                                              return false;
                                                          }
            
            // slice_act_cr_qp_offset  se(v)
            if (!bit_buffer->WriteSignedExponentialGolomb(
                                                          slice_segment_header->slice_act_cr_qp_offset)) {
                                                              return false;
                                                          }
        }
        
        // TODO(chemag): add support for pps_range_extension()
        // slice_segment_header->chroma_qp_offset_list_enabled_flag =
        //    pps->pps_range_extension->chroma_qp_offset_list_enabled_flag;
        if (slice_segment_header->chroma_qp_offset_list_enabled_flag) {
            // cu_chroma_qp_offset_enabled_flag  u(1)
            if (!bit_buffer->WriteBits(
                                       slice_segment_header->cu_chroma_qp_offset_enabled_flag, 1)) {
                                           return false;
                                       }
        }
        
        if (slice_segment_header->deblocking_filter_override_enabled_flag) {
            // deblocking_filter_override_flag  u(1)
            if (!bit_buffer->WriteBits(
                                       slice_segment_header->deblocking_filter_override_flag, 1)) {
                                           return false;
                                       }
        }
        
        if (slice_segment_header->deblocking_filter_override_flag) {
            // slice_deblocking_filter_disabled_flag  u(1)
            if (!bit_buffer->WriteBits(
                                       1, slice_segment_header->slice_deblocking_filter_disabled_flag)) {
                                           return false;
                                       }
            
            if (!slice_segment_header->slice_deblocking_filter_disabled_flag) {
                // slice_beta_offset_div2 se(v)
                if (!bit_buffer->WriteSignedExponentialGolomb(
                                                              slice_segment_header->slice_beta_offset_div2)) {
                                                                  return false;
                                                              }
                
                // slice_tc_offset_div2 se(v)
                if (!bit_buffer->WriteSignedExponentialGolomb(
                                                              slice_segment_header->slice_tc_offset_div2)) {
                                                                  return false;
                                                              }
            }
        }
        
        if (slice_segment_header->pps_loop_filter_across_slices_enabled_flag &&
            (slice_segment_header->slice_sao_luma_flag ||
             slice_segment_header->slice_sao_chroma_flag ||
             !slice_segment_header->slice_deblocking_filter_disabled_flag)) {
            // slice_loop_filter_across_slices_enabled_flag  u(1)
            if (!bit_buffer->WriteBits(
                                       slice_segment_header
                                       ->slice_loop_filter_across_slices_enabled_flag, 1)) {
                                           return false;
                                       }
        }
    }
    
    if (slice_segment_header->tiles_enabled_flag ||
        slice_segment_header->entropy_coding_sync_enabled_flag) {
        // num_entry_point_offsets  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(
                                                slice_segment_header->num_entry_point_offsets)) {
                                                    return false;
                                                }
        if (slice_segment_header->num_entry_point_offsets > 0) {
            // offset_len_minus1  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(
                                                    slice_segment_header->offset_len_minus1)) {
                                                        return false;
                                                    }
            
            for (uint32_t i = 0; i < slice_segment_header->num_entry_point_offsets;
                 i++) {
                // entry_point_offset_minus1[i]  u(v)
                if (!bit_buffer->WriteBits(slice_segment_header->entry_point_offset_minus1[i], slice_segment_header->offset_len_minus1 + 1)) {
                    return false;
                }
            }
        }
    }
    
    if (slice_segment_header->slice_segment_header_extension_present_flag) {
        // slice_segment_header_extension_length  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(
                                                slice_segment_header->slice_segment_header_extension_length)) {
                                                    return false;
                                                }
        for (uint32_t i = 0;
             i < slice_segment_header->slice_segment_header_extension_length; i++) {
            // slice_segment_header_extension_data_byte[i]  u(8)
            if (!bit_buffer->WriteBits(slice_segment_header->slice_segment_header_extension_data_byte[i], 8)) {
                return false;
            }
        }
    }
    
    // TODO(chemag): implement byte_alignment()
    // byte_alignment()
    return true;
}

bool WriteSliceSegmentLayer(H265SliceSegmentLayerParser::SliceSegmentLayerState* slice_segment_layer,
                            H265SpsParser::SpsState* sps, BitBufferWriter* bit_buffer) noexcept {
    // H265 slice segment layer (slice_segment_layer_rbsp()) NAL Unit.
    // Section 7.3.2.9 ("Slice segment layer RBSP syntax") of the H.265
    // standard for a complete description.
    // auto slice_segment_layer = std::make_unique<SliceSegmentLayerState>();
    
    if(!WriteSliceSegmentHeader(slice_segment_layer->slice_segment_header.get(), sps, bit_buffer))
        return false;
    
    // slice_segment_data()
    // rbsp_slice_segment_trailing_bits()
    for (size_t i = 0; i < slice_segment_layer->payload.size(); i++) {
        // user_data_payload_byte  b(8)
        if (!bit_buffer->WriteUInt8(slice_segment_layer->payload[i])) {
            return false;
        }
    }
    
    return true;
}
