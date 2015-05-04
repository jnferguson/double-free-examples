/* glibc fastbin double free example; jemalloc/tcmalloc double free examples
 *
 * This example demonstrates a stable double free condition with statically 
 * bound objects, which as such do not have virtuable tables and thus their
 * call chains are not verified by gcc's VTV protections. 
 *
 * The expected condition for all allocators is that:
 * The double destructor condition will pass through undisturbted
 * The printed pointers will output the same pointer for 'o' and 'h'
 * The printed results of get value will be 1, 2
 * The printed results of the get_state() will be 3 and 3, instead of
 * the expected 1 and 3.
 *
 * Thus, we can conclude:
 * - no current exploit mitigations can detect and protect this-- the only
 *   available mechanism is to detect the double free. See 'notes.txt' for
 *   an explanation of why you do not have a glibc allocator patch.
 *  
 * - vtable verification would have detected this, but the objects are not
 *   dynamically bound, thus there are no vtables to verify.
 *
 * - When statically bound, the correct methods are called, however due to 
 *   the two instances sharing the same slot of memory, their instance variables
 *   are shared.
 *
 * Conclusion:
 * - Vtable verification is not enough, at some point, it will be necessary to 
 *   monitor the internals of class instances. However, then we have java and 
 *   some line of sanity should be drawn.
 *
 * - Double free's are a sub-set of use-after-free's and many (?most?) C++ 
 *   objects are small enough to hit the fastbin code paths, or such codepaths
 *   are non-existant in other allocators. This is probably not sufficient 
 *   over the long term and modifications likely should be made in the form
 *   of just adding a flag to check to see if the object is allocated or not.
 *   This will not fix all issues, but it will mitigate this particular subset.
 */

#include <cstdint>
#include <cstdlib>
#include <iostream>

class type_one
{
	private:
		std::size_t	m_state;

	protected:
	public:
		type_one(void) : m_state(0) {}
		type_one(std::size_t s) : m_state(s) {}
		~type_one(void) {}
		std::size_t get_state(void) { return m_state; }
		void set_state(std::size_t s) { m_state = s; }
		std::size_t get_value(void) { return 1; }
};

class type_two
{
	private:
		std::size_t m_state;
	protected:
	public:
		type_two(void) : m_state(0) {}
		type_two(std::size_t s) : m_state(s) {}
		~type_two(void) {}
		std::size_t get_state(void) { return m_state; }
		void set_state(std::size_t s) { m_state = s; }
		std::size_t get_value(void) { return 2; }
};

signed int
main(void)
{
	type_one*	o(nullptr);
	type_one*	t(nullptr);
	type_two*	h(nullptr);

	o = new type_one;
	t = new type_one;
	
	delete o;
	delete t;
	delete o;

	o = new type_one;
	t = new type_one;
	h = new type_two;

	o->set_state(1);
	h->set_state(3);

	std::cout << "o: " << o << " t: " << t << " h: " << h << std::endl;
	std::cout << "o->get_value(): " << o->get_value() << " h->get_value(): " << h->get_value() << std::endl;
	std::cout << "o->get_state(): " << o->get_state() << " h->get_state(): " << h->get_state() << std::endl;

	return EXIT_SUCCESS;

}
