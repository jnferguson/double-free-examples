/* glibc fastbin's/tcmalloc/jemalloc double free/destructors example
 *
 * This is functionally the same at the static-static-same.cpp 
 * example, and just exemplifies that the final keyword causes
 * static binding.
 *
 */

#include <cstdint>
#include <cstdlib>
#include <iostream>

class type_one
{
	private:
		bool m_state;
	protected:
	public:
		type_one(void) : m_state(false) {}
		virtual ~type_one(void) final {}
		virtual void set_state(bool s) final { m_state = s; }
		virtual bool get_state(void) final { return m_state; }
};

signed int
main(void)
{
	type_one*	one(nullptr);
	type_one*	two(nullptr);
	type_one*	three(nullptr);

	one = new type_one;
	two	= new type_one;

	delete one;
	delete two;
	delete one;

	one 	= new type_one;
	two 	= new type_one;
	three	= new type_one;

	std::cout << "one: " << one << " two: " << two << " three: " << three << std::endl;
	one->set_state(false);
	three->set_state(true);

	std::cout << "one:   '" << one->get_state() << "'" << std::endl;
	std::cout << "two:   '" << two->get_state() << "'" << std::endl;
	std::cout << "three: '" << three->get_state() << "'" << std::endl;

	return EXIT_SUCCESS;
}
