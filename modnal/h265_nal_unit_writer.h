#ifndef H265_NAL_UNIT_WRITER_INCLUDED
#define H265_NAL_UNIT_WRITER_INCLUDED

#include "h265_aud_writer.h"
#include "h265_pps_writer.h"
#include "h265_vps_writer.h"
#include "h265_sps_writer.h"
#include "h265_sei_writer.h"
#include "h265_slice_writer.h"
#include "h265_wutils.h"

bool WriteNalUnit(const H265NalUnitParser::NalUnitState* nal_unit, H265SpsParser::SpsState* sps, BitBufferWriter* bit_buffer) noexcept;

bool WriteNalUnitHeader(const H265NalUnitHeaderParser::NalUnitHeaderState* nal_unit_header, BitBufferWriter* bit_buffer) noexcept;


bool WriteNalUnitPayload(const H265NalUnitPayloadParser::NalUnitPayloadState* state, H265SpsParser::SpsState* sps,
                         BitBufferWriter* bit_buffer, uint32_t nal_unit_type) noexcept;

#endif // H265_NAL_UNIT_WRITER_INCLUDED
