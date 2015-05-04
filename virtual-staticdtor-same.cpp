/* glibc fastbin / tcmalloc / jemalloc double destructor/free example
 *
 * This example demonstrates a pattern with a base type with a protected
 * destructor so as to avoid glibc's corruption of the vftable pointer, 
 * that exact condition does not exhibit itself with jemalloc, however
 * there appears to be additional memory corruption in tcmalloc that 
 * leaves the heap in a less than stable state, however it was not 
 * further investigated.
 *
 * In this example, whether vtable verification is enabled or not is
 * irrelevant, as the same object type occupies the same memory location
 * and so all vptr's will correctly validate. However, the instance 
 * variables are shared and thus the objects become entangled with
 * one another and a modification to the state of one object modifies
 * the state of the other. As such, the unauthenticated regular user
 * becomes an authenticated administrative user when the instance
 * variables in one instance are changed.
 *
 */

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>

class user_base_type
{
	private:
	protected:
		bool	m_is_admin;
		bool	m_is_auth;

		~user_base_type(void) {}
	public:
		user_base_type(bool auth, bool admin) : m_is_auth(auth), m_is_admin(admin) {}
		virtual void set_auth(bool a) { m_is_auth = a; }
		virtual void set_admin(bool a) { m_is_admin = a; }
		virtual bool get_auth(void) { return m_is_auth; }
		virtual bool get_admin(void) { return m_is_admin; }
};

class user_type : public user_base_type
{
	private:
	protected:
	public:
		user_type(void) : user_base_type(false, false) {}
		~user_type(void) {}
};

signed int
main(void)
{
	user_type*		o(nullptr);
	user_type*		t(nullptr);
	user_type*		h(nullptr);

	o = new user_type;
	t = new user_type;

	delete o;
	delete t;
	delete o;

	o = new user_type;
	t = new user_type;
	h = new user_type;

	std::cout << "o: " << o << " t: " << t << " h: " << h << std::endl;
	
	o->set_auth(false);
	o->set_admin(false);
	h->set_auth(true);
	h->set_admin(true);

	std::cout << "o auth: " << o->get_auth() << " admin: " << o->get_admin() << std::endl;
	std::cout << "h auth: " << h->get_auth() << " admin: " << h->get_admin() << std::endl;

	return EXIT_SUCCESS;
}
