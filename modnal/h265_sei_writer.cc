#include "h265_sei_writer.h"


bool WriteSeiUserDataRegisteredItuTT35( H265SeiUserDataRegisteredItuTT35Parser::H265SeiUserDataRegisteredItuTT35State* payload_state, BitBufferWriter* bit_buffer) {
    
    // H265 SEI user data ITU T-35 (user_data_registered_itu_t_t35()) parser.
    // Section D.2.6 ("User data registered by Recommendation ITU-T T.35
    // SEI message syntax") of the H.265 standard for a complete description.
    
    // itu_t_t35_country_code  b(8)
    if (!bit_buffer->WriteUInt8(payload_state->itu_t_t35_country_code)) {
        return false;
    }
    
    if (payload_state->itu_t_t35_country_code == 0xff) {
        // itu_t_t35_country_code_extension_byte  b(8)
        if (!bit_buffer->WriteUInt8(payload_state->itu_t_t35_country_code_extension_byte)) {
            return false;
        }
    }
    
    for (size_t i = 0; i < payload_state->payload.size(); ++i) {
        // itu_t_t35_payload_byte  b(8)
        if (!bit_buffer->WriteUInt8(payload_state->payload[i])) {
            return false;
        }
    }
    
    return true;
}

bool WriteSeiUserDataUnregistered(H265SeiUserDataUnregisteredParser::H265SeiUserDataUnregisteredState* payload_state, BitBufferWriter* bit_buffer)
{
    
    // H265 SEI user data unregistered (user_data_unregistered()) parser.
    // Section D.2.7 ("User data unregisterd SEI message syntax") of
    // the H.265 standard for a complete description.
#ifdef CHECK
    // uuid_iso_iec_11578  u(128)
    payload_state->uuid_iso_iec_11578_1 = 0;
    if (!bit_buffer->WriteBits(bits_tmp, 32)) {
        return false;
    }
    payload_state->uuid_iso_iec_11578_1 |= (uint64_t)bits_tmp << 32;
    if (!bit_buffer->WriteBits(bits_tmp, 32)) {
        return false;
    }
    payload_state->uuid_iso_iec_11578_1 |= (uint64_t)bits_tmp << 0;
    
    payload_state->uuid_iso_iec_11578_2 = 0;
    if (!bit_buffer->WriteBits(bits_tmp, 32)) {
        return false;
    }
    payload_state->uuid_iso_iec_11578_2 |= (uint64_t)bits_tmp << 32;
    if (!bit_buffer->WriteBits(bits_tmp, 32)) {
        return false;
    }
    payload_state->uuid_iso_iec_11578_2 |= (uint64_t)bits_tmp << 0;
#else
    uint32_t upper = (payload_state->uuid_iso_iec_11578_1 >> 32) & 0xFFFFFFFF;
    uint32_t lower = payload_state->uuid_iso_iec_11578_1 & 0xFFFFFFFF;
    
    // uuid_iso_iec_11578  u(128)
    payload_state->uuid_iso_iec_11578_1 = 0;
    if (!bit_buffer->WriteBits(upper, 32)) {
        return false;
    }
    if (!bit_buffer->WriteBits(lower, 32)) {
        return false;
    }
    
    upper = (payload_state->uuid_iso_iec_11578_1 >> 32) & 0xFFFFFFFF;
    lower = payload_state->uuid_iso_iec_11578_1 & 0xFFFFFFFF;
    
    if (!bit_buffer->WriteBits(upper, 32)) {
        return false;
    }
    if (!bit_buffer->WriteBits(lower, 32)) {
        return false;
    }
#endif
    for (size_t i = 0; i < payload_state->payload.size(); ++i) {
        // user_data_payload_byte  b(8)
        if (!bit_buffer->WriteUInt8(payload_state->payload[i])) {
            return false;
        }
    }
    return true;
}


