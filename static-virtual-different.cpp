/* glibc fastbin's/tcmalloc/jemalloc double free/destructor example
 *
 * This example uses a statically bound object which is double free'd
 * and a dynamically bound object which is allocated in a manner that
 * shares the same space as the statically bound one after the double
 * free condition occurs.
 *
 * This condition cannot be detected by vtable verification, because the
 * vtable is correct in the instance where one exists (type_two) and there
 * is no vftable to verify for type_one.
 *
 * The expected output in all conditions is:
 * - The printed output of the pointers has the same pointer for 'o' and 'h'
 * - The printed output of get_value() returns the correct outputs for each
 *   object type, 1 and 2 respectively.
 * - The printed output of get_state() is shared across the two instances and
 *   thus should output 3 and 3.
 *
 * Again, there is no really sane mechanism for detecting this state due to
 * the fact that one of the two objects is statically bound. I suppose it is
 * possible to emit code that matches statically bound call sites to objects
 * they're operating on, but again, we begin to approach a language closer
 * to java (of course, we being to approach vulnerabilities closer to javas 
 * as well).
 *
 * Currently, the most likely fix of substance is to detect the double free
 * condition. 
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
		virtual ~type_two(void) {}
		virtual std::size_t get_state(void) { return m_state; }
		virtual void set_state(std::size_t s) { m_state = s; }
		virtual std::size_t get_value(void) { return 2; }
};

#include <unistd.h>

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
	sleep(60*1);
	h->set_state(3);

	std::cout << "o: " << o << " t: " << t << " h: " << h << std::endl;
	std::cout << "o->get_value(): " << o->get_value() << std::endl << " h->get_value(): " << h->get_value() << std::endl;
	std::cout << "o->get_state(): " << o->get_state() << " h->get_state(): " << h->get_state() << std::endl;

	return EXIT_SUCCESS;

}
