#include "h265_sps_range_extension_writer.h"

bool WriteSpsRangeExtension(const H265SpsRangeExtensionParser::SpsRangeExtensionState* sps_range_extension,
    BitBufferWriter* bit_buffer) noexcept {
  // H265 sps_range_extension() NAL Unit.
  // Section 7.3.2.2.2 ("Sequence parameter set range extension syntax") of
  // the H.265 standard for a complete description.

  // transform_skip_rotation_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(
          sps_range_extension->transform_skip_rotation_enabled_flag, 1)) {
    return false;
  }

  // transform_skip_context_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(
          sps_range_extension->transform_skip_context_enabled_flag, 1)) {
    return false;
  }

  // implicit_rdpcm_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(
                            sps_range_extension->implicit_rdpcm_enabled_flag, 1)) {
    return false;
  }

  // explicit_rdpcm_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(
                            sps_range_extension->explicit_rdpcm_enabled_flag, 1)) {
    return false;
  }

  // extended_precision_processing_flag  u(1)
  if (!bit_buffer->WriteBits(
          sps_range_extension->extended_precision_processing_flag, 1)) {
    return false;
  }

  // intra_smoothing_disabled_flag  u(1)
  if (!bit_buffer->WriteBits(
          sps_range_extension->intra_smoothing_disabled_flag, 1)) {
    return false;
  }

  // high_precision_offsets_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(
          sps_range_extension->high_precision_offsets_enabled_flag, 1)) {
    return false;
  }

  // persistent_rice_adaptation_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(
          sps_range_extension->persistent_rice_adaptation_enabled_flag, 1)) {
    return false;
  }

  // cabac_bypass_alignment_enabled_flag  u(1)
  if (!bit_buffer->WriteBits(
          sps_range_extension->cabac_bypass_alignment_enabled_flag, 1)) {
    return false;
  }

  return true;
}
