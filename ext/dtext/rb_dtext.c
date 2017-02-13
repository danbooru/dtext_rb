#include "dtext.h"

#include <glib.h>
#include <ruby.h>
#include <ruby/encoding.h>

static VALUE mDTextRagel = Qnil;
static VALUE mDTextRagelError = Qnil;

static const char* each_node(const StateMachine* self, const size_t argc, const char* argv[]) {
  VALUE result = Qnil;
  VALUE block_args[argc];

  for (size_t i = 0; i < argc; i++) {
    block_args[i] = rb_enc_str_new_cstr(argv[i], rb_enc_find("utf-8"));
  }

  // XXX create an ensure block to do cleanup.
  result = rb_yield_values2(argc, block_args);

  if (NIL_P(result)) {
    return NULL;
  } else {
    result = rb_String(result);
    return rb_string_value_cstr(&result);
  }
}

static VALUE parse(int argc, VALUE * argv, VALUE self) {
  VALUE input;
  VALUE input0;
  VALUE options;
  VALUE opt_inline;
  VALUE opt_strip;
  VALUE opt_mentions;
  VALUE ret;
  rb_encoding * encoding = NULL;
  StateMachine * sm = NULL;
  bool f_strip = false;
  bool f_inline = false;
  bool f_mentions = true;

  if (argc == 0) {
    rb_raise(rb_eArgError, "wrong number of arguments (0 for 1)");
  }

  input = argv[0];

  if (NIL_P(input)) {
    return Qnil;
  }

  input0 = rb_str_dup(input);
  input0 = rb_str_cat(input0, "\0", 1);

  if (argc > 1) {
    options = argv[1];

    if (!NIL_P(options)) {
      opt_strip  = rb_hash_aref(options, ID2SYM(rb_intern("strip")));
      if (RTEST(opt_strip)) {
        f_strip = true;
      }

      opt_inline = rb_hash_aref(options, ID2SYM(rb_intern("inline")));
      if (RTEST(opt_inline)) {
        f_inline = true;
      }

      opt_mentions = rb_hash_aref(options, ID2SYM(rb_intern("disable_mentions")));
      if (RTEST(opt_mentions)) {
        f_mentions = false;
      }
    }
  }

  sm = init_machine(RSTRING_PTR(input0), RSTRING_LEN(input0), f_strip, f_inline, f_mentions);
  if (rb_block_given_p()) {
    sm->each_node = each_node;
  }

  if (!parse_helper(sm)) {
    GError* error = g_error_copy(sm->error);
    free_machine(sm);
    rb_raise(mDTextRagelError, error->message);
  }

  encoding = rb_enc_find("utf-8");
  ret = rb_enc_str_new(sm->output->str, sm->output->len, encoding);

  free_machine(sm);

  return ret;
}

void Init_dtext() {
  mDTextRagel = rb_define_module("DTextRagel");
  mDTextRagelError = rb_define_class_under(mDTextRagel, "Error", rb_eStandardError);
  rb_define_singleton_method(mDTextRagel, "parse", parse, -1);
}
