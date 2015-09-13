
# additional sources for specific targets
SOURCES += $(ROOT)/src/popen2.cpp
CXXFLAGS += -fPIC


export


# no default goal
.DEFAULT_GOAL = .


# use generic rules for 'clean'
clean:
	$(MAKE) -f $(ROOT)/makefile.generic/clean.rules $@
	# clean parts related to additional sources
	$(MAKE) -C $(ROOT)/src -f $(ROOT)/makefile.generic/clean.rules $@


# use generic rules for all other targets
%:
	$(MAKE) -f $(ROOT)/makefile.generic/compile.rules $@
	# make dynamic library
	$(MAKE) -f $(ROOT)/makefile.generic/link-so.rules $@
	# make static library
	$(MAKE) -f $(ROOT)/makefile.generic/ar.rules $@

