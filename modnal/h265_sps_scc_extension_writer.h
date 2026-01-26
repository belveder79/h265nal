#ifndef H265_SPS_SCC_EXTENSION_WRITER_INCLUDED
#define H265_SPS_SCC_EXTENSION_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteSpsSccExtension(H265SpsSccExtensionParser::SpsSccExtensionState* sps_scc_extension, uint32_t chroma_format_idc,
                          uint32_t bit_depth_luma_minus8, uint32_t bit_depth_chroma_minus8, BitBufferWriter* bit_buffer) noexcept;

#endif // H265_SPS_SCC_EXTENSION_WRITER_INCLUDED
