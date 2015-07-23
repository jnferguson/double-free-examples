# double-free-examples
Basic code examples of conditions that still work and other nuances relating to double free's primarily in glibc, and to a lesser extent tcmalloc and jemalloc.

These primarily revolve around one very basic concept: many allocators "optimize" the double free security checks to only ensure that we are not deallocating the most recently deallocated chunk.
This leads to the behavior that if you can deallocate one chunk, then a different one, then the prior one a second time you can bypass the sanity checks. This pattern holds with
tcmalloc (chrome/et cetera), jemalloc (firefox) and glibc (linux).

Furthermore, the way of the world is moving towards things like vtable/vptr verification and control flow semantics to detect and prevent the corruption of 
function pointers which have pioneered the bypass to the stack cookie, and GCC now ships with vtable verification.

In some instances, we have some rather neat conditions where you can bypass vptr verification and have 2 objects occupy the same memory, a sort of schrodinger's object condition. 
There is at present no real method for preventing related attacks, beyond ensuring that double free conditions do not occur.
