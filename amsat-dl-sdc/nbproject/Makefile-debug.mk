#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-debug.mk)" "nbproject/Makefile-local-debug.mk"
include nbproject/Makefile-local-debug.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=debug
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/driver/base/i2c.c src/driver/adf4351.c src/driver/i2c_native.c src/driver/lnb.c src/driver/usart.c src/driver/irq.c src/driver/si5328.c src/font8x8.c src/driver/lcd.c src/gps.c src/gpio.c src/main.c src/pinmap.c src/freq_tables.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/driver/base/i2c.p1 ${OBJECTDIR}/src/driver/adf4351.p1 ${OBJECTDIR}/src/driver/i2c_native.p1 ${OBJECTDIR}/src/driver/lnb.p1 ${OBJECTDIR}/src/driver/usart.p1 ${OBJECTDIR}/src/driver/irq.p1 ${OBJECTDIR}/src/driver/si5328.p1 ${OBJECTDIR}/src/font8x8.p1 ${OBJECTDIR}/src/driver/lcd.p1 ${OBJECTDIR}/src/gps.p1 ${OBJECTDIR}/src/gpio.p1 ${OBJECTDIR}/src/main.p1 ${OBJECTDIR}/src/pinmap.p1 ${OBJECTDIR}/src/freq_tables.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/src/driver/base/i2c.p1.d ${OBJECTDIR}/src/driver/adf4351.p1.d ${OBJECTDIR}/src/driver/i2c_native.p1.d ${OBJECTDIR}/src/driver/lnb.p1.d ${OBJECTDIR}/src/driver/usart.p1.d ${OBJECTDIR}/src/driver/irq.p1.d ${OBJECTDIR}/src/driver/si5328.p1.d ${OBJECTDIR}/src/font8x8.p1.d ${OBJECTDIR}/src/driver/lcd.p1.d ${OBJECTDIR}/src/gps.p1.d ${OBJECTDIR}/src/gpio.p1.d ${OBJECTDIR}/src/main.p1.d ${OBJECTDIR}/src/pinmap.p1.d ${OBJECTDIR}/src/freq_tables.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/driver/base/i2c.p1 ${OBJECTDIR}/src/driver/adf4351.p1 ${OBJECTDIR}/src/driver/i2c_native.p1 ${OBJECTDIR}/src/driver/lnb.p1 ${OBJECTDIR}/src/driver/usart.p1 ${OBJECTDIR}/src/driver/irq.p1 ${OBJECTDIR}/src/driver/si5328.p1 ${OBJECTDIR}/src/font8x8.p1 ${OBJECTDIR}/src/driver/lcd.p1 ${OBJECTDIR}/src/gps.p1 ${OBJECTDIR}/src/gpio.p1 ${OBJECTDIR}/src/main.p1 ${OBJECTDIR}/src/pinmap.p1 ${OBJECTDIR}/src/freq_tables.p1

