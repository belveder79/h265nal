#include "h265_sei_writer.h"


bool WriteSeiUserDataRegisteredItuTT35( H265SeiUserDataRegisteredItuTT35Parser::H265SeiUserDataRegisteredItuTT35State* payload_state, BitBufferWriter* bit_buffer) {
    
#pragma unused(payload_state)
#pragma unused(bit_buffer)
    std::cout << "Unimplemented!" << std::endl;
    // H265 SEI user data ITU T-35 (user_data_registered_itu_t_t35()) parser.
    // Section D.2.6 ("User data registered by Recommendation ITU-T T.35
    // SEI message syntax") of the H.265 standard for a complete description.
    
    /*
     uint32_t remaining_payload_size = payload_size;
     if (remaining_payload_size == 0) {
     return false;
     }
     auto payload_state =
     std::make_unique<H265SeiUserDataRegisteredItuTT35State>();
     
     // itu_t_t35_country_code  b(8)
     if (!bit_buffer->ReadUInt8(payload_state->itu_t_t35_country_code)) {
     return false;
     }
     remaining_payload_size--;
     
     if (payload_state->itu_t_t35_country_code == 0xff) {
     if (remaining_payload_size <= 0) {
     return false;
     }
     
     // itu_t_t35_country_code_extension_byte  b(8)
     if (!bit_buffer->ReadUInt8(
     payload_state->itu_t_t35_country_code_extension_byte)) {
     return false;
     }
     remaining_payload_size--;
     }
     
     payload_state->payload.resize(remaining_payload_size);
     for (size_t i = 0; i < payload_state->payload.size(); ++i) {
     // itu_t_t35_payload_byte  b(8)
     if (!bit_buffer->ReadUInt8(payload_state->payload[i])) {
     return false;
     }
     }
     */
    return true;
}

bool WriteSeiUserDataUnregistered(H265SeiUserDataUnregisteredParser::H265SeiUserDataUnregisteredState* payload_state, BitBufferWriter* bit_buffer)
{
#pragma unused(payload_state)
#pragma unused(bit_buffer)
    std::cout << "Unimplemented!" << std::endl;
    /*
     // H265 SEI user data unregistered (user_data_unregistered()) parser.
     // Section D.2.7 ("User data unregisterd SEI message syntax") of
     // the H.265 standard for a complete description.
     uint32_t remaining_payload_size = payload_size;
     if (remaining_payload_size < 16) {
     return false;
     }
     auto payload_state = std::make_unique<H265SeiUserDataUnregisteredState>();
     
     // uuid_iso_iec_11578  u(128)
     payload_state->uuid_iso_iec_11578_1 = 0;
     if (!bit_buffer->ReadBits(32, bits_tmp)) {
     return false;
     }
     payload_state->uuid_iso_iec_11578_1 |= (uint64_t)bits_tmp << 32;
     if (!bit_buffer->ReadBits(32, bits_tmp)) {
     return false;
     }
     payload_state->uuid_iso_iec_11578_1 |= (uint64_t)bits_tmp << 0;
     
     payload_state->uuid_iso_iec_11578_2 = 0;
     if (!bit_buffer->ReadBits(32, bits_tmp)) {
     return false;
     }
     payload_state->uuid_iso_iec_11578_2 |= (uint64_t)bits_tmp << 32;
     if (!bit_buffer->ReadBits(32, bits_tmp)) {
     return false;
     }
     payload_state->uuid_iso_iec_11578_2 |= (uint64_t)bits_tmp << 0;
     
     remaining_payload_size -= 16;
     
     payload_state->payload.resize(remaining_payload_size);
     for (size_t i = 0; i < payload_state->payload.size(); ++i) {
     // user_data_payload_byte  b(8)
     if (!bit_buffer->ReadUInt8(payload_state->payload[i])) {
     return false;
     }
     }
     */
    return true;
}


bool WriteSeiAlphaChannelInfo(H265SeiAlphaChannelInfoParser::H265SeiAlphaChannelInfoState* payload_state, BitBufferWriter* bit_buffer) {
    // H265 SEI alpha channel info (alpha_channel_info()) parser.
    // Section F.14.2.8 ("Alpha channel information SEI message syntax") and
    //  F.14.3.8 ("Alpha channel information SEI message semantics") of
    // the H.265 standard for a complete description.
    
#pragma unused(payload_state)
#pragma unused(bit_buffer)
    std::cout << "Unimplemented!" << std::endl;
    /*
     auto payload_state = std::make_unique<H265SeiAlphaChannelInfoState>();
     
     // alpha_channel_cancel_flag  u(1)
     if (!bit_buffer->ReadBits(1, payload_state->alpha_channel_cancel_flag)) {
     return false;
     }
     
     if (!payload_state->alpha_channel_cancel_flag) {
     // alpha_channel_use_idc  u(3)
     if (!bit_buffer->ReadBits(3, payload_state->alpha_channel_use_idc)) {
     return false;
     }
     
     // alpha_channel_bit_depth_minus8  u(3)
     if (!bit_buffer->ReadBits(3,
     payload_state->alpha_channel_bit_depth_minus8)) {
     return false;
     }
     
     // alpha_transparent_value  u(v)
     // The number of bits used for the representation of the
     // alpha_transparent_value syntax element is
     // alpha_channel_bit_depth_minus8 + 9
     if (!bit_buffer->ReadBits(payload_state->alpha_channel_bit_depth_minus8 + 9,
     payload_state->alpha_transparent_value)) {
     return false;
     }
     
     // alpha_opaque_value  u(v)
     // The number of bits used for the representation of the
     // alpha_opaque_value syntax element is alpha_channel_bit_depth_minus8 + 9
     if (!bit_buffer->ReadBits(payload_state->alpha_channel_bit_depth_minus8 + 9,
     payload_state->alpha_opaque_value)) {
     return false;
     }
     
     // alpha_channel_incr_flag  u(1)
     if (!bit_buffer->ReadBits(1, payload_state->alpha_channel_incr_flag)) {
     return false;
     }
     
     // alpha_channel_clip_flag  u(1)
     if (!bit_buffer->ReadBits(1, payload_state->alpha_channel_clip_flag)) {
     return false;
     }
     
     if (payload_state->alpha_channel_clip_flag) {
     // alpha_channel_clip_type_flag  u(1)
     if (!bit_buffer->ReadBits(1,
     payload_state->alpha_channel_clip_type_flag)) {
     return false;
     }
     }
     }
     
     */
    return true;
}


