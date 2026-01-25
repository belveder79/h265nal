#include "h265_vps_writer.h"

bool WriteProfileInfo(h265nal::H265ProfileInfoParser::ProfileInfoState* profile_info, h265nal::BitBufferWriter* bit_buffer) noexcept
{
    //#if REVERSE_THIS
    uint32_t bits_tmp, bits_tmp_hi;
    //#endif
    
    // profile_space  u(2)
    if (!bit_buffer->WriteBits(profile_info->profile_space, 2)) {
        return false;
    }
    // tier_flag  u(1)
    if (!bit_buffer->WriteBits(profile_info->tier_flag, 1)) {
        return false;
    }
    // profile_idc  u(5)
    if (!bit_buffer->WriteBits(profile_info->profile_idc, 5)) {
        return false;
    }
    // for (j = 0; j < 32; j++)
    for (uint32_t j = 0; j < 32; j++) {
        // profile_compatibility_flag[j]  u(1)
        if (!bit_buffer->WriteBits(profile_info->profile_compatibility_flag[j], 1)) {
            return false;
        }
    }
//    return true; // TODO
//----
    // progressive_source_flag  u(1)
    if (!bit_buffer->WriteBits(profile_info->progressive_source_flag, 1)) {
        return false;
    }
    // interlaced_source_flag  u(1)
    if (!bit_buffer->WriteBits(profile_info->interlaced_source_flag, 1)) {
        return false;
    }
    // non_packed_constraint_flag  u(1)
    if (!bit_buffer->WriteBits(profile_info->non_packed_constraint_flag, 1)) {
        return false;
    }
    // frame_only_constraint_flag  u(1)
    if (!bit_buffer->WriteBits(profile_info->frame_only_constraint_flag, 1)) {
        return false;
    }
    //wbsp_trailing_bits(bit_buffer);
    //return true; // TODO
    
    if (profile_info->profile_idc == 4 ||
        profile_info->profile_compatibility_flag[4] == 1 ||
        profile_info->profile_idc == 5 ||
        profile_info->profile_compatibility_flag[5] == 1 ||
        profile_info->profile_idc == 6 ||
        profile_info->profile_compatibility_flag[6] == 1 ||
        profile_info->profile_idc == 7 ||
        profile_info->profile_compatibility_flag[7] == 1 ||
        profile_info->profile_idc == 8 ||
        profile_info->profile_compatibility_flag[8] == 1 ||
        profile_info->profile_idc == 9 ||
        profile_info->profile_compatibility_flag[9] == 1 ||
        profile_info->profile_idc == 10 ||
        profile_info->profile_compatibility_flag[10] == 1 ||
        profile_info->profile_idc == 11 ||
        profile_info->profile_compatibility_flag[11] == 1) {
        // The number of bits in this syntax structure is not affected by
        // this condition
        // max_12bit_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->max_12bit_constraint_flag, 1)) {
            return false;
        }
        // max_10bit_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->max_10bit_constraint_flag, 1)) {
            return false;
        }
        // max_8bit_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->max_8bit_constraint_flag, 1)) {
            return false;
        }
        // max_422chroma_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->max_422chroma_constraint_flag, 1)) {
            return false;
        }
        // max_420chroma_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->max_420chroma_constraint_flag, 1)) {
            return false;
        }
        // max_monochrome_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->max_monochrome_constraint_flag, 1)) {
            return false;
        }
        // intra_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->intra_constraint_flag, 1)) {
            return false;
        }
        // one_picture_only_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->one_picture_only_constraint_flag, 1)) {
            return false;
        }
        // lower_bit_rate_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->lower_bit_rate_constraint_flag, 1)) {
            return false;
        }
        if (profile_info->profile_idc == 5 ||
            profile_info->profile_compatibility_flag[5] == 1 ||
            profile_info->profile_idc == 9 ||
            profile_info->profile_compatibility_flag[9] == 1 ||
            profile_info->profile_idc == 10 ||
            profile_info->profile_compatibility_flag[10] == 1 ||
            profile_info->profile_idc == 11 ||
            profile_info->profile_compatibility_flag[11] == 1) {
            // max_14bit_constraint_flag  u(1)
            if (!bit_buffer->WriteBits(profile_info->max_14bit_constraint_flag, 1)) {
                return false;
            }
            
            bits_tmp_hi = (uint32_t)(profile_info->reserved_zero_33bits >> 32);
            bits_tmp = (uint32_t)profile_info->reserved_zero_33bits;
            
            // reserved_zero_33bits  u(33)
            if (!bit_buffer->WriteBits(bits_tmp_hi, 1)) {
                return false;
            }
            if (!bit_buffer->WriteBits(bits_tmp, 32)) {
                return false;
            }
        } else {
            bits_tmp_hi = (uint32_t)(profile_info->reserved_zero_34bits >> 32);
            bits_tmp = (uint32_t)profile_info->reserved_zero_34bits;
            
            if (!bit_buffer->WriteBits(bits_tmp_hi, 2)) {
                return false;
            }
            if (!bit_buffer->WriteBits(bits_tmp, 32)) {
                return false;
            }
        }
    } else if (profile_info->profile_idc == 2 ||
               profile_info->profile_compatibility_flag[2] == 1) {
        // reserved_zero_7bits  u(7)
        if (!bit_buffer->WriteBits(profile_info->reserved_zero_7bits, 7)) {
            return false;
        }
        // one_picture_only_constraint_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->one_picture_only_constraint_flag, 1)) {
            return false;
        }
        bits_tmp_hi = (uint32_t)(profile_info->reserved_zero_35bits >> 32);
        bits_tmp = (uint32_t)profile_info->reserved_zero_35bits;
        // reserved_zero_35bits  u(35)
        if (!bit_buffer->WriteBits(bits_tmp_hi, 3)) {
            return false;
        }
        if (!bit_buffer->WriteBits(bits_tmp, 32)) {
            return false;
        }
    } else {
        bits_tmp_hi = (uint32_t)(profile_info->reserved_zero_43bits >> 32);
        bits_tmp = (uint32_t)profile_info->reserved_zero_43bits;
        
        // reserved_zero_43bits  u(43)
        if (!bit_buffer->WriteBits(bits_tmp_hi, 11)) {
            return false;
        }
        if (!bit_buffer->WriteBits(bits_tmp, 32)) {
            return false;
        }
    }
    // get profile type
    profile_info->profile_type = profile_info->GetProfileType();
    // The number of bits in this syntax structure is not affected by
    // this condition
    if (profile_info->profile_idc == 1 ||
        profile_info->profile_compatibility_flag[1] == 1 ||
        profile_info->profile_idc == 2 ||
        profile_info->profile_compatibility_flag[2] == 1 ||
        profile_info->profile_idc == 3 ||
        profile_info->profile_compatibility_flag[3] == 1 ||
        profile_info->profile_idc == 4 ||
        profile_info->profile_compatibility_flag[4] == 1 ||
        profile_info->profile_idc == 5 ||
        profile_info->profile_compatibility_flag[5] == 1 ||
        profile_info->profile_idc == 9 ||
        profile_info->profile_compatibility_flag[9] == 1 ||
        profile_info->profile_idc == 11 ||
        profile_info->profile_compatibility_flag[11] == 1) {
        // inbld_flag  u(1)
        if (!bit_buffer->WriteBits(profile_info->inbld_flag, 1)) {
            return false;
        }
    } else {
        // reserved_zero_bit  u(1)
        if (!bit_buffer->WriteBits(profile_info->reserved_zero_bit, 1)) {
            return false;
        }
    }
    
    return true;
}

