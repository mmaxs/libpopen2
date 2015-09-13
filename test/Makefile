
# additional settings, sources, and prerequisites for specific targets
libpopen2 = $(BUILD_ROOT)/lib/libpopen2.a
hello% example%: private LDLIBS += $(libpopen2)


export


# no default goal
.DEFAULT_GOAL = .


# pattern rules should contain recipes to not to be ignored by make
# as a user redifinition of the built-in rules to cancel them;
# therefore we duplicate the commands from the generic rule here
hello% example%: $(libpopen2)
	# use generic rules for compiling and linking
	$(MAKE) -f $(ROOT)/makefile.generic/compile.rules $@
	$(MAKE) -f $(ROOT)/makefile.generic/link.rules $@

# building the required library
$(libpopen2):
	$(MAKE) -C $(ROOT) lib/popen2


# use generic rules for 'clean'
clean:
	$(MAKE) -f $(ROOT)/makefile.generic/clean.rules $@
	# clean parts related to additional sources
	$(MAKE) -C $(ROOT)/lib -f $(ROOT)/makefile.generic/clean.rules $@


# use generic rules for all other targets
%:
	$(MAKE) -f $(ROOT)/makefile.generic/compile.rules $@
	$(MAKE) -f $(ROOT)/makefile.generic/link.rules $@

