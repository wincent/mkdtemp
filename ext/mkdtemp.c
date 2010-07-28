// Copyright 2007-2010 Wincent Colaiuta. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <ruby.h>
#include <errno.h>
#include <unistd.h>
#include "ruby_compat.h"

// helper function needed by rb_iterate; see:
//  http://blade.nagaokaut.ac.jp/cgi-bin/scat.rb/ruby/ruby-talk/144100
VALUE call_chdir(VALUE dir)
{
    return rb_funcall(rb_cDir, rb_intern("chdir"), 1, dir);
}

// helper function needed by rb_iterate
VALUE yield_block(VALUE ignored, VALUE block)
{
    return rb_funcall(block, rb_intern("call"), 0);
}

// Document-method: mkdtemp
//
// call-seq:
//     Dir.mkdtemp([string]) -> String or nil
//     Dir.mkdtemp([string]) { ... } -> String or nil
//
// This method securely creates temporary directories. It is a wrapper for the
// mkdtemp() function in the standard C library. It takes an optional String
// parameter as a template describing the desired form of the directory name
// and overwriting the template in-place; if no template is supplied then
// "/tmp/temp.XXXXXX" is used as a default.
//
// If supplied a block, performs a Dir.chdir into the created directory and
// yields to the block:
//
//      # this:            # is a shorthand for:
//      Dir.mkdtemp do     #   dir = Dir.mkdtemp
//        puts Dir.pwd     #   Dir.chdir dir do
//      end                #     puts Dir.pwd
//                         #   end
//
// Note that the exact implementation of mkdtemp() may vary depending on the
// target system. For example, on Mac OS X at the time of writing, the man page
// states that the template may contain "some number" of "Xs" on the end of the
// string, whereas on Red Hat Enterprise Linux it states that the template
// suffix "must be XXXXXX".
static VALUE dir_mkdtemp_m(int argc, VALUE *argv, VALUE self)
{
    VALUE template, block;
    char *c_template;
    char *path;

    // process arguments
    if (rb_scan_args(argc, argv, "01&", &template, &block) == 0)    // 0 mandatory, 1 optional, 1 block
        template = Qnil;                                            // default to nil if no argument passed
    if (NIL_P(template))
        template = rb_str_new2("/tmp/temp.XXXXXX");                 // fallback to this template if passed nil
    SafeStringValue(template);                                      // raises if template is tainted and SAFE level > 0
    template = StringValue(template);                               // duck typing support

    // create temporary storage
    c_template = malloc(RSTRING_LEN(template) + 1);
    if (!c_template)
        rb_raise(rb_eNoMemError, "failed to allocate %ld bytes of template storage", RSTRING_LEN(template) + 1);
    strncpy(c_template, RSTRING_PTR(template), RSTRING_LEN(template));
    c_template[RSTRING_LEN(template)] = 0;              // NUL-terminate

    // fill out template
    path = mkdtemp(c_template);
    if (path)
        template = rb_str_new2(path);
    free(c_template);
    if (path == NULL)
        rb_raise(rb_eSystemCallError, "mkdtemp failed (error #%d: %s)", errno, strerror(errno));

    // yield to block if given, inside Dir.chdir
    if (rb_block_given_p() == Qtrue)
        rb_iterate(call_chdir, template, yield_block, block);
    return template;
}

void Init_mkdtemp()
{
#if 0
    // for Yardoc, need to fake this here
    VALUE rb_cDir = rb_define_class("Dir", rb_cObject);
#endif
    rb_define_singleton_method(rb_cDir, "mkdtemp", dir_mkdtemp_m, -1);
}
