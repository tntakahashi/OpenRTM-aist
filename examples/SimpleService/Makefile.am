## -*- Makefile -*-
##---------------------------------------------------------------------------
## Makefile.am for MyServiceProvider/MyServiceConsumer components
##
## $Id$
##---------------------------------------------------------------------------

AUTOMAKE_OPTIONS = 1.9

IDLC = @IDLC@
IDLFLAGS = @IDL_FLAGS@
LIBS = @LIBS@

AM_CPPFLAGS=-I$(top_srcdir)/src/lib -I$(top_srcdir)/src/lib/coil/include -I$(top_srcdir)/src/lib/rtm/idl
AM_LDFLAGS=-L$(top_builddir) -L$(top_builddir)/src/lib/rtm -L$(top_builddir)/src/lib/rtm/idl

#------------------------------------------------------------
# Some rules for IDL compile
#------------------------------------------------------------
SUFFIXES = .o .so .idl Skel.cpp Stub.cpp Stub.o Skel.o

WRAPPER  = $(top_builddir)/utils/rtm-skelwrapper/rtm-skelwrapper
WRAPPER_FLAGS = --include-dir="" --skel-suffix=Skel --stub-suffix=Stub

idl-stamp: $(IDL_SOURCES)
	@touch idl-stamp
	@for f in $(IDL_SOURCES) ; do \
		$(IDLC) $(IDLFLAGS) $$f ; \
		PYTHONPATH="$(top_srcdir)/build:$(PYTHONPATH)" \
		PATH="$(top_srcdir)/utils/rtm-config:$(PATH)"  \
		$(WRAPPER) $(WRAPPER_FLAGS) --idl-file=$$f ; \
	done ;

$(ALL_SRC): idl-stamp
	@if test -f $@ ; then \
		touch $@ ; \
	else \
		rm -f idl-stamp ; \
		$(MAKE) $(AM_MAKEFLAGS) idl-stamp ; \
	fi ;

#------------------------------------------------------------
# Source list
#------------------------------------------------------------
IDL_SOURCES = MyService.idl

PROVIDER_DIST_SRC   = MyServiceSVC_impl.cpp MyServiceProvider.cpp
PROVIDER_NODIST_SRC = $(IDL_SOURCES:.idl=Skel.cpp)
PROVIDER_SRC        = $(PROVIDER_DIST_SRC) $(PROVIDER_NODIST_SRC)

PROVIDER_DIST_H     = $(PROVIDER_DIST_SRC:.cpp=.h)
PROVIDER_NODIST_H   = $(PROVIDER_NODIST_SRC:.cpp=.h)
PROVIDER_H          = $(PROVIDER_DIST_H) $(PROVIDER_NODIST_H)
PROVIDER_COMP_SRC   = $(PROVIDER_SRC) MyServiceProviderComp.cpp

CONSUMER_DIST_SRC   = MyServiceConsumer.cpp
CONSUMER_NODIST_SRC = $(IDL_SOURCES:.idl=Stub.cpp)
CONSUMER_SRC        = $(CONSUMER_DIST_SRC) $(CONSUMER_NODIST_SRC)

CONSUMER_DIST_H     = $(CONSUMER_DIST_SRC:.cpp=.h)
CONSUMER_NODIST_H   = $(CONSUMER_NODIST_SRC:.cpp=.h)
CONSUMER_H          = $(CONSUMER_DIST_H) $(CONSUMER_NODIST_H)
CONSUMER_COMP_SRC   = $(CONSUMER_SRC) MyServiceConsumerComp.cpp

#------------------------------------------------------------
# Object list
#------------------------------------------------------------
PROVIDER_OBJ=$(PROVIDER_DIST_SRC:.cpp=.o)
CONSUMER_OBJ=$(CONSUMER_DIST_SRC:.cpp=.o)

#------------------------------------------------------------
# Targets
#------------------------------------------------------------
# Targets directories
exampledir = $(rtm_exampledir)
socompdir  = $(exampledir)/rtc
sourcedir  = $(exampledir)/src/SimpleService

