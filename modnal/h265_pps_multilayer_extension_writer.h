#ifndef H265_PPS_MULTILAYER_EXTENSION_WRITER_INCLUDED
#define H265_PPS_MULTILAYER_EXTENSION_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WritePpsMultilayerExtension(H265PpsMultilayerExtensionParser::PpsMultilayerExtensionState* pps_multilayer_extension,
                                 BitBufferWriter* bit_buffer) noexcept;

#endif // H265_PPS_MULTILAYER_EXTENSION_WRITER_INCLUDED
