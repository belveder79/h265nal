#include "h265_sps_multilayer_extension_writer.h"

bool WriteSpsMultilayerExtension(H265SpsMultilayerExtensionParser::SpsMultilayerExtensionState* sps_multilayer_extension,
    BitBufferWriter* bit_buffer) noexcept {
  // H265 sps_multilayer_extension() NAL Unit.
  // Section F.7.3.2.2.4 ("Sequence parameter set multilayer extension syntax")
  // of the H.265 standard for a complete description.

  // inter_view_mv_vert_constraint_flag  u(1)
  if (!bit_buffer->WriteBits(
          sps_multilayer_extension->inter_view_mv_vert_constraint_flag, 1)) {
    return false;
  }

  return true;
}
