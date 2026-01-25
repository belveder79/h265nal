#include "h265_nal_unit_writer.h"

bool WriteNalUnit(h265nal::H265NalUnitParser::NalUnitState* nal_unit, h265nal::BitBufferWriter* wbit_buffer) noexcept
{
    if(!WriteNalUnitHeader(nal_unit->nal_unit_header.get(), wbit_buffer))
        return false;
    if(!WriteNalUnitPayload(nal_unit->nal_unit_payload.get(), wbit_buffer,
                        nal_unit->nal_unit_header->nal_unit_type))
        return false;
    return true;
}

bool WriteNalUnitHeader(h265nal::H265NalUnitHeaderParser::NalUnitHeaderState* nal_unit_header, h265nal::BitBufferWriter* bit_buffer) noexcept
{
    // forbidden_zero_bit  f(1)
    if (!bit_buffer->WriteBits(nal_unit_header->forbidden_zero_bit, 1)) {
        return false;
    }
    
    // nal_unit_type  u(6)
    if (!bit_buffer->WriteBits(nal_unit_header->nal_unit_type, 6)) {
        return false;
    }
    
    // nuh_layer_id  u(6)
    if (!bit_buffer->WriteBits(nal_unit_header->nuh_layer_id, 6)) {
        return false;
    }
    
    // nuh_temporal_id_plus1  u(3)
    if (!bit_buffer->WriteBits(nal_unit_header->nuh_temporal_id_plus1, 3)) {
        return false;
    }
    return true;
}

bool WriteNalUnitPayload(h265nal::H265NalUnitPayloadParser::NalUnitPayloadState* state,
                         h265nal::BitBufferWriter* bit_buffer, uint32_t nal_unit_type) noexcept {
    // H265 NAL Unit Payload (nal_unit()) parser.
    // Section 7.3.1.1 ("General NAL unit header syntax") of the H.265
    // standard for a complete description.
    
    // payload (Table 7-1, Section 7.4.2.2)
    switch (nal_unit_type) {
        case h265nal::TRAIL_N:
        case h265nal::TRAIL_R:
        case h265nal::TSA_N:
        case h265nal::TSA_R:
        case h265nal::STSA_N:
        case h265nal::STSA_R:
        case h265nal::RADL_N:
        case h265nal::RADL_R:
        case h265nal::RASL_N:
        case h265nal::RASL_R: {
            // slice_segment_layer_rbsp()
            //C nal_unit_payload->slice_segment_layer =
            //C    H265SliceSegmentLayerParser::ParseSliceSegmentLayer(
            //C        bit_buffer, nal_unit_type, bitstream_parser_state);
            WriteSliceSegmentLayer(state->slice_segment_layer.get(), bit_buffer);
            break;
        }
        case h265nal::RSV_VCL_N10:
        case h265nal::RSV_VCL_R11:
        case h265nal::RSV_VCL_N12:
        case h265nal::RSV_VCL_R13:
        case h265nal::RSV_VCL_N14:
        case h265nal::RSV_VCL_R15:
            // reserved, non-IRAP, sub-layer non-reference pictures
            break;
        case h265nal::BLA_W_LP:
        case h265nal::BLA_W_RADL:
        case h265nal::BLA_N_LP:
        case h265nal::IDR_W_RADL:
        case h265nal::IDR_N_LP:
        case h265nal::CRA_NUT: {
            // slice_segment_layer_rbsp()
            //C nal_unit_payload->slice_segment_layer =
            //C     H265SliceSegmentLayerParser::ParseSliceSegmentLayer(
            //C       bit_buffer, nal_unit_type, bitstream_parser_state);
            WriteSliceSegmentLayer(state->slice_segment_layer.get(), bit_buffer);
            break;
        }
        case h265nal::RSV_IRAP_VCL22:
        case h265nal::RSV_IRAP_VCL23:
            // reserved, IRAP pictures
            break;
        case h265nal::RSV_VCL24:
        case h265nal::RSV_VCL25:
        case h265nal::RSV_VCL26:
        case h265nal::RSV_VCL27:
        case h265nal::RSV_VCL28:
        case h265nal::RSV_VCL29:
        case h265nal::RSV_VCL30:
        case h265nal::RSV_VCL31:
            // reserved, non-IRAP pictures
            break;
        case h265nal::VPS_NUT: {
            // video_parameter_set_rbsp()
            WriteVps(state->vps.get(), bit_buffer);
            break;
        }
        case h265nal::SPS_NUT: {
            // seq_parameter_set_rbsp()
            WriteSps(state->sps.get(), bit_buffer);
            break;
        }
        case h265nal::PPS_NUT: {
            // pic_parameter_set_rbsp()
            WritePps(state->pps.get(), bit_buffer);
            break;
        }
        case h265nal::AUD_NUT: {
            // access_unit_delimiter_rbsp()
            WriteAud(state->aud.get(), bit_buffer);
            break;
        }
        case h265nal::EOS_NUT:
            // end_of_seq_rbsp()
            // TODO(chemag): add support for end_of_seq(()
            break;
        case h265nal::EOB_NUT:
            // end_of_bitstream_rbsp()
            // TODO(chemag): add support for end_of_bitstream(()
            break;
        case h265nal::FD_NUT:
            // filler_data_rbsp()
            // TODO(chemag): add support for filler_data()
            break;
        case h265nal::PREFIX_SEI_NUT:
        case h265nal::SUFFIX_SEI_NUT:
            WriteSei(state->sei.get(), bit_buffer);
            break;
        case h265nal::RSV_NVCL41:
        case h265nal::RSV_NVCL42:
        case h265nal::RSV_NVCL43:
        case h265nal::RSV_NVCL44:
        case h265nal::RSV_NVCL45:
        case h265nal::RSV_NVCL46:
        case h265nal::RSV_NVCL47:
            // reserved
            break;
        default:
            // unspecified
            break;
    }
    
    return true;
}
