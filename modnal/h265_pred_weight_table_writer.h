#ifndef H265_PRED_WEIGHT_TABLE_WRITER_INCLUDED
#define H265_PRED_WEIGHT_TABLE_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WritePredWeightTable(H265PredWeightTableParser::PredWeightTableState* pred_weight_table,
    BitBufferWriter* bit_buffer) noexcept;

#endif // H265_PRED_WEIGHT_TABLE_WRITER_INCLUDED
