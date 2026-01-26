#include "h265_vps_writer.h"
#include "h265_profile_tier_level_writer.h"
#include "h265_hrd_parameters_writer.h"
#include "h265_sub_layer_hrd_parameters_writer.h"

bool WriteVps(H265VpsParser::VpsState* vps, BitBufferWriter* bit_buffer) noexcept
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
            // layer_id_included_flag[i][j]  u(1)
            if (!bit_buffer->WriteBits(vps->layer_id_included_flag[i - 1][j], 1)) {
                return false;
            }
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
