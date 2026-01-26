#ifndef H265_HDR_PARAMETERS_WRITER_INCLUDED
#define H265_HDR_PARAMETERS_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteHrdParameters(H265HrdParametersParser::HrdParametersState* hrd_parameters,
                        BitBufferWriter* bit_buffer,
                        uint32_t commonInfPresentFlag, uint32_t maxNumSubLayersMinus1) noexcept;

#endif // H265_HDR_PARAMETERS_WRITER_INCLUDED
