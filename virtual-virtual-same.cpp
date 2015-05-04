#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

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
		virtual uint8_t* buf(void) { return m_buf; }
		virtual void set_buf(uint8_t b) { std::memset(m_buf, b, sizeof(m_buf)); return; }
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
	three->set_buf(0x43);

	std::cout << "one:   '" << one->buf() << "'" << std::endl;
	std::cout << "two:   '" << two->buf() << "'" << std::endl;
	std::cout << "three: '" << three->buf() << "'" << std::endl;

	return EXIT_SUCCESS;
}
