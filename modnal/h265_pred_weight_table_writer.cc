#include "h265_pred_weight_table_writer.h"

bool WritePredWeightTable(const H265PredWeightTableParser::PredWeightTableState* pred_weight_table,
    BitBufferWriter* bit_buffer) noexcept {

  // H265 pred_weight_table() NAL Unit.
  // Section 7.3.6.3 ("Weighted prediction parameters syntax") of the
  // H.265 standard for a complete description.

  // luma_log2_weight_denom  ue(v)
  if (!bit_buffer->WriteExponentialGolomb(
          pred_weight_table->luma_log2_weight_denom)) {
    return false;
  }

  if (pred_weight_table->ChromaArrayType != 0) {
    // delta_chroma_log2_weight_denom  se(v)
    if (!bit_buffer->WriteSignedExponentialGolomb(
            pred_weight_table->delta_chroma_log2_weight_denom)) {
      return false;
    }
  }

  for (uint32_t i = 0; i <= pred_weight_table->num_ref_idx_l0_active_minus1;
       ++i) {
    // from ffmpeg/libavcodec/cbs_h265_syntax_template.c
    // "1 (is not same POC and same layer_id)"
    // luma_weight_l0_flag[i]  u(1)
    if (!bit_buffer->WriteBits(pred_weight_table->luma_weight_l0_flag[i], 1)) {
      return false;
    }
  }

  if (pred_weight_table->ChromaArrayType != 0) {
    for (uint32_t i = 0; i <= pred_weight_table->num_ref_idx_l0_active_minus1;
         ++i) {
      // from ffmpeg/libavcodec/cbs_h265_syntax_template.c
      // "1 (is not same POC and same layer_id)"
      // chroma_weight_l0_flag[i]  u(1)
      if (!bit_buffer->WriteBits(pred_weight_table->chroma_weight_l0_flag[i], 1)) {
        return false;
      }
    }
  }

  for (uint32_t i = 0; i <= pred_weight_table->num_ref_idx_l0_active_minus1;
       ++i) {
    if (pred_weight_table->luma_weight_l0_flag[i]) {
      // delta_luma_weight_l0[i]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pred_weight_table->delta_luma_weight_l0[i])) {
        return false;
      }

      // luma_offset_l0[i]  se(v)
      if (!bit_buffer->WriteSignedExponentialGolomb(pred_weight_table->luma_offset_l0[i])) {
        return false;
      }

    }
    if (pred_weight_table->ChromaArrayType != 0) {
      if (pred_weight_table->chroma_weight_l0_flag[i]) {
        //pred_weight_table->delta_chroma_weight_l0.emplace_back();
        //pred_weight_table->delta_chroma_offset_l0.emplace_back();
        for (uint32_t j = 0; j < 2; ++j) {
          // delta_chroma_weight_l0[i][j]  se(v)
            
          if (!bit_buffer->WriteSignedExponentialGolomb(pred_weight_table->delta_chroma_weight_l0[i][j])) {
            return false;
          }
          // delta_chroma_offset_l0[i][j]  se(v)
          if (!bit_buffer->WriteSignedExponentialGolomb(pred_weight_table->delta_chroma_offset_l0[i][j])) {
            return false;
          }
        }
      }
    }
  }
  return true;
}
