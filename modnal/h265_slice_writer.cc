#include "h265_slice_writer.h"

bool WriteSliceSegmentLayer(h265nal::H265SliceSegmentLayerParser::SliceSegmentLayerState* slice_segment_layer,
                            h265nal::BitBufferWriter* bit_buffer) noexcept {
  // H265 slice segment layer (slice_segment_layer_rbsp()) NAL Unit.
  // Section 7.3.2.9 ("Slice segment layer RBSP syntax") of the H.265
  // standard for a complete description.
  // auto slice_segment_layer = std::make_unique<SliceSegmentLayerState>();
#pragma unused(slice_segment_layer)
#pragma unused(bit_buffer)
  // input parameters
  // slice_segment_layer->nal_unit_type = nal_unit_type;
#ifdef CHECK
  // slice_segment_header()
  slice_segment_layer->slice_segment_header =
      H265SliceSegmentHeaderParser::ParseSliceSegmentHeader(
          bit_buffer, nal_unit_type, bitstream_parser_state);
  if (slice_segment_layer->slice_segment_header == nullptr) {
    return false;
  }
#else
    std::cout << "Unimplemented" << std::endl;
#endif

  // slice_segment_data()
  // rbsp_slice_segment_trailing_bits()

  return true;
}