bool WriteProfileTierLevel(h265nal::H265ProfileTierLevelParser::ProfileTierLevelState* profile_tier_level, h265nal::BitBufferWriter* bit_buffer) noexcept
{
    // profile_tier_level() parser.
    // Section 7.3.3 ("Profile, tier and level syntax") of the H.265
    // standard for a complete description.
    
    if (profile_tier_level->profilePresentFlag) {
        WriteProfileInfo(profile_tier_level->general.get(), bit_buffer);
    }
    //return true; //TODO
    
    // general_level_idc  u(8)
    if (!bit_buffer->WriteBits(profile_tier_level->general_level_idc, 8)) {
        return false;
    }
    
    for (uint32_t i = 0; i < profile_tier_level->maxNumSubLayersMinus1; i++) {
        if (!bit_buffer->WriteBits(profile_tier_level->sub_layer_profile_present_flag[i], 1)) {
            return false;
        }
        if (!bit_buffer->WriteBits(profile_tier_level->sub_layer_level_present_flag[i], 1)) {
            return false;
        }
    }
    
    if (profile_tier_level->maxNumSubLayersMinus1 > 0) {
        for (uint32_t i = profile_tier_level->maxNumSubLayersMinus1; i < 8; i++) {
            if (!bit_buffer->WriteBits(profile_tier_level->reserved_zero_2bits[i-1], 2)) {
                return false;
            }
        }
    }
    
    for (uint32_t i = 0; i < profile_tier_level->maxNumSubLayersMinus1; i++) {
        if (profile_tier_level->sub_layer_profile_present_flag[i]) {
            WriteProfileInfo(profile_tier_level->sub_layer[i].get(),bit_buffer);
        }
        
        if (profile_tier_level->sub_layer_profile_present_flag[i]) {
            if (!bit_buffer->WriteBits(profile_tier_level->sub_layer_level_idc[i], 8)) {
                return false;
            }
        }
    }
    return true;
}

