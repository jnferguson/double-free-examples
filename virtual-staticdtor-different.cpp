/* glibc fastbin / jemalloc / tcmalloc double destructor/free examples
 *
 * This example uses a base class that has a protected destructor ala
 * an interface or trait marking class. The two classes that inherit 
 * from it inherit the virtual functions. 
 *
 * The expected outcome of this is that in implementations with
 * vtable verification, then of course this is detected because
 * libvtv matches the call site to the particular object instance,
 * not just that the called object is a valid object, despite the 
 * conjectures of some academic papers on the subject.
 *
 * On implementations lacking this functionality, then the
 * unauthenticated regular user becomes an authenticated 
 * administrative user. The names and context are exemplification
 * for those lacking proper creativity to discern how such a
 * thing could be useful sans an execve() outcome.
 */
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>

class user_type
{
	private:
	protected:
		bool	m_is_admin;
		bool	m_is_auth;

		~user_type(void) {}
	public:
		user_type(bool auth, bool admin) : m_is_auth(auth), m_is_admin(admin) {}
		virtual void set_auth(bool a) { m_is_auth = a; }
		virtual void set_admin(bool a) { m_is_admin = a; }
		virtual bool get_auth(void) { return m_is_auth; }
		virtual bool get_admin(void) { return m_is_admin; }
};

class admin_type : public user_type
{
	private:
	protected:
	public:
		admin_type(void) : user_type(false, true) {}
		~admin_type(void);
};

class nonadmin_type : public user_type
{
	private:
	protected:
	public:
		nonadmin_type(void) : user_type(false, false) {}
		~nonadmin_type(void) {}
};

signed int
main(void)
{
	nonadmin_type*	o(nullptr);
	nonadmin_type*	t(nullptr);
	admin_type*		h(nullptr);

	o = new nonadmin_type;
	t = new nonadmin_type;

	delete o;
	delete t;
	delete o;

	o = new nonadmin_type;
	t = new nonadmin_type;
	h = new admin_type;

	std::cout << "o: " << o << " t: " << t << " h: " << h << std::endl;
	
	o->set_auth(false);
	o->set_admin(false);
	h->set_auth(true);
	h->set_auth(true);

	std::cout << "o auth: " << o->get_auth() << " admin: " << o->get_admin() << std::endl;
	std::cout << "h auth: " << h->get_auth() << " admin: " << h->get_auth() << std::endl;

	return EXIT_SUCCESS;
}
