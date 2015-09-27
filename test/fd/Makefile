
# project directory
ROOT = $(CURDIR)/../..
BUILD_ROOT = $(ROOT)/build


# CXX common settings and flags
CXX_FILE_SUFFIX = .cpp
CXX_HEADER_SUFFIX = .h
CXX = c++
CXXSTD = -std=c++1y


# preprocessor flags
CPPFLAGS = -U__STRICT_ANSI__


# compiler flags
IFLAGS = -iquote $(ROOT)/src/include -iquote $(ROOT)/src/fd/include
CXXFLAGS = $(CXXSTD) -Wall -Wpedantic -O2 -g -pipe $(IFLAGS)


# linker flags
LDFLAGS =

# static linking
# use LDSTATIC variable to specify static linking options, e.g.:
#   LDSTATIC = -static
# or
#   make LDSTATIC="-static -static-libgcc -static-libstdc++" <goal>
# or define the target-specific assignment for appropriate goals in local makefiles
#   <goal>: LDSTATIC = -static

# linking a shared library
# use makefile.generic/link.rules and define the target-specific assignment
# to CXXFLAGS, LDFLAGS, and LDOUT for appropriate goals in local makefiles, e.g.
#   <goal>: CXXFLAGS += -fPIC
#   <goal>: LDFLAGS += -shared
#   <goal>: LDOUT = lib$@.so
# alternatively, use makefile.generic/link-so.rules which basically do the above but leaving aside CXXFLAGS
# also when makefile.generic/link-so.rules is used
# 	if SONAME_VERSION is nonempty LDFLAGS is added with -Wl,-soname=$(notdir $(LDOUT)).$(SONAME_VERSION)
# 	if SOFILE_VERSION is nonempty linker output is $(LDOUT).$(SOFILE_VERSION)

# AR common settings and flags
AR = ar
ARFLAGS = rvus


# additional settings, sources, and prerequisites for specific targets
libpopen2 = $(BUILD_ROOT)/lib/libpopen2.a
fd3: private LDLIBS += $(libpopen2)


export


# no default goal
.DEFAULT_GOAL = $@


# pattern rules should contain recipes to not to be ignored by make
# as a user redifinition of the built-in rules to cancel them;
# therefore we duplicate the commands from the generic rule here
fd3: $(libpopen2)
	# use generic rules for compiling and linking
	$(MAKE) -f $(ROOT)/makefile.generic/compile.rules $@
	$(MAKE) -f $(ROOT)/makefile.generic/link.rules $@

# building the required library
$(libpopen2):
	$(MAKE) -C $(ROOT)/lib popen2


# use generic rules for 'clean'
clean:
	$(MAKE) -f $(ROOT)/makefile.generic/clean.rules $@
	# clean parts related to additional sources
	$(MAKE) -C $(ROOT)/lib $@


# use generic rules for all other targets
%:
	$(MAKE) -f $(ROOT)/makefile.generic/compile.rules $@
	$(MAKE) -f $(ROOT)/makefile.generic/link.rules $@

