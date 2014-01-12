#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/maestro/maestro.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/stewart-platform/configure.o \
	${OBJECTDIR}/stewart-platform/demo.o \
	${OBJECTDIR}/stewart-platform/matrix.o \
	${OBJECTDIR}/stewart-platform/optimise.o \
	${OBJECTDIR}/stewart-platform/parse.o \
	${OBJECTDIR}/stewart-platform/shapes.o \
	${OBJECTDIR}/stewart-platform/solve.o \
	${OBJECTDIR}/stewart-platform/vector.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lGL -lGLU -lglut

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/labyrinth

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/labyrinth: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/labyrinth ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/maestro/maestro.o: maestro/maestro.cpp 
	${MKDIR} -p ${OBJECTDIR}/maestro
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/maestro/maestro.o maestro/maestro.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/stewart-platform/configure.o: stewart-platform/configure.cpp 
	${MKDIR} -p ${OBJECTDIR}/stewart-platform
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stewart-platform/configure.o stewart-platform/configure.cpp

${OBJECTDIR}/stewart-platform/demo.o: stewart-platform/demo.cpp 
	${MKDIR} -p ${OBJECTDIR}/stewart-platform
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stewart-platform/demo.o stewart-platform/demo.cpp

${OBJECTDIR}/stewart-platform/matrix.o: stewart-platform/matrix.cpp 
	${MKDIR} -p ${OBJECTDIR}/stewart-platform
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stewart-platform/matrix.o stewart-platform/matrix.cpp

${OBJECTDIR}/stewart-platform/optimise.o: stewart-platform/optimise.cpp 
	${MKDIR} -p ${OBJECTDIR}/stewart-platform
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stewart-platform/optimise.o stewart-platform/optimise.cpp

${OBJECTDIR}/stewart-platform/parse.o: stewart-platform/parse.cpp 
	${MKDIR} -p ${OBJECTDIR}/stewart-platform
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stewart-platform/parse.o stewart-platform/parse.cpp

${OBJECTDIR}/stewart-platform/shapes.o: stewart-platform/shapes.cpp 
	${MKDIR} -p ${OBJECTDIR}/stewart-platform
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stewart-platform/shapes.o stewart-platform/shapes.cpp

${OBJECTDIR}/stewart-platform/solve.o: stewart-platform/solve.cpp 
	${MKDIR} -p ${OBJECTDIR}/stewart-platform
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stewart-platform/solve.o stewart-platform/solve.cpp

${OBJECTDIR}/stewart-platform/vector.o: stewart-platform/vector.cpp 
	${MKDIR} -p ${OBJECTDIR}/stewart-platform
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stewart-platform/vector.o stewart-platform/vector.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/labyrinth

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
