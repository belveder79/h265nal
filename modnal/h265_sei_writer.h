#ifndef H265_SEI_WRITER_INCLUDED
#define H265_SEI_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteSei(h265nal::H265SeiMessageParser::SeiMessageState* sei_message_state, h265nal::BitBufferWriter* bit_buffer) noexcept;

#endif // H265_SEI_WRITER_INCLUDED
