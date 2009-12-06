/*
Copyright 2007-2009 Wincent Colaiuta
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <ruby.h>
#include <errno.h>
#include <unistd.h>

/*

call-seq:
    Dir.mkdtemp([string])   -> String or nil

This method securely creates temporary directories. It is a wrapper for the mkdtemp() function in the standard C library. It takes
an optional String parameter as a template describing the desired form of the directory name and overwriting the template in-place;
if no template is supplied then "/tmp/temp.XXXXXX" is used as a default.

Note that the exact implementation of mkdtemp() may vary depending on the target system. For example, on Mac OS X at the time of
writing, the man page states that the template may contain "some number" of "Xs" on the end of the string, whereas on Red Hat
Enterprise Linux it states that the template suffix "must be XXXXXX".

*/
static VALUE dir_mkdtemp_m(int argc, VALUE *argv, VALUE self)
{
    VALUE template;
    char *c_template;
    char *path;

    /* process argument */
    if (rb_scan_args(argc, argv, "01", &template) == 0) /* check for 0 mandatory arguments, 1 optional argument */
        template = Qnil;                                /* default to nil if no argument passed */
    if (NIL_P(template))
        template = rb_str_new2("/tmp/temp.XXXXXX");     /* fallback to this template if passed nil */
    SafeStringValue(template);                          /* raises if template is tainted and SAFE level > 0 */
    template = StringValue(template);                   /* duck typing support */

    /* create temporary storage */
    c_template = malloc(RSTRING_LEN(template) + 1);
    if (!c_template)
        rb_raise(rb_eNoMemError, "failed to allocate %d bytes of template storage", RSTRING_LEN(template) + 1);
    strncpy(c_template, RSTRING_PTR(template), RSTRING_LEN(template));
    c_template[RSTRING_LEN(template)] = 0;              /* NUL-terminate */

    /* fill out template */
    path = mkdtemp(c_template);
    if (path)
        template = rb_str_new2(path);
    free(c_template);
    if (path == NULL)
        rb_raise(rb_eSystemCallError, "mkdtemp failed (error #%d: %s)", errno, strerror(errno));
    return template;
}

void Init_mkdtemp()
{
    rb_define_module_function(rb_cDir, "mkdtemp", dir_mkdtemp_m, -1);
}