# Loadable RTCs
noinst_LTLIBRARIES = libMyServiceSkel.la libMyServiceStub.la 
nodist_libMyServiceSkel_la_SOURCES = $(PROVIDER_NODIST_SRC) $(PROVIDER_NODIST_H)
nodist_libMyServiceStub_la_SOURCES = $(CONSUMER_NODIST_SRC) $(CONSUMER_NODIST_H)

socomp_LTLIBRARIES = MyServiceProvider.la MyServiceConsumer.la

dist_MyServiceProvider_la_SOURCES = $(PROVIDER_DIST_SRC) $(PROVIDER_DIST_H) 
MyServiceProvider_la_LDFLAGS = -module
MyServiceProvider_la_DEPENDENCIES = libMyServiceSkel.la
MyServiceProvider_la_LIBADD  =                \
	libMyServiceSkel.la                   \
	$(top_builddir)/src/lib/rtm/libRTC.la \
	$(top_builddir)/src/lib/coil/lib/libcoil.la

dist_MyServiceConsumer_la_SOURCES = $(CONSUMER_DIST_SRC) $(CONSUMER_DIST_H)
MyServiceConsumer_la_LDFLAGS = -module
MyServiceConsumer_la_DEPENDENCIES = libMyServiceStub.la
MyServiceConsumer_la_LIBADD  =                \
	libMyServiceStub.la                   \
	$(top_builddir)/src/lib/rtm/libRTC.la \
	$(top_builddir)/src/lib/coil/lib/libcoil.la

# Standalone RTCs
example_PROGRAMS = MyServiceProviderComp MyServiceConsumerComp
MyServiceProviderComp_SOURCES = MyServiceProviderComp.cpp
MyServiceProviderComp_DEPENDENCIES = MyServiceProvider.la
MyServiceProviderComp_LDADD   =               \
	.libs/MyServiceProvider.a             \
	$(top_builddir)/src/lib/rtm/libRTC.la \
	$(top_builddir)/src/lib/coil/lib/libcoil.la

MyServiceConsumerComp_SOURCES = MyServiceConsumerComp.cpp
MyServiceConsumerComp_DEPENDENCIES = MyServiceConsumer.la
MyServiceConsumerComp_LDADD   =               \
	.libs/MyServiceConsumer.a             \
	$(top_builddir)/src/lib/rtm/libRTC.la \
	$(top_builddir)/src/lib/coil/lib/libcoil.la


# Example sources
source_DATA =                     \
	$(EXTRA_DIST)             \
	$(PROVIDER_DIST_H)        \
	$(PROVIDER_DIST_SRC)      \
	$(CONSUMER_DIST_H)        \
	$(CONSUMER_DIST_SRC)      \
	MyServiceProviderComp.cpp \
	MyServiceConsumerComp.cpp

#------------------------------------------------------------
# Extra distributed files
#------------------------------------------------------------
EXTRA_DIST =                       \
	$(IDL_SOURCES)             \
	Makefile.MyServiceProvider \
	Makefile.MyServiceConsumer \
	README.MyServiceProvider   \
	README.MyServiceConsumer   \
	gen.sh                     \
	rtc.conf

ALL_SRC =                    \
        $(PROVIDER_COMP_SRC) \
        $(CONSUMER_COMP_SRC)

#------------------------------------------------------------
# File list for deb/ports packages
#------------------------------------------------------------
lst:
	echo $(ALL_SRC) > src.lst
	echo $(EXTRA_DIST) > other.lst

#------------------------------------------------------------
# Visual Studio Project
#------------------------------------------------------------
win32_builddir=$(top_builddir)/win32/OpenRTM-aist/examples/SimpleService

vcproj: vc9proj vcxproj

