bin_PROGRAMS = fadatool


# set the include path found by configure

fadatool_CXXFLAGS = $(AM_CXXFLAGS)
fadatool_CFLAGS = $(AM_CFLAGS)

INCLUDES = $(all_includes)

# the library search path.
fadatool_LDFLAGS = $(all_libraries) 
fadatool_SOURCES = ADaAn_Parser.cc agc.cpp assignment.cpp condition.cpp \
   contextquast.cpp controlstructure.cpp domaincondition.cpp edge.cpp \
   elementarydependence.cpp expression.cpp fada.cpp flattenquast.cpp global.cpp graph.cpp \
   graphprinter.cpp graphviz.cpp guardedreferences.cpp html.cpp index.cpp inequation.cpp \
   latex.cpp lc_rhs_term.cpp ldemonstrator.cpp lex.yy.c lexicographicinequation.cpp \
   logicalclause.cpp memorytracer.cpp options.cpp program.cpp quast.cpp read_reference.cpp \
   references.cpp solver.cpp statement.cpp written_reference.cpp

include_HEADERS = ADaAn_Parser.hh agc.h assignment.h condition.h constants.h \
   contextquast.h controlstructure.h domaincondition.h elementarydependence.h \
   expression.h fada.h flattenquast.h global.h graphviz.h guardedreferences.h html.h \
   index.h inequation.h latex.h lc_rhs_term.h ldemonstrator.h \
   lexicographicinequation.h logicalclause.h memorytracer.h options.h program.h quast.h \
   read_reference.h references.h solver.h statement.h written_reference.h


libfadalib_a_CXXFLAGS=$(AM_CXXFLAGS)
libfadalib_a_CFLAGS=$(AM_CFLAGS)
lib_LTLIBRARIES = libfada.la
libfada_la_SOURCES = ADaAn_Parser.cc agc.cpp assignment.cpp condition.cpp \
   contextquast.cpp controlstructure.cpp domaincondition.cpp edge.cpp \
   elementarydependence.cpp expression.cpp fada.cpp flattenquast.cpp global.cpp graph.cpp \
   graphprinter.cpp graphviz.cpp guardedreferences.cpp html.cpp index.cpp inequation.cpp \
   latex.cpp lc_rhs_term.cpp ldemonstrator.cpp lex.yy.c lexicographicinequation.cpp \
   logicalclause.cpp memorytracer.cpp options.cpp program.cpp quast.cpp read_reference.cpp \
   references.cpp solver.cpp statement.cpp written_reference.cpp



