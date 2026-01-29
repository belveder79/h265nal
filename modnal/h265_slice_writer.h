#ifndef H265_SLICE_WRITER_INCLUDED
#define H265_SLICE_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteSliceSegmentLayer(const H265SliceSegmentLayerParser::SliceSegmentLayerState* slice_segment_layer,
                            H265SpsParser::SpsState* sps, BitBufferWriter* bit_buffer) noexcept;

#endif // H265_SLICE_WRITER_INCLUDED