bool WriteSubLayerHrdParameters( h265nal::H265SubLayerHrdParametersParser::SubLayerHrdParametersState* sub_layer_hrd_parameters,
                                h265nal::BitBufferWriter* bit_buffer, uint32_t CpbCnt,
                                uint32_t sub_pic_hrd_params_present_flag) noexcept {
    
    // H265 sub_layer_hrd_parameters NAL Unit.
    // Section E.2.3 ("Sub-layer HRD parameters syntax") of the H.265
    // standard for a complete description.
    
    for (uint32_t i = 0; i < CpbCnt; i++) {
        // bit_rate_value_minus1[i]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(sub_layer_hrd_parameters->bit_rate_value_minus1[i])) {
            return false;
        }
        
        // cpb_size_value_minus1[i]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(sub_layer_hrd_parameters->cpb_size_value_minus1[i])) {
            return false;
        }
        
        if (sub_pic_hrd_params_present_flag) {
            // cpb_size_du_value_minus1[i]  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(sub_layer_hrd_parameters->cpb_size_du_value_minus1[i])) {
                return false;
            }
            
            // bit_rate_du_value_minus1[i]  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(sub_layer_hrd_parameters->bit_rate_du_value_minus1[i])) {
                return false;
            }
        //} else {
        //    sub_layer_hrd_parameters->cpb_size_du_value_minus1.push_back(0);
        //    sub_layer_hrd_parameters->bit_rate_du_value_minus1.push_back(0);
        }
        
        // cbr_flag[i]  u(1)
        if (!bit_buffer->WriteBits(sub_layer_hrd_parameters->cbr_flag[i], 1)) {
            return false;
        }
    }
    
    return true;
}