bool WriteSeiAlphaChannelInfo(H265SeiAlphaChannelInfoParser::H265SeiAlphaChannelInfoState* payload_state, BitBufferWriter* bit_buffer) {
    // H265 SEI alpha channel info (alpha_channel_info()) parser.
    // Section F.14.2.8 ("Alpha channel information SEI message syntax") and
    //  F.14.3.8 ("Alpha channel information SEI message semantics") of
    // the H.265 standard for a complete description.
    
    // alpha_channel_cancel_flag  u(1)
    if (!bit_buffer->WriteBits(payload_state->alpha_channel_cancel_flag, 1)) {
        return false;
    }
    
    if (!payload_state->alpha_channel_cancel_flag) {
        // alpha_channel_use_idc  u(3)
        if (!bit_buffer->WriteBits(payload_state->alpha_channel_use_idc, 3)) {
            return false;
        }
        
        // alpha_channel_bit_depth_minus8  u(3)
        if (!bit_buffer->WriteBits(payload_state->alpha_channel_bit_depth_minus8, 3)) {
            return false;
        }
        
        // alpha_transparent_value  u(v)
        // The number of bits used for the representation of the
        // alpha_transparent_value syntax element is
        // alpha_channel_bit_depth_minus8 + 9
        if (!bit_buffer->WriteBits(payload_state->alpha_transparent_value, payload_state->alpha_channel_bit_depth_minus8 + 9)) {
            return false;
        }
        
        // alpha_opaque_value  u(v)
        // The number of bits used for the representation of the
        // alpha_opaque_value syntax element is alpha_channel_bit_depth_minus8 + 9
        if (!bit_buffer->WriteBits(payload_state->alpha_opaque_value, payload_state->alpha_channel_bit_depth_minus8 + 9)) {
            return false;
        }
        
        // alpha_channel_incr_flag  u(1)
        if (!bit_buffer->WriteBits(payload_state->alpha_channel_incr_flag, 1)) {
            return false;
        }
        
        // alpha_channel_clip_flag  u(1)
        if (!bit_buffer->WriteBits(payload_state->alpha_channel_clip_flag, 1)) {
            return false;
        }
        
        if (payload_state->alpha_channel_clip_flag) {
            // alpha_channel_clip_type_flag  u(1)
            if (!bit_buffer->WriteBits(payload_state->alpha_channel_clip_type_flag, 1)) {
                return false;
            }
        }
    }
    
    return true;
}


bool WriteSeiMasteringDisplayColourVolume(H265SeiMasteringDisplayColourVolumeParser::H265SeiMasteringDisplayColourVolumeState* payload_state, BitBufferWriter* bit_buffer)
{
    // H265 SEI mastering display colour volume
    // (mastering_display_colour_volume()) parser. Section D.2.28 ("Mastering
    // display colour volume SEI message syntax") of the H.265 standard for a
    // complete description.
    
    // display_primaries_x[c] and display_primaries_y[c]  u(16) each
    for (int c = 0; c < 3; c++) {
        if (!bit_buffer->WriteBits(payload_state->display_primaries_x[c], 16)) {
            return false;
        }
        
        if (!bit_buffer->WriteBits(payload_state->display_primaries_y[c], 16)) {
            return false;
        }
    }
    
    // white_point_x  u(16)
    if (!bit_buffer->WriteBits(payload_state->white_point_x, 16)) {
        return false;
    }
    
    // white_point_y  u(16)
    if (!bit_buffer->WriteBits(payload_state->white_point_y, 16)) {
        return false;
    }
    
    // max_display_mastering_luminance  u(32)
    if (!bit_buffer->WriteBits(payload_state->max_display_mastering_luminance, 32)) {
        return false;
    }
    
    // min_display_mastering_luminance  u(32)
    if (!bit_buffer->WriteBits(payload_state->min_display_mastering_luminance, 32)) {
        return false;
    }
    
    return true;
}

