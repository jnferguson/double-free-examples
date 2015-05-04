/*
 * glibc fastbin's and jemalloc/tcmalloc double destructor/free example
 *
 * This example is functionally the same as static-static-different.cpp
 * however in this instance the statically bound object that is allocated
 * to share the same memory as another object is the same.
 *
 * The effect is the same; the instance variables are shared across the
 * two instances, however the methods are called correctly. Moreover, 
 * again there is no virtual function table to verify, and thus no 
 * mechanism in place for mitigations like vtable verification to 
 * detect.
 *
 * Furthermore, because we are using a second instance of the same
 * type, there is really no mechanism for detecting that this condition
 * is occurring. Thus, again, the only real protection is to detect the
 * double free condition in the first place.
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
};

signed int
main(void)
{
	type_one*	o(nullptr);
	type_one*	t(nullptr);
	type_one*	h(nullptr);

	o = new type_one;
	t = new type_one;
	
	delete o;
	delete t;
	delete o;

	o = new type_one;
	t = new type_one;
	h = new type_one;

	o->set_state(1);
	t->set_state(2);
	h->set_state(3);

	std::cout << "o: " << o << " t: " << t << " h: " << h << std::endl;
	std::cout << "o: " << o->get_state() << " t: " << t->get_state() << " h: " << h->get_state() << std::endl;

	return EXIT_SUCCESS;

}
