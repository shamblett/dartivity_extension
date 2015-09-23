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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ExtAll.o \
	${OBJECTDIR}/ExtPlatform.o \
	${OBJECTDIR}/ExtResource.o \
	${OBJECTDIR}/dartivity_extension.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../../../../../iot/iotivity/sources/git/out/linux/x86_64/debug -lSSMCore -lSSMSDK -lTGMSDKLibrary -lc_common -lcoap -lconnectivity_abstraction -loc -loc_logger -loc_logger_core -locpmapi -locsrm -loctbstack -lppm -lrcs_client -lrcs_common -lrcs_container -lrcs_server -ltinydtls -lNotificationManager -lTestBundle -lcpluff -lmosquitto -lpmimpl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdartivity_extension.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdartivity_extension.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdartivity_extension.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/ExtAll.o: ExtAll.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I../../../../../dart-sdk/ -I../../../../../../../iot/iotivity/sources/git/resource/include -I../../../../../../../iot/iotivity/sources/git/resource/csdk/stack/include -I../../../../../../../iot/iotivity/sources/git/resource/oc_logger/include -I../../../../../../../iot/iotivity/sources/git/resource/c_common -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ExtAll.o ExtAll.cpp

${OBJECTDIR}/ExtPlatform.o: ExtPlatform.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I../../../../../dart-sdk/ -I../../../../../../../iot/iotivity/sources/git/resource/include -I../../../../../../../iot/iotivity/sources/git/resource/csdk/stack/include -I../../../../../../../iot/iotivity/sources/git/resource/oc_logger/include -I../../../../../../../iot/iotivity/sources/git/resource/c_common -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ExtPlatform.o ExtPlatform.cpp

${OBJECTDIR}/ExtResource.o: ExtResource.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I../../../../../dart-sdk/ -I../../../../../../../iot/iotivity/sources/git/resource/include -I../../../../../../../iot/iotivity/sources/git/resource/csdk/stack/include -I../../../../../../../iot/iotivity/sources/git/resource/oc_logger/include -I../../../../../../../iot/iotivity/sources/git/resource/c_common -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ExtResource.o ExtResource.cpp

${OBJECTDIR}/dartivity_extension.o: dartivity_extension.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -I../../../../../dart-sdk/ -I../../../../../../../iot/iotivity/sources/git/resource/include -I../../../../../../../iot/iotivity/sources/git/resource/csdk/stack/include -I../../../../../../../iot/iotivity/sources/git/resource/oc_logger/include -I../../../../../../../iot/iotivity/sources/git/resource/c_common -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dartivity_extension.o dartivity_extension.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdartivity_extension.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
