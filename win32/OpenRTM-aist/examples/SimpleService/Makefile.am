## -*- Makefile -*-
##---------------------------------------------------------------------------
## Makefile.am for win32/OpenRTM-aist/examples/SimpleService
##
## $Id$
##---------------------------------------------------------------------------
clean-local:
	rm -rf Debug Release
	rm -rf *~ *.cpp *.h *.hh *.cc *.idl rtc.conf
	rm -rf *.vcproj *vcxproj *.user *.ncb *.suo
	rm -rf MyServiceProvider MyServiceProviderComp
	rm -rf MyServiceConsumer MyServiceConsumerComp

src:
	cp -p $(top_srcdir)/examples/SimpleService/*.cpp .
	cp -p $(top_srcdir)/examples/SimpleService/*.h .
	cp -p $(top_srcdir)/examples/SimpleService/*.idl .
	cp -p $(top_srcdir)/examples/SimpleService/rtc.conf .

dist-hook: src
