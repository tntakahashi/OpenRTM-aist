# -*- Makefile -*-
#------------------------------------------------------------
# @file   Makefile.am
# @brief  Makefile.am for NVUtil unit test
# @date   $Date: 2007/01/21 06:52:23 $
# @author Noriaki Ando <n-ando@aist.go.jp>
#
# $Id$
#
#------------------------------------------------------------

AUTOMAKE_OPTIONS = 1.9

IDLC = @IDLC@
IDLFLAGS = @IDL_FLAGS@
LIBS = @LIBS@

AM_CPPFLAGS =                    \
	-coverage \
	 -I.                      \
	-I$(top_builddir)         \
	-I$(top_builddir)/src/lib \
	-I$(top_builddir)/src/lib/rtm/idl \
	-I$(top_builddir)/src/lib/coil/include \
	-I$(top_builddir)/src/lib/rtm \
	-I$(includedir)

AM_LDFLAGS =              \
	-L.               \
	-L$(top_builddir)


IDL_SOURCES =                                \
	$(top_srcdir)/src/lib/rtm/idl/RTC.idl        \
	$(top_srcdir)/src/lib/rtm/idl/SDOPackage.idl 

noinst_PROGRAMS = NVUtilTests

nodist_NVUtilTests_SOURCES =             \
	../TestRunner.cpp                \
	$(IDL_SOURCES:.idl=Skel.cpp)     \
	$(top_srcdir)/src/lib/coil/common/stringutil.cpp \
	$(top_srcdir)/src/lib/coil/common/Properties.cpp \
	$(top_srcdir)/src/lib/rtm/NVUtil.cpp     \
	NVUtilTests.cpp

NVUtilTests_LDFLAGS = -L$(libdir)
NVUtilTests_LDADD   = -lcppunit -lgcov

EXTRA_DIST =        \
	NVUtilTests.cpp

# all
all: do-test do-coverage

do-coverage: 
	rm -rf ./Coverage
	./NVUtilTests 
	gcov NVUtilTests.gcda 
	mkdir -p ./Coverage 
	lcov -c -d . -o ./Coverage/NVUtilTests.info 
	../rtm_tests_changeInfo.py ./Coverage/NVUtilTests.info 
	genhtml -o ./Coverage ./Coverage/NVUtilTests.info 

# do tests
do-test:
	./NVUtilTests --xml nvutiltests.xml --xsl default.xsl

# clean-up
clean-local:
	rm -rf ./Coverage
	rm -f *.gcda *.gcno *.gcov
	rm -f NVUtilTests
	rm -f                               \
        *.log *.xml                         \
	*.o *.Po *.gch *.la                 \
	*Skel.cpp *Skel.h *Stub.cpp *Stub.h \
	*~ *core                            \
	Makefile.old                        \
	*.cc *.hh *.i *.a *.c *.inl


