#ifndef H265_SCALING_LIST_DATA_WRITER_INCLUDED
#define H265_SCALING_LIST_DATA_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteScalingListData(const H265ScalingListDataParser::ScalingListDataState* scaling_list_data,
                          BitBufferWriter* bit_buffer) noexcept;

#endif // H265_SCALING_LIST_DATA_WRITER_INCLUDED
