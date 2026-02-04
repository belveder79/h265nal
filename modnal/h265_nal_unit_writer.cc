#include "h265_nal_unit_writer.h"

bool WriteNalUnit(const H265NalUnitParser::NalUnitState* nal_unit, H265SpsParser::SpsState* sps, BitBufferWriter* wbit_buffer) noexcept
{
    if(!WriteNalUnitHeader(nal_unit->nal_unit_header.get(), wbit_buffer))
        return false;
    if(!WriteNalUnitPayload(nal_unit->nal_unit_payload.get(), sps, wbit_buffer,
                        nal_unit->nal_unit_header->nal_unit_type))
        return false;
    
    if(nal_unit->trailing0x80Byte)
        if(!wbit_buffer->WriteUInt8(0x80))
            return false;
    
    return true;
}

bool WriteNalUnitHeader(const H265NalUnitHeaderParser::NalUnitHeaderState* nal_unit_header, BitBufferWriter* bit_buffer) noexcept
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

bool WriteNalUnitPayload(const H265NalUnitPayloadParser::NalUnitPayloadState* state, H265SpsParser::SpsState* sps,
                         BitBufferWriter* bit_buffer, uint32_t nal_unit_type) noexcept {
    // H265 NAL Unit Payload (nal_unit()) parser.
    // Section 7.3.1.1 ("General NAL unit header syntax") of the H.265
    // standard for a complete description.
    
    // payload (Table 7-1, Section 7.4.2.2)
    switch (nal_unit_type) {
        case TRAIL_N:
        case TRAIL_R:
        case TSA_N:
        case TSA_R:
        case STSA_N:
        case STSA_R:
        case RADL_N:
        case RADL_R:
        case RASL_N:
        case RASL_R: {
            // slice_segment_layer_rbsp()
            //C nal_unit_payload->slice_segment_layer =
            //C    H265SliceSegmentLayerParser::ParseSliceSegmentLayer(
            //C        bit_buffer, nal_unit_type, bitstream_parser_state);
            if( sps != nullptr)
                WriteSliceSegmentLayer(state->slice_segment_layer.get(), sps, bit_buffer);
            else
                return false;
            break;
        }
        case RSV_VCL_N10:
        case RSV_VCL_R11:
        case RSV_VCL_N12:
        case RSV_VCL_R13:
        case RSV_VCL_N14:
        case RSV_VCL_R15:
            // reserved, non-IRAP, sub-layer non-reference pictures
            break;
        case BLA_W_LP:
        case BLA_W_RADL:
        case BLA_N_LP:
        case IDR_W_RADL:
        case IDR_N_LP:
        case CRA_NUT: {
            // slice_segment_layer_rbsp()
            //C nal_unit_payload->slice_segment_layer =
            //C     H265SliceSegmentLayerParser::ParseSliceSegmentLayer(
            //C       bit_buffer, nal_unit_type, bitstream_parser_state);
            if( sps != nullptr)
                WriteSliceSegmentLayer(state->slice_segment_layer.get(), sps, bit_buffer);
            else
                return false;
            break;
        }
        case RSV_IRAP_VCL22:
        case RSV_IRAP_VCL23:
            // reserved, IRAP pictures
            break;
        case RSV_VCL24:
        case RSV_VCL25:
        case RSV_VCL26:
        case RSV_VCL27:
        case RSV_VCL28:
        case RSV_VCL29:
        case RSV_VCL30:
        case RSV_VCL31:
            // reserved, non-IRAP pictures
            break;
        case VPS_NUT: {
            // video_parameter_set_rbsp()
            WriteVps(state->vps.get(), bit_buffer);
            break;
        }
        case SPS_NUT: {
            // seq_parameter_set_rbsp()
            WriteSps(state->sps.get(), bit_buffer);
            break;
        }
        case PPS_NUT: {
            // pic_parameter_set_rbsp()
            WritePps(state->pps.get(), bit_buffer);
            break;
        }
        case AUD_NUT: {
            // access_unit_delimiter_rbsp()
            WriteAud(state->aud.get(), bit_buffer);
            break;
        }
        case EOS_NUT:
            // end_of_seq_rbsp()
            // TODO(chemag): add support for end_of_seq(()
            break;
        case EOB_NUT:
            // end_of_bitstream_rbsp()
            // TODO(chemag): add support for end_of_bitstream(()
            break;
        case FD_NUT:
            // filler_data_rbsp()
            // TODO(chemag): add support for filler_data()
            break;
        case PREFIX_SEI_NUT:
        case SUFFIX_SEI_NUT:
            WriteSei(state->sei.get(), bit_buffer);
            break;
        case RSV_NVCL41:
        case RSV_NVCL42:
        case RSV_NVCL43:
        case RSV_NVCL44:
        case RSV_NVCL45:
        case RSV_NVCL46:
        case RSV_NVCL47:
            // reserved
            break;
        default:
            // unspecified
            break;
    }
    
    return true;
}
