#include "h265_profile_tier_level_writer.h"

bool WriteProfileInfo(H265ProfileInfoParser::ProfileInfoState* profile_info, BitBufferWriter* bit_buffer) noexcept
{
    uint32_t bits_tmp, bits_tmp_hi;
    
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

bool WriteProfileTierLevel(H265ProfileTierLevelParser::ProfileTierLevelState* profile_tier_level, BitBufferWriter* bit_buffer) noexcept
{
    // profile_tier_level() parser.
    // Section 7.3.3 ("Profile, tier and level syntax") of the H.265
    // standard for a complete description.
    
    if (profile_tier_level->profilePresentFlag) {
        WriteProfileInfo(profile_tier_level->general.get(), bit_buffer);
    }
    
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
