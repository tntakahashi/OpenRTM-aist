## -*- Makefile -*-
##---------------------------------------------------------------------------
## Makefile.am for coil (Component Object Infrastructure Layer)
##
## $Id$
##---------------------------------------------------------------------------

DIST_SUBDIRS = coil bin build .

clean-local:
	rm -f *~
	rm -f *.sln *.ncb *.suo

EXTRA_DIST =               \
	coil                   \
	coil.sln.dep           \
	coil_config.vsprops    \
	coil_config.props      \
	coil_distbuild.vsprops \
	coil_distbuild.props   \
	coil_vc9.sln           \
	coil_vc10.sln          \
	coil_vc11.sln          \
	coil_vc12.sln          \
	coil_vc14.sln

#------------------------------------------------------------
# sln file build rules
#------------------------------------------------------------
win32_builddir = .

sln: coil_vc9.sln coil_vc10.sln coil_vc11.sln coil_vc12.sln coil_vc14.sln

vcproj:
	(cd coil ; make vcproj)


coil_vc9.sln: coil.sln.dep vcproj
	$(top_srcdir)/build/slntool.py     \
		--vcversion VC9            \
		--dep coil.sln.dep \
		--out coil_vc9.sln \
		`find ./ -name '*_vc9.vcproj'`

coil_vc10.sln: coil.sln.dep vcproj
	$(top_srcdir)/build/slntool.py     \
		--vcversion VC10            \
		--dep coil.sln.dep \
		--out coil_vc10.sln \
		`find ./ -name '*_vc10.vcxproj'`

coil_vc11.sln: coil.sln.dep vcproj
	$(top_srcdir)/build/slntool.py     \
		--vcversion VC11            \
		--dep coil.sln.dep \
		--out coil_vc11.sln \
		`find ./ -name '*_vc11.vcxproj'`

coil_vc12.sln: coil.sln.dep vcproj
	$(top_srcdir)/build/slntool.py     \
		--vcversion VC12            \
		--dep coil.sln.dep \
		--out coil_vc12.sln \
		`find ./ -name '*_vc12.vcxproj'`
	
coil_vc14.sln: coil.sln.dep vcproj
	$(top_srcdir)/build/slntool.py     \
		--vcversion VC14            \
		--dep coil.sln.dep \
		--out coil_vc14.sln \
		`find ./ -name '*_vc14.vcxproj'`

