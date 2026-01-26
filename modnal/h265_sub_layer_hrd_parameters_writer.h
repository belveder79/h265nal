#ifndef H265_SUB_LAYER_HRD_PARAMETERS_WRITER_INCLUDED
#define H265_SUB_LAYER_HRD_PARAMETERS_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteSubLayerHrdParameters( H265SubLayerHrdParametersParser::SubLayerHrdParametersState* sub_layer_hrd_parameters,
                                BitBufferWriter* bit_buffer, uint32_t CpbCnt,
                                uint32_t sub_pic_hrd_params_present_flag) noexcept;

#endif // H265_SUB_LAYER_HRD_PARAMETERS_WRITER_INCLUDED
