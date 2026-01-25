#ifndef H265_NAL_UNIT_WRITER_INCLUDED
#define H265_NAL_UNIT_WRITER_INCLUDED

#include "h265_aud_writer.h"
#include "h265_pps_writer.h"
#include "h265_vps_writer.h"
#include "h265_sps_writer.h"
#include "h265_sei_writer.h"
#include "h265_slice_writer.h"
#include "h265_wutils.h"

bool WriteNalUnit(h265nal::H265NalUnitParser::NalUnitState* nal_unit, h265nal::BitBufferWriter* bit_buffer) noexcept;

bool WriteNalUnitHeader(h265nal::H265NalUnitHeaderParser::NalUnitHeaderState* nal_unit_header, h265nal::BitBufferWriter* bit_buffer) noexcept;


bool WriteNalUnitPayload(h265nal::H265NalUnitPayloadParser::NalUnitPayloadState* state,
                         h265nal::BitBufferWriter* bit_buffer, uint32_t nal_unit_type) noexcept;

#endif // H265_NAL_UNIT_WRITER_INCLUDED
