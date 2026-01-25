#include "h265_sei_writer.h"



bool WriteSei(h265nal::H265SeiMessageParser::SeiMessageState* sei_message_state, h265nal::BitBufferWriter* bit_buffer) noexcept {
  // H265 SEI NAL Unit (access_unit_delimiter_rbsp()) parser.
  // Section 7.3.5 ("Supplemental enhancement information message syntax") of
  // the H.265 standard for a complete description.

#pragma unused(sei_message_state)
#pragma unused(bit_buffer)
    std::cout << "Unimplemented!" << std::endl;
/*
  uint32_t payload_type = 0;
  uint32_t payload_size = 0;
  uint32_t ff_byte = 0xff;
  auto sei_message_state = std::make_unique<SeiMessageState>();

  // ff_byte/last_payload_type_byte  f(8)
  ff_byte = 0xff;
  while (ff_byte == 0xff) {
    if (!bit_buffer->ReadBits(8, ff_byte)) {
      return false;
    }
    payload_type += ff_byte;
  }
  sei_message_state->payload_type = static_cast<SeiType>(payload_type);

  // ff_byte/last_payload_size_byte  f(8)
  ff_byte = 0xff;
  while (ff_byte == 0xff) {
    if (!bit_buffer->ReadBits(8, ff_byte)) {
      return false;
    }
    payload_size += ff_byte;
  }
  sei_message_state->payload_size = payload_size;

  // Section D.2.1: General SEI message syntax
  // TODO(chema): move dispatcher to a separate function
  // TODO(chema): enforce nal_unit_type check
  // sei_payload(payloadType, payloadSize)
  std::unique_ptr<H265SeiPayloadParser> payload_parser = nullptr;
  switch (static_cast<SeiType>(payload_type)) {
    case SeiType::user_data_registered_itu_t_t35:
      payload_parser =
          std::make_unique<H265SeiUserDataRegisteredItuTT35Parser>();
      break;
    case SeiType::user_data_unregistered:
      payload_parser = std::make_unique<H265SeiUserDataUnregisteredParser>();
      break;
    case SeiType::mastering_display_colour_volume:
      payload_parser =
          std::make_unique<H265SeiMasteringDisplayColourVolumeParser>();
      break;
    case SeiType::content_light_level_info:
      payload_parser = std::make_unique<H265SeiContentLightLevelInfoParser>();
      break;
    case SeiType::alpha_channel_info:
      payload_parser = std::make_unique<H265SeiAlphaChannelInfoParser>();
      break;
    default:
      payload_parser = std::make_unique<H265SeiUnknownParser>();
      break;
  }

  sei_message_state->payload_state =
      payload_parser->parse_payload(bit_buffer, payload_size);
 */
  return true;
}
/*
 std::unique_ptr<H265SeiPayloadParser::H265SeiPayloadState>
 H265SeiUserDataRegisteredItuTT35Parser::parse_payload(BitBuffer* bit_buffer,
 uint32_t payload_size) {
 // H265 SEI user data ITU T-35 (user_data_registered_itu_t_t35()) parser.
 // Section D.2.6 ("User data registered by Recommendation ITU-T T.35
 // SEI message syntax") of the H.265 standard for a complete description.
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
 return payload_state;
 }
 
 std::unique_ptr<H265SeiPayloadParser::H265SeiPayloadState>
 H265SeiUserDataUnregisteredParser::parse_payload(BitBuffer* bit_buffer,
 uint32_t payload_size) {
 uint32_t bits_tmp;
 
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
 return payload_state;
 }
 
 std::unique_ptr<H265SeiPayloadParser::H265SeiPayloadState>
 H265SeiAlphaChannelInfoParser::parse_payload(BitBuffer* bit_buffer,
 uint32_t payload_size) {
 (void)payload_size;
 // H265 SEI alpha channel info (alpha_channel_info()) parser.
 // Section F.14.2.8 ("Alpha channel information SEI message syntax") and
 //  F.14.3.8 ("Alpha channel information SEI message semantics") of
 // the H.265 standard for a complete description.
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
 
 return payload_state;
 }
 
 std::unique_ptr<H265SeiPayloadParser::H265SeiPayloadState>
 H265SeiMasteringDisplayColourVolumeParser::parse_payload(
 BitBuffer* bit_buffer, uint32_t payload_size) {
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
 
 return payload_state;
 }
 
 std::unique_ptr<H265SeiPayloadParser::H265SeiPayloadState>
 H265SeiContentLightLevelInfoParser::parse_payload(BitBuffer* bit_buffer,
 uint32_t payload_size) {
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
 
 return payload_state;
 }
 
 std::unique_ptr<H265SeiPayloadParser::H265SeiPayloadState>
 H265SeiUnknownParser::parse_payload(BitBuffer* bit_buffer,
 uint32_t payload_size) {
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
 return payload_state;
 }
 */
