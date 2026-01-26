#include "h265_aud_writer.h"

bool WriteAud(H265AudParser::AudState* aud, BitBufferWriter* bit_buffer) noexcept {

  // pic_type  u(3)
  if (!bit_buffer->WriteBits(aud->pic_type, 3)) {
    return false;
  }

  wbsp_trailing_bits(bit_buffer);

  return true;
}
