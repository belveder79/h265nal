#ifndef H265_SLICE_WRITER_INCLUDED
#define H265_SLICE_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteSliceSegmentLayer(h265nal::H265SliceSegmentLayerParser::SliceSegmentLayerState* slice_segment_layer,
                            h265nal::BitBufferWriter* bit_buffer) noexcept;

#endif // H265_SLICE_WRITER_INCLUDED
