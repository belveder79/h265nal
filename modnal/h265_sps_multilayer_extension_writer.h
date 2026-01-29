#ifndef H265_SPS_MULTILAYER_EXTENSION_WRITER_INCLUDED
#define H265_SPS_MULTILAYER_EXTENSION_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteSpsMultilayerExtension(const H265SpsMultilayerExtensionParser::SpsMultilayerExtensionState* sps_multilayer_extension,
                                 BitBufferWriter* bit_buffer) noexcept;

#endif // H265_SPS_MULTILAYER_EXTENSION_WRITER_INCLUDED
