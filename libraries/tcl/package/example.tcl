# Authored by Charles A Daniels, 2019

# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>

# This file demonstrates how to call our C extension from samplepackage.c from
# TCL. You can run this file via `tclsh ./example.tcl`


# grab the current TCl stack frame
set frameinfo "[info frame [info frame]]"

# find the parent directory based on the frame info, this makes this script
# still work even if it's included, rather than executed directly
set parentdir "[file dirname [file normalize [dict get $frameinfo file]]]"

load "$parentdir[file separator]samplepackage.so"

# remember that linked_time is set by an alarm callback that fires once per
# second, so we have to give it time to run after loading the package
puts "sleeping for 2 seconds... "
after 2000

puts "the 14th Fibonacci number is: [samplepackage::fibo 14]"
puts "system time in seconds is: $samplepackage::linked_time"

# this lets us access things from the package without prefixing, this is very
# much like 'using namespace' in C++
namespace import ::samplepackage::*

puts "the 17th Fibonacci number is: [fibo 17]"
