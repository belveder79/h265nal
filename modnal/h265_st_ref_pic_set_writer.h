#ifndef H265_ST_REF_PIC_SET_WRITER_INCLUDED
#define H265_ST_REF_PIC_SET_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteStRefPicSet(H265StRefPicSetParser::StRefPicSetState* st_ref_pic_set,
                      BitBufferWriter* bit_buffer) noexcept;

#endif // H265_ST_REF_PIC_SET_WRITER_INCLUDED
