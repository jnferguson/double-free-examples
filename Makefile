CXX = g++
CXXFLAGS = -ggdb -std=c++11 -Wl,-z,relro,-z,now -ftrapv -fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIC -pie
WITH_VTV =  -fvtable-verify=preinit
GENERIC_ALLOC_FLAGS = -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
TCMALLOC_FLAGS = $(GENERIC_ALLOC_FLAGS) -ltcmalloc
JEMALLOC_FLAGS = $(GENERIC_ALLOC_FLAGS) -ljemalloc

default:
	@echo "There are multiple targets, each with a 'vtv' or 'novtv' option"
	@echo "Each compiles linking either to glibc, tcmalloc or jemalloc"
	@echo "Thus 'glibc-vtv' and 'glibc-novtv' compile with and without vtable verficiation"
	@echo "and link against glibc, 'tcmalloc-vtv' and 'tcmalloc-novtv' do as you would expect"
	@echo "as does 'jemalloc-vtv' and 'jemalloc-novtv'. They all expect that the libraries"
	@echo "are within your library path and installed, thus the lack of a configure script"

glibc-vtv: clean 
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o static-same static-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o static-different static-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o static-virtual-different static-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o virtual-glibc virtual-glibc.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o virtual-virtual-same virtual-virtual-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o virtual-virtual-different virtual-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o virtual-final-same virtual-final-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o virtual-staticdtor-different virtual-staticdtor-different.cpp 
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o virtual-staticdtor-same virtual-staticdtor-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) -o unmap-glibc unmap-glibc.cpp

glibc-novtv: clean 
	$(CXX) $(CXXFLAGS) -o static-same static-same.cpp
	$(CXX) $(CXXFLAGS) -o static-different static-different.cpp
	$(CXX) $(CXXFLAGS) -o static-virtual-different static-virtual-different.cpp
	$(CXX) $(CXXFLAGS) -o virtual-glibc virtual-glibc.cpp
	$(CXX) $(CXXFLAGS) -o virtual-virtual-same virtual-virtual-same.cpp
	$(CXX) $(CXXFLAGS) -o virtual-virtual-different virtual-virtual-different.cpp
	$(CXX) $(CXXFLAGS) -o virtual-final-same virtual-final-same.cpp	
	$(CXX) $(CXXFLAGS) -o virtual-staticdtor-different virtual-staticdtor-different.cpp
	$(CXX) $(CXXFLAGS) -o virtual-staticdtor-same virtual-staticdtor-same.cpp

tcmalloc-novtv: clean 
	$(CXX) $(CXXFLAGS) $(TCMALLOC_FLAGS) -o static-same static-same.cpp
	$(CXX) $(CXXFLAGS) $(TCMALLOC_FLAGS) -o static-different static-different.cpp
	$(CXX) $(CXXFLAGS) $(TCMALLOC_FLAGS) -o static-virtual-different static-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(TCMALLOC_FLAGS) -o virtual-virtual-same virtual-virtual-same.cpp
	$(CXX) $(CXXFLAGS) $(TCMALLOC_FLAGS) -o virtual-virtual-different virtual-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(TCMALLOC_FLAGS) -o virtual-final-same virtual-final-same.cpp
	$(CXX) $(CXXFLAGS) $(TCMALLOC_FLAGS) -o virtual-staticdtor-different virtual-staticdtor-different.cpp
	$(CXX) $(CXXFLAGS) $(TCMALLOC_FLAGS) -o virtual-staticdtor-same virtual-staticdtor-same.cpp

tcmalloc-vtv: clean
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(TCMALLOC_FLAGS) -o static-same static-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(TCMALLOC_FLAGS) -o static-different static-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(TCMALLOC_FLAGS) -o static-virtual-different static-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(TCMALLOC_FLAGS) -o virtual-virtual-same virtual-virtual-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(TCMALLOC_FLAGS) -o virtual-virtual-different virtual-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(TCMALLOC_FLAGS) -o virtual-final-same virtual-final-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(TCMALLOC_FLAGS) -o virtual-staticdtor-different virtual-staticdtor-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(TCMALLOC_FLAGS) -o virtual-staticdtor-same virtual-staticdtor-same.cpp

jemalloc-novtv: clean 
	$(CXX) $(CXXFLAGS) $(JEMALLOC_FLAGS) -o static-same static-same.cpp
	$(CXX) $(CXXFLAGS) $(JEMALLOC_FLAGS) -o static-different static-different.cpp
	$(CXX) $(CXXFLAGS) $(JEMALLOC_FLAGS) -o static-virtual-different static-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(JEMALLOC_FLAGS) -o virtual-virtual-same virtual-virtual-same.cpp
	$(CXX) $(CXXFLAGS) $(JEMALLOC_FLAGS) -o virtual-virtual-different virtual-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(JEMALLOC_FLAGS) -o virtual-final-same virtual-final-same.cpp
	$(CXX) $(CXXFLAGS) $(JEMALLOC_FLAGS) -o virtual-staticdtor-different virtual-staticdtor-different.cpp
	$(CXX) $(CXXFLAGS) $(JEMALLOC_FLAGS) -o virtual-staticdtor-same virtual-staticdtor-same.cpp

jemalloc-vtv: clean
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(JEMALLOC_FLAGS) -o static-same static-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(JEMALLOC_FLAGS) -o static-different static-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(JEMALLOC_FLAGS) -o static-virtual-different static-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(JEMALLOC_FLAGS) -o virtual-virtual-same virtual-virtual-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(JEMALLOC_FLAGS) -o virtual-virtual-different virtual-virtual-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(JEMALLOC_FLAGS) -o virtual-final-same virtual-final-same.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(JEMALLOC_FLAGS) -o virtual-staticdtor-different virtual-staticdtor-different.cpp
	$(CXX) $(CXXFLAGS) $(WITH_VTV) $(JEMALLOC_FLAGS) -o virtual-staticdtor-same virtual-staticdtor-same.cpp

clean:
	@rm -f static-same static-different static-virtual-different virtual-virtual-same virtual-glibc
	@rm -f virtual-virtual-different virtual-staticdtor-same virtual-staticdtor-different 
	@rm -f unmap-glibc virtual-final-same	