bool WriteHrdParameters(h265nal::H265HrdParametersParser::HrdParametersState* hrd_parameters,
                        h265nal::BitBufferWriter* bit_buffer,
                        uint32_t commonInfPresentFlag, uint32_t maxNumSubLayersMinus1) noexcept {
    
    // H265 hrd_parameters NAL Unit.
    // Section E.2.2 ("HRD parameters syntax") of the H.265
    // standard for a complete description.
    
    if (commonInfPresentFlag) {
        // nal_hrd_parameters_present_flag  u(1)
        if (!bit_buffer->WriteBits(
                                   hrd_parameters->nal_hrd_parameters_present_flag, 1)) {
                                       return false;
                                   }
        
        // vcl_hrd_parameters_present_flag  u(1)
        if (!bit_buffer->WriteBits(
                                   hrd_parameters->vcl_hrd_parameters_present_flag, 1)) {
                                       return false;
                                   }
        
        if (hrd_parameters->nal_hrd_parameters_present_flag ||
            hrd_parameters->vcl_hrd_parameters_present_flag) {
            // sub_pic_hrd_params_present_flag  u(1)
            if (!bit_buffer->WriteBits(
                                       hrd_parameters->sub_pic_hrd_params_present_flag, 1)) {
                                           return false;
                                       }
            
            if (hrd_parameters->sub_pic_hrd_params_present_flag) {
                // tick_divisor_minus2  u(8)
                if (!bit_buffer->WriteBits(hrd_parameters->tick_divisor_minus2, 8)) {
                    return false;
                }
                
                // du_cpb_removal_delay_increment_length_minus1  u(5)
                if (!bit_buffer->WriteBits(
                                           hrd_parameters->du_cpb_removal_delay_increment_length_minus1, 5)) {
                                               return false;
                                           }
                
                // sub_pic_cpb_params_in_pic_timing_sei_flag  u(1)
                if (!bit_buffer->WriteBits(
                                           hrd_parameters->sub_pic_cpb_params_in_pic_timing_sei_flag, 1)) {
                                               return false;
                                           }
                
                // dpb_output_delay_du_length_minus1  u(5)
                if (!bit_buffer->WriteBits(
                                           hrd_parameters->dpb_output_delay_du_length_minus1, 5)) {
                                               return false;
                                           }
            }
            
            // bit_rate_scale  u(4)
            if (!bit_buffer->WriteBits(hrd_parameters->bit_rate_scale, 4)) {
                return false;
            }
            
            // cpb_size_scale  u(4)
            if (!bit_buffer->WriteBits(hrd_parameters->cpb_size_scale, 4)) {
                return false;
            }
            
            if (hrd_parameters->sub_pic_hrd_params_present_flag) {
                // cpb_size_du_scale  u(4)
                if (!bit_buffer->WriteBits(hrd_parameters->cpb_size_du_scale, 4)) {
                    return false;
                }
            }
            
            // initial_cpb_removal_delay_length_minus1  u(5)
            if (!bit_buffer->WriteBits(
                                       hrd_parameters->initial_cpb_removal_delay_length_minus1, 5)) {
                                           return false;
                                       }
            
            // au_cpb_removal_delay_length_minus1  u(5)
            if (!bit_buffer->WriteBits(
                                       hrd_parameters->au_cpb_removal_delay_length_minus1, 5)) {
                                           return false;
                                       }
            
            // dpb_output_delay_length_minus1  u(5)
            if (!bit_buffer->WriteBits(
                                       hrd_parameters->dpb_output_delay_length_minus1, 5)) {
                                           return false;
                                       }
        }
    }
    
    for (uint32_t i = 0; i <= maxNumSubLayersMinus1; i++) {
        // fixed_pic_rate_general_flag[i]  u(1)
        if (!bit_buffer->WriteBits(hrd_parameters->fixed_pic_rate_general_flag[i], 1)) {
            return false;
        }
        
        if (!hrd_parameters->fixed_pic_rate_general_flag[i]) {
            // fixed_pic_rate_within_cvs_flag[i]  u(1)
            if (!bit_buffer->WriteBits(hrd_parameters->fixed_pic_rate_within_cvs_flag[i], 1)) {
                return false;
            }
            // } else {
            //  hrd_parameters->fixed_pic_rate_within_cvs_flag.push_back(0);
        }
        
        if (hrd_parameters->fixed_pic_rate_within_cvs_flag[i]) {
            // elemental_duration_in_tc_minus1[i]  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(hrd_parameters->elemental_duration_in_tc_minus1[i])) {
                return false;
            }
            // // need to set low_delay_hrd_flag[i] to 0
            // hrd_parameters->low_delay_hrd_flag.push_back(0);
        } else {
            // need to set elemental_duration_in_tc_minus1[i] to 0
            // hrd_parameters->elemental_duration_in_tc_minus1.push_back(0);
            
            // low_delay_hrd_flag[i]  u(1)
            if (!bit_buffer->WriteBits(hrd_parameters->low_delay_hrd_flag[i], 1)) {
                return false;
            }
        }
        
        if (!hrd_parameters->low_delay_hrd_flag[i]) {
            // cpb_cnt_minus1[i]  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(hrd_parameters->cpb_cnt_minus1[i])) {
                return false;
            }
            //} else {
            //  // need to set cpb_cnt_minus1[i] to 0
            //  hrd_parameters->cpb_cnt_minus1.push_back(0);
        }
        
        if (hrd_parameters->nal_hrd_parameters_present_flag) {
            // sub_layer_hrd_parameters(i)
            auto CpbCnt = hrd_parameters->cpb_cnt_minus1[i] + 1;
            if(!WriteSubLayerHrdParameters(
                                           hrd_parameters->sub_layer_hrd_parameters_vector[i].get(),
                                           bit_buffer, CpbCnt,
                                           hrd_parameters->sub_pic_hrd_params_present_flag)) {
                                               return false;
                                           }
        }
        
        if (hrd_parameters->vcl_hrd_parameters_present_flag) {
            // sub_layer_hrd_parameters(i)
            auto CpbCnt = hrd_parameters->cpb_cnt_minus1[i] + 1;
            if(!WriteSubLayerHrdParameters(hrd_parameters->sub_layer_hrd_parameters_vector[i].get(),
                                           bit_buffer, CpbCnt,
                                           hrd_parameters->sub_pic_hrd_params_present_flag)) {
                return false;
            }
        }
    }
    return true;
}