bool WriteSeiMasteringDisplayColourVolume(H265SeiMasteringDisplayColourVolumeParser::H265SeiMasteringDisplayColourVolumeState* payload_state, BitBufferWriter* bit_buffer)
{
#pragma unused(payload_state)
#pragma unused(bit_buffer)
    std::cout << "Unimplemented!" << std::endl;
    /*
     (void)payload_size;
     // H265 SEI mastering display colour volume
     // (mastering_display_colour_volume()) parser. Section D.2.28 ("Mastering
     // display colour volume SEI message syntax") of the H.265 standard for a
     // complete description.
     auto payload_state =
     std::make_unique<H265SeiMasteringDisplayColourVolumeState>();
     
     // display_primaries_x[c] and display_primaries_y[c]  u(16) each
     for (int c = 0; c < 3; c++) {
     uint32_t primaries_x, primaries_y;
     if (!bit_buffer->ReadBits(16, primaries_x)) {
     return false;
     }
     payload_state->display_primaries_x[c] = static_cast<uint16_t>(primaries_x);
     
     if (!bit_buffer->ReadBits(16, primaries_y)) {
     return false;
     }
     payload_state->display_primaries_y[c] = static_cast<uint16_t>(primaries_y);
     }
     
     // white_point_x  u(16)
     uint32_t white_x, white_y;
     if (!bit_buffer->ReadBits(16, white_x)) {
     return false;
     }
     payload_state->white_point_x = static_cast<uint16_t>(white_x);
     
     // white_point_y  u(16)
     if (!bit_buffer->ReadBits(16, white_y)) {
     return false;
     }
     payload_state->white_point_y = static_cast<uint16_t>(white_y);
     
     // max_display_mastering_luminance  u(32)
     if (!bit_buffer->ReadBits(32,
     payload_state->max_display_mastering_luminance)) {
     return false;
     }
     
     // min_display_mastering_luminance  u(32)
     if (!bit_buffer->ReadBits(32,
     payload_state->min_display_mastering_luminance)) {
     return false;
     }
     */
    return true;
}

bool WriteSeiContentLightLevelInfo(H265SeiContentLightLevelInfoParser::H265SeiContentLightLevelInfoState* payload_state, BitBufferWriter* bit_buffer)
{
    
#pragma unused(payload_state)
#pragma unused(bit_buffer)
    std::cout << "Unimplemented!" << std::endl;
    /*
     (void)payload_size;
     // H265 SEI content light level info (content_light_level_info()) parser.
     // Section D.2.35 ("Content light level information SEI message syntax")
     // of the H.265 standard for a complete description.
     auto payload_state = std::make_unique<H265SeiContentLightLevelInfoState>();
     
     // max_content_light_level  u(16)
     uint32_t max_content_light, max_pic_average;
     if (!bit_buffer->ReadBits(16, max_content_light)) {
     return false;
     }
     payload_state->max_content_light_level =
     static_cast<uint16_t>(max_content_light);
     
     // max_pic_average_light_level  u(16)
     if (!bit_buffer->ReadBits(16, max_pic_average)) {
     return false;
     }
     payload_state->max_pic_average_light_level =
     static_cast<uint16_t>(max_pic_average);
     
     */
    return true;
}

bool WriteSeiUnknownPayload(H265SeiUnknownParser::H265SeiUnknownState* state,
                            BitBufferWriter* bit_buffer) {
#pragma unused(state)
#pragma unused(bit_buffer)
    std::cout << "Unimplemented!" << std::endl;
    /*
     // We have no specific details for this sei, just keep all the bytes
     // in a payload buffer.
     uint32_t remaining_payload_size = payload_size;
     if (remaining_payload_size == 0) {
     return false;
     }
     auto payload_state = std::make_unique<H265SeiUnknownState>();
     payload_state->payload.resize(remaining_payload_size);
     for (size_t i = 0; i < payload_state->payload.size(); ++i) {
     if (!bit_buffer->ReadUInt8(payload_state->payload[i])) {
     return false;
     }
     }
     */
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

