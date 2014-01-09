#include "psd_native_ext.h"

VALUE psd_native_clipping_mask_apply_bang(VALUE self) {
  VALUE node = rb_iv_get(self, "@node");
  if (rb_funcall(node, rb_intern("clipped?"), 0) == Qfalse) {
    return Qnil;
  }

  psd_logger("debug", "Applying clipping mask with native code");

  VALUE canvas = rb_iv_get(self, "@canvas");
  VALUE* canvas_pixels = RARRAY_PTR(rb_funcall(rb_funcall(canvas, rb_intern("canvas"), 0), rb_intern("pixels"), 0));
  VALUE mask = rb_iv_get(self, "@mask");
  VALUE* mask_pixels = RARRAY_PTR(rb_funcall(rb_funcall(mask, rb_intern("canvas"), 0), rb_intern("pixels"), 0));

  uint32_t canvas_width = FIX2UINT(rb_funcall(canvas, rb_intern("width"), 0));
  uint32_t canvas_height = FIX2UINT(rb_funcall(canvas, rb_intern("height"), 0));
  int canvas_left = FIX2INT(rb_funcall(canvas, rb_intern("left"), 0));
  int canvas_top = FIX2INT(rb_funcall(canvas, rb_intern("top"), 0));

  uint32_t mask_width = FIX2UINT(rb_funcall(mask, rb_intern("width"), 0));
  uint32_t mask_height = FIX2UINT(rb_funcall(mask, rb_intern("height"), 0));
  int mask_left = FIX2UINT(rb_funcall(mask, rb_intern("left"), 0));
  int mask_top = FIX2UINT(rb_funcall(mask, rb_intern("top"), 0));

  int x, y, doc_x, doc_y, mask_x, mask_y;
  uint32_t alpha, color;
  VALUE pixel;
  for (y = 0; y < canvas_height; y++) {
    for (x = 0; x < canvas_width; x++) {
      doc_x = canvas_left + x;
      doc_y = canvas_top + y;

      mask_x = doc_x - mask_left;
      mask_y = doc_y - mask_top;

      if (mask_x < 0 || mask_x > mask_width || mask_y < 0 || mask_y > mask_height) {
        alpha = 0;
      } else {
        pixel = mask_pixels[mask_y * mask_width + mask_x];
        alpha = (pixel == Qnil ? 0 : A(FIX2UINT(pixel)));
      }

      color = FIX2UINT(canvas_pixels[y * canvas_width + x]);
      canvas_pixels[y * canvas_width + x] = INT2FIX((color & 0xffffff00) | (A(color) * alpha / 255));
    }
  }

  return Qnil;
}