# Source Files
SOURCEFILES=src/driver/base/i2c.c src/driver/adf4351.c src/driver/i2c_native.c src/driver/lnb.c src/driver/usart.c src/driver/irq.c src/driver/si5328.c src/font8x8.c src/driver/lcd.c src/gps.c src/gpio.c src/main.c src/pinmap.c src/freq_tables.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-debug.mk dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F2520
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/driver/base/i2c.p1: src/driver/base/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver/base" 
	@${RM} ${OBJECTDIR}/src/driver/base/i2c.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/base/i2c.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/base/i2c.p1 src/driver/base/i2c.c 
	@-${MV} ${OBJECTDIR}/src/driver/base/i2c.d ${OBJECTDIR}/src/driver/base/i2c.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/base/i2c.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/adf4351.p1: src/driver/adf4351.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/adf4351.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/adf4351.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/adf4351.p1 src/driver/adf4351.c 
	@-${MV} ${OBJECTDIR}/src/driver/adf4351.d ${OBJECTDIR}/src/driver/adf4351.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/adf4351.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/i2c_native.p1: src/driver/i2c_native.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/i2c_native.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/i2c_native.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/i2c_native.p1 src/driver/i2c_native.c 
	@-${MV} ${OBJECTDIR}/src/driver/i2c_native.d ${OBJECTDIR}/src/driver/i2c_native.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/i2c_native.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/lnb.p1: src/driver/lnb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/lnb.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/lnb.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/lnb.p1 src/driver/lnb.c 
	@-${MV} ${OBJECTDIR}/src/driver/lnb.d ${OBJECTDIR}/src/driver/lnb.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/lnb.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/usart.p1: src/driver/usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/usart.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/usart.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/usart.p1 src/driver/usart.c 
	@-${MV} ${OBJECTDIR}/src/driver/usart.d ${OBJECTDIR}/src/driver/usart.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/usart.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/irq.p1: src/driver/irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/irq.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/irq.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/irq.p1 src/driver/irq.c 
	@-${MV} ${OBJECTDIR}/src/driver/irq.d ${OBJECTDIR}/src/driver/irq.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/irq.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/si5328.p1: src/driver/si5328.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/si5328.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/si5328.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/si5328.p1 src/driver/si5328.c 
	@-${MV} ${OBJECTDIR}/src/driver/si5328.d ${OBJECTDIR}/src/driver/si5328.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/si5328.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/font8x8.p1: src/font8x8.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/font8x8.p1.d 
	@${RM} ${OBJECTDIR}/src/font8x8.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/font8x8.p1 src/font8x8.c 
	@-${MV} ${OBJECTDIR}/src/font8x8.d ${OBJECTDIR}/src/font8x8.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/font8x8.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/lcd.p1: src/driver/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/lcd.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/lcd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/lcd.p1 src/driver/lcd.c 
	@-${MV} ${OBJECTDIR}/src/driver/lcd.d ${OBJECTDIR}/src/driver/lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/gps.p1: src/gps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/gps.p1.d 
	@${RM} ${OBJECTDIR}/src/gps.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/gps.p1 src/gps.c 
	@-${MV} ${OBJECTDIR}/src/gps.d ${OBJECTDIR}/src/gps.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/gps.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/gpio.p1: src/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/gpio.p1.d 
	@${RM} ${OBJECTDIR}/src/gpio.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/gpio.p1 src/gpio.c 
	@-${MV} ${OBJECTDIR}/src/gpio.d ${OBJECTDIR}/src/gpio.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/gpio.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/pinmap.p1: src/pinmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/pinmap.p1.d 
	@${RM} ${OBJECTDIR}/src/pinmap.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/pinmap.p1 src/pinmap.c 
	@-${MV} ${OBJECTDIR}/src/pinmap.d ${OBJECTDIR}/src/pinmap.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/pinmap.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/freq_tables.p1: src/freq_tables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/freq_tables.p1.d 
	@${RM} ${OBJECTDIR}/src/freq_tables.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/freq_tables.p1 src/freq_tables.c 
	@-${MV} ${OBJECTDIR}/src/freq_tables.d ${OBJECTDIR}/src/freq_tables.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/freq_tables.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/driver/base/i2c.p1: src/driver/base/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver/base" 
	@${RM} ${OBJECTDIR}/src/driver/base/i2c.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/base/i2c.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/base/i2c.p1 src/driver/base/i2c.c 
	@-${MV} ${OBJECTDIR}/src/driver/base/i2c.d ${OBJECTDIR}/src/driver/base/i2c.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/base/i2c.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/adf4351.p1: src/driver/adf4351.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/adf4351.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/adf4351.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/adf4351.p1 src/driver/adf4351.c 
	@-${MV} ${OBJECTDIR}/src/driver/adf4351.d ${OBJECTDIR}/src/driver/adf4351.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/adf4351.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/i2c_native.p1: src/driver/i2c_native.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/i2c_native.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/i2c_native.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/i2c_native.p1 src/driver/i2c_native.c 
	@-${MV} ${OBJECTDIR}/src/driver/i2c_native.d ${OBJECTDIR}/src/driver/i2c_native.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/i2c_native.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/lnb.p1: src/driver/lnb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/lnb.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/lnb.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/lnb.p1 src/driver/lnb.c 
	@-${MV} ${OBJECTDIR}/src/driver/lnb.d ${OBJECTDIR}/src/driver/lnb.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/lnb.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/usart.p1: src/driver/usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/usart.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/usart.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/usart.p1 src/driver/usart.c 
	@-${MV} ${OBJECTDIR}/src/driver/usart.d ${OBJECTDIR}/src/driver/usart.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/usart.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/irq.p1: src/driver/irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/irq.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/irq.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/irq.p1 src/driver/irq.c 
	@-${MV} ${OBJECTDIR}/src/driver/irq.d ${OBJECTDIR}/src/driver/irq.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/irq.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/si5328.p1: src/driver/si5328.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/si5328.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/si5328.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/si5328.p1 src/driver/si5328.c 
	@-${MV} ${OBJECTDIR}/src/driver/si5328.d ${OBJECTDIR}/src/driver/si5328.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/si5328.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/font8x8.p1: src/font8x8.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/font8x8.p1.d 
	@${RM} ${OBJECTDIR}/src/font8x8.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/font8x8.p1 src/font8x8.c 
	@-${MV} ${OBJECTDIR}/src/font8x8.d ${OBJECTDIR}/src/font8x8.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/font8x8.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/driver/lcd.p1: src/driver/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/driver" 
	@${RM} ${OBJECTDIR}/src/driver/lcd.p1.d 
	@${RM} ${OBJECTDIR}/src/driver/lcd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/driver/lcd.p1 src/driver/lcd.c 
	@-${MV} ${OBJECTDIR}/src/driver/lcd.d ${OBJECTDIR}/src/driver/lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/driver/lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/gps.p1: src/gps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/gps.p1.d 
	@${RM} ${OBJECTDIR}/src/gps.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/gps.p1 src/gps.c 
	@-${MV} ${OBJECTDIR}/src/gps.d ${OBJECTDIR}/src/gps.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/gps.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/gpio.p1: src/gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/gpio.p1.d 
	@${RM} ${OBJECTDIR}/src/gpio.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/gpio.p1 src/gpio.c 
	@-${MV} ${OBJECTDIR}/src/gpio.d ${OBJECTDIR}/src/gpio.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/gpio.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/pinmap.p1: src/pinmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/pinmap.p1.d 
	@${RM} ${OBJECTDIR}/src/pinmap.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/pinmap.p1 src/pinmap.c 
	@-${MV} ${OBJECTDIR}/src/pinmap.d ${OBJECTDIR}/src/pinmap.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/pinmap.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/freq_tables.p1: src/freq_tables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/freq_tables.p1.d 
	@${RM} ${OBJECTDIR}/src/freq_tables.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -DXPRJ_debug=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/freq_tables.p1 src/freq_tables.c 
	@-${MV} ${OBJECTDIR}/src/freq_tables.d ${OBJECTDIR}/src/freq_tables.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/freq_tables.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_debug=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -mrom=default,-7dc0-7fff -mram=default,-5f4-5ff,-f9c-f9c,-fd4-fd4,-fdb-fdf,-fe3-fe7,-feb-fef,-ffd-fff  $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.map  -DXPRJ_debug=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"src" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/amsat-dl-sdc.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/debug
	${RM} -r dist/debug

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