bool WriteVps(h265nal::H265VpsParser::VpsState* vps, h265nal::BitBufferWriter* bit_buffer) noexcept
{
    // H265 VPS (video_parameter_set_rbsp()) NAL Unit.
    // Section 7.3.2.1 ("Video parameter set data syntax") of the H.265
    // standard for a complete description.
    
    // vps_video_parameter_set_id  u(4)
    if (!bit_buffer->WriteBits(vps->vps_video_parameter_set_id, 4)) {
        return false;
    }
    
    // vps_base_layer_internal_flag  u(1)
    if (!bit_buffer->WriteBits(vps->vps_base_layer_internal_flag, 1)) {
        return false;
    }
    
    // vps_base_layer_available_flag  u(1)
    if (!bit_buffer->WriteBits(vps->vps_base_layer_available_flag, 1)) {
        return false;
    }
    
    // vps_max_layers_minus1  u(6)
    if (!bit_buffer->WriteBits(vps->vps_max_layers_minus1, 6)) {
        return false;
    }
    
    // vps_max_sub_layers_minus1  u(3)
    if (!bit_buffer->WriteBits(vps->vps_max_sub_layers_minus1, 3)) {
        return false;
    }
    
    // vps_temporal_id_nesting_flag  u(1)
    if (!bit_buffer->WriteBits(vps->vps_temporal_id_nesting_flag, 1)) {
        return false;
    }
    
    // vps_reserved_0xffff_16bits  u(16)
    if (!bit_buffer->WriteBits(vps->vps_reserved_0xffff_16bits, 16)) {
        return false;
    }
    // profile_tier_level(1, vps_max_sub_layers_minus1)
    if(!WriteProfileTierLevel(vps->profile_tier_level.get(), bit_buffer)) {
        return false;
    }
    //return true; // TODO
    
    // vps_sub_layer_ordering_info_present_flag  u(1)
    if (!bit_buffer->WriteBits(vps->vps_sub_layer_ordering_info_present_flag, 1)) {
        return false;
    }
    
    for (uint32_t i = (vps->vps_sub_layer_ordering_info_present_flag
                       ? 0
                       : vps->vps_max_sub_layers_minus1);
         i <= vps->vps_max_sub_layers_minus1; i++) {
        // vps_max_dec_pic_buffering_minus1[i]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(vps->vps_max_dec_pic_buffering_minus1[vps->vps_sub_layer_ordering_info_present_flag ? i : i-1])) {
            return false;
        }
        // vps_max_num_reorder_pics[i]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(vps->vps_max_num_reorder_pics[vps->vps_sub_layer_ordering_info_present_flag ? i : i-1])) {
            return false;
        }
        // vps_max_latency_increase_plus1[i]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(vps->vps_max_latency_increase_plus1[vps->vps_sub_layer_ordering_info_present_flag ? i : i-1])) {
            return false;
        }
    }
    
    // vps_max_layer_id  u(6)
    if (!bit_buffer->WriteBits(vps->vps_max_layer_id, 6)) {
        return false;
    }
    
    // vps_num_layer_sets_minus1  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(vps->vps_num_layer_sets_minus1)) {
        return false;
    }
    
    for (uint32_t i = 1; i <= vps->vps_num_layer_sets_minus1; i++) {
        //vps->layer_id_included_flag.emplace_back();
        for (uint32_t j = 0; j <= vps->vps_max_layer_id; j++) {
#if REVERSE_THIS
            // layer_id_included_flag[i][j]  u(1)
            if (!bit_buffer->ReadBits(1, bits_tmp)) {
                return false;
            }
            vps->layer_id_included_flag[i - 1].push_back(bits_tmp);
#else
            // layer_id_included_flag[i][j]  u(1)
            if (!bit_buffer->WriteBits(vps->layer_id_included_flag[i - 1][j], 1)) {
                return false;
            }
#endif
        }
    }
    
    // vps_timing_info_present_flag  u(1)
    if (!bit_buffer->WriteBits(vps->vps_timing_info_present_flag, 1)) {
        return false;
    }
    
    if (vps->vps_timing_info_present_flag) {
        // vps_num_units_in_tick  u(32)
        if (!bit_buffer->WriteBits(vps->vps_num_units_in_tick, 32)) {
            return false;
        }
        
        // vps_time_scale  u(32)
        if (!bit_buffer->WriteBits(vps->vps_time_scale, 32)) {
            return false;
        }
        
        // vps_poc_proportional_to_timing_flag  u(1)
        if (!bit_buffer->WriteBits(vps->vps_poc_proportional_to_timing_flag, 1)) {
            return false;
        }
        
        if (vps->vps_poc_proportional_to_timing_flag) {
            // vps_num_ticks_poc_diff_one_minus1  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(vps->vps_num_ticks_poc_diff_one_minus1)) {
                return false;
            }
        }
        // vps_num_hrd_parameters  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(vps->vps_num_hrd_parameters)) {
            return false;
        }
        
        for (uint32_t i = 0; i < vps->vps_num_hrd_parameters; i++) {
            // hrd_layer_set_idx[i]  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(vps->hrd_layer_set_idx[i])) {
                return false;
            }
            
            if (i > 0) {
                // cprms_present_flag[i]  u(1)
                if (!bit_buffer->WriteExponentialGolomb(vps->cprms_present_flag[i])) {
                    return false;
                }
                // } else {
                //    vps->cprms_present_flag.push_back(0);
            }
            
            // hrd_parameters(cprms_present_flag[i], vps_max_sub_layers_minus1)
            if(!WriteHrdParameters(vps->hrd_parameters.get(), bit_buffer, vps->cprms_present_flag[i],
                                   vps->vps_max_sub_layers_minus1))
                return false;
        }
    }
    
    // vps_extension_flag  u(1)
    if (!bit_buffer->WriteBits(vps->vps_extension_flag, 1)) {
        return false;
    }
    
    //TODO!
    if (vps->vps_extension_flag) {
        while (more_rbsp_data(bit_buffer)) {
            // vps_extension_data_flag  u(1)
            if (!bit_buffer->WriteBits(vps->vps_extension_data_flag, 1)) {
                return false;
            }
        }
    }
    wbsp_trailing_bits(bit_buffer);
    return true;
}
