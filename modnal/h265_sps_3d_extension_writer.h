#ifndef H265_SPS_3D_EXTENSION_WRITER_INCLUDED
#define H265_SPS_3D_EXTENSION_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteSps3dExtension(H265Sps3dExtensionParser::Sps3dExtensionState* sps_3d_extension, BitBufferWriter* bit_buffer) noexcept ;

#endif // H265_SPS_3D_EXTENSION_WRITER_INCLUDED