bool WriteSeiContentLightLevelInfo(H265SeiContentLightLevelInfoParser::H265SeiContentLightLevelInfoState* payload_state, BitBufferWriter* bit_buffer)
{
    // H265 SEI content light level info (content_light_level_info()) parser.
    // Section D.2.35 ("Content light level information SEI message syntax")
    // of the H.265 standard for a complete description.
    
    // max_content_light_level  u(16)
    if (!bit_buffer->WriteBits(payload_state->max_content_light_level, 16)) {
        return false;
    }
    
    // max_pic_average_light_level  u(16)
    if (!bit_buffer->WriteBits(payload_state->max_pic_average_light_level, 16)) {
        return false;
    }
    
    return true;
}

bool WriteSeiUnknownPayload(H265SeiUnknownParser::H265SeiUnknownState* payload_state,
                            BitBufferWriter* bit_buffer) {
    
    // We have no specific details for this sei, just keep all the bytes
    // in a payload buffer.
    for (size_t i = 0; i < payload_state->payload.size(); ++i) {
        if (!bit_buffer->WriteUInt8(payload_state->payload[i])) {
            return false;
        }
    }
    return true;
}


bool WriteSei(H265SeiMessageParser::SeiMessageState* sei_message_state, BitBufferWriter* bit_buffer) noexcept {
    // H265 SEI NAL Unit (access_unit_delimiter_rbsp()) parser.
    // Section 7.3.5 ("Supplemental enhancement information message syntax") of
    // the H.265 standard for a complete description.
    
    uint32_t payload_type = static_cast<uint32_t>(sei_message_state->payload_type);
    uint32_t payload_size = sei_message_state->payload_size;
    uint32_t ff_byte = 0xff;
    
    while(payload_type > 0xff)
    {
        if (!bit_buffer->WriteBits(ff_byte,8)) {
            return false;
        }
        payload_type -= 0xff;
    }
    if (!bit_buffer->WriteBits(payload_type,8)) {
        return false;
    }
    
    while(payload_size > 0xff)
    {
        if (!bit_buffer->WriteBits(ff_byte,8)) {
            return false;
        }
        payload_size -= 0xff;
    }
    if (!bit_buffer->WriteBits(payload_size,8)) {
        return false;
    }
    
    bool ok = true;
    payload_type = static_cast<uint32_t>(sei_message_state->payload_type);
    switch (static_cast<SeiType>(payload_type)) {
        case SeiType::user_data_registered_itu_t_t35:
            ok = WriteSeiUserDataRegisteredItuTT35((H265SeiUserDataRegisteredItuTT35Parser::H265SeiUserDataRegisteredItuTT35State*)sei_message_state->payload_state.get(), bit_buffer);
            break;
        case SeiType::user_data_unregistered:
            ok = WriteSeiUserDataUnregistered(
                                              (H265SeiUserDataUnregisteredParser::H265SeiUserDataUnregisteredState*)sei_message_state->payload_state.get(), bit_buffer);
            break;
        case SeiType::mastering_display_colour_volume:
            ok = WriteSeiMasteringDisplayColourVolume((H265SeiMasteringDisplayColourVolumeParser::H265SeiMasteringDisplayColourVolumeState*)sei_message_state->payload_state.get(), bit_buffer);
            break;
        case SeiType::content_light_level_info:
            ok = WriteSeiContentLightLevelInfo(
                                               (H265SeiContentLightLevelInfoParser::H265SeiContentLightLevelInfoState*)sei_message_state->payload_state.get(), bit_buffer);
            break;
        case SeiType::alpha_channel_info:
            ok = WriteSeiAlphaChannelInfo(
                                          (H265SeiAlphaChannelInfoParser::H265SeiAlphaChannelInfoState*)sei_message_state->payload_state.get(), bit_buffer);
            break;
        default:
            ok = WriteSeiUnknownPayload(
                                        (H265SeiUnknownParser::H265SeiUnknownState*)sei_message_state->payload_state.get(), bit_buffer);
            break;
    }
    return ok;
}

