#ifndef H265_AUD_WRITER_INCLUDED
#define H265_AUD_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteAud(H265AudParser::AudState* aud, BitBufferWriter* bit_buffer) noexcept;

#endif // H265_AUD_WRITER_INCLUDED
