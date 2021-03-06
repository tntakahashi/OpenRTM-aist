# -*- Makefile -*-
#------------------------------------------------------------
# @file   Makefile.am
# @brief  Makefile.am for CORBA_IORUtil unit test
# @date   $Date: 2007/01/21 06:51:41 $
# @author Noriaki Ando <n-ando@aist.go.jp>
#
# $Id: Makefile.am 1758 2010-01-22 14:04:54Z hakuta $
#
#------------------------------------------------------------

#
# $Log: Makefile.am,v $
# Revision 1.3  2007/01/21 06:51:41  n-ando
# The include search path option order in AM_CPPFLAGS was changed.
#
# Revision 1.2  2007/01/12 14:43:13  n-ando
# Some fixes for distribution control.
#
# Revision 1.1  2007/01/06 18:05:13  n-ando
# *** empty log message ***
#
#


AUTOMAKE_OPTIONS = 1.9

IDLC = @IDLC@
IDLFLAGS = @IDL_FLAGS@
LIBS = @LIBS@

AM_CPPFLAGS =                     \
	-coverage \
	 -I.                      \
	-I$(top_builddir)         \
	-I$(top_builddir)/src/lib/ \
	-I$(top_builddir)/src/lib/rtm \
	-I$(top_builddir)/src/lib/rtm/idl \
	-I$(top_builddir)/src/lib/coil/include \
	-I$(includedir)
#	-DORB_IS_OMNIORB

AM_LDFLAGS =              \
	-L.               \
	-L$(top_builddir) \
	-L$(top_builddir)/src/lib/coil/lib

IDL_SOURCES = \
	$(top_srcdir)/src/lib/rtm/idl/SDOPackage.idl    \
	$(top_srcdir)/src/lib/rtm/idl/RTC.idl           \
	$(top_srcdir)/src/lib/rtm/idl/BasicDataType.idl \
	$(top_srcdir)/src/lib/rtm/idl/OpenRTM.idl \
	$(top_srcdir)/src/lib/rtm/idl/Manager.idl \
	$(top_srcdir)/src/lib/rtm/idl/DataPort.idl

noinst_PROGRAMS = CORBA_IORUtilTests

nodist_CORBA_IORUtilTests_SOURCES =                 \
	../TestRunner.cpp                           \
	$(IDL_SOURCES:.idl=Skel.cpp)                \
	$(top_srcdir)/src/lib/rtm/CORBA_IORUtil.cpp \
	CORBA_IORUtilTests.cpp

CORBA_IORUtilTests_LDFLAGS = -L$(libdir) $(AM_LDFLAGS)
CORBA_IORUtilTests_LDADD   = -lcppunit -lcoil -luuid -lgcov -lrt

# Extra sources included distribution
EXTRA_DIST =               \
	CORBA_IORUtilTests.cpp


# all
all: do-test do-coverage

do-coverage: 
	rm -rf ./Coverage
	./CORBA_IORUtilTests 
	gcov CORBA_IORUtilTests.gcda 
	mkdir -p ./Coverage 
	lcov -c -d . -o ./Coverage/CORBA_IORUtilTests.info 
	../rtm_tests_changeInfo.py ./Coverage/CORBA_IORUtilTests.info 
	genhtml -o ./Coverage ./Coverage/CORBA_IORUtilTests.info 

# do tests
do-test:
	./CORBA_IORUtilTests --xml corba_iorutiltests.xml --xsl default.xsl

# clean-up
clean-local:
	rm -rf ./Coverage
	rm -f *.gcda *.gcno *.gcov
	rm -f CORBA_IORUtilTests
	rm -f                               \
        *.log *.xml                         \
	*.o *.Po *.gch *.la                 \
	*Skel.cpp *Skel.h *Stub.cpp *Stub.h \
	*~ *core                            \
	Makefile.old                        \
	*.cc *.hh *.i *.a *.c *.inl