vc9proj: 
	$(top_builddir)/build/vcprojtool.py vcproj                   \
		--type EXE                                           \
		--vcversion "9.00"                                   \
		--projectname "MyServiceProviderComp"                \
		--version $(RTM_VERSION)                             \
		--out $(win32_builddir)/MyServiceProvider_vc9.vcproj \
		--yaml ../rtc.vcproj.yaml                            \
		--source $(PROVIDER_COMP_SRC)                        \
		--header $(PROVIDER_H)
	$(top_builddir)/build/vcprojtool.py vcproj                      \
		--type DLL                                              \
		--vcversion "9.00"                                      \
		--projectname "MyServiceProvider"                       \
		--version $(RTM_VERSION)                                \
		--out $(win32_builddir)/MyServiceProviderDll_vc9.vcproj \
		--yaml ../rtcdll.vcproj.yaml                            \
		--source $(PROVIDER_SRC)                                \
		--header $(PROVIDER_H)
	$(top_builddir)/build/vcprojtool.py vcproj                   \
		--type EXE                                           \
		--vcversion "9.00"                                   \
		--projectname "MyServiceConsumerComp"                \
		--version $(RTM_VERSION)                             \
		--out $(win32_builddir)/MyServiceConsumer_vc9.vcproj \
		--yaml ../rtc.vcproj.yaml                            \
		--source $(CONSUMER_COMP_SRC)                        \
		--header $(CONSUMER_H)
	$(top_builddir)/build/vcprojtool.py vcproj                      \
		--type DLL                                              \
		--vcversion "9.00"                                      \
		--projectname "MyServiceConsumer"                       \
		--version $(RTM_VERSION)                                \
		--out $(win32_builddir)/MyServiceConsumerDll_vc9.vcproj \
		--yaml ../rtcdll.vcproj.yaml                            \
		--source $(CONSUMER_COMP_SRC)                           \
		--header $(CONSUMER_H)

vcxproj:
	for ver in $(VC_VERSION); do \
		$(top_builddir)/build/vcxprojtool.py vcxproj                   \
			--type EXE                                           \
			--vcversion "$${ver}.00"                                   \
			--projectname "MyServiceProviderComp"                \
			--version $(RTM_VERSION)                             \
			--out $(win32_builddir)/MyServiceProvider_vc$${ver}.vcxproj \
			--yaml ../rtc.vcproj.yaml                            \
			--source $(PROVIDER_COMP_SRC)                        \
			--header $(PROVIDER_H) ; \
		$(top_builddir)/build/vcxprojtool.py vcxproj                      \
			--type DLL                                              \
			--vcversion "$${ver}.00"                                      \
			--projectname "MyServiceProvider"                       \
			--version $(RTM_VERSION)                                \
			--out $(win32_builddir)/MyServiceProviderDll_vc$${ver}.vcxproj \
			--yaml ../rtcdll.vcproj.yaml                            \
			--source $(PROVIDER_SRC)                                \
			--header $(PROVIDER_H) ; \
		$(top_builddir)/build/vcxprojtool.py vcxproj                   \
			--type EXE                                           \
			--vcversion "$${ver}.00"                                   \
			--projectname "MyServiceConsumerComp"                \
			--version $(RTM_VERSION)                             \
			--out $(win32_builddir)/MyServiceConsumer_vc$${ver}.vcxproj \
			--yaml ../rtc.vcproj.yaml                            \
			--source $(CONSUMER_COMP_SRC)                        \
			--header $(CONSUMER_H) ; \
		$(top_builddir)/build/vcxprojtool.py vcxproj                      \
			--type DLL                                              \
			--vcversion "$${ver}.00"                                      \
			--projectname "MyServiceConsumer"                       \
			--version $(RTM_VERSION)                                \
			--out $(win32_builddir)/MyServiceConsumerDll_vc$${ver}.vcxproj \
			--yaml ../rtcdll.vcproj.yaml                            \
			--source $(CONSUMER_COMP_SRC)                           \
			--header $(CONSUMER_H) ; \
	done

dist-hook: lst vcproj


clean_objs:
	rm -f $(OBJS)

clean_skelstub:
	rm -f *Skel.h *Skel.cpp
	rm -f *Stub.h *Stub.cpp
	rm -f *.hh *SK.cc
	rm -f *C.cpp *C.h *S.cpp *S.h *_T.cpp *_T.h *.inl
	rm -f *-common.c *-decls.h *-skelimpl.c *-skels.c *-stubs.c
	rm -f MyService.h MyService.hh

clean-local: clean_objs clean_skelstub
	rm -f *.bak *.rpo *.sym lib*.*_pure_*
	rm -f *.lst *core *~ *loT *.log
	rm -f Makefile.old
	rm -f idl-stamp
