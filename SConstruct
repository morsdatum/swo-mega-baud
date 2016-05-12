# Top SConstruct file
cm_tool = '/opt/bleeding-edge-arm-4_9/'
cm_prefix = cm_tool + 'bin/arm-none-eabi-'


msp_tool = '/opt/msp430-gcc-linux-3.5.0/'
msp_prefix = msp_tool + 'bin/msp430-elf-'

cm_dir = 'cortex'
msp_dir = 'msp430'

msp_cpu_flags = Split('-mmcu=msp430g2553')

opti_flags = Split('-O2 -g -std=gnu99')
cm_opti_flags = opti_flags + Split('-fomit-frame-pointer -fexpensive-optimizations')

cm_cpu_flags = Split('-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb')

cxx_flags = Split('''
		-x c++
		-fno-rtti
		''')
		
cm_cxx_flags = cxx_flags + Split('''
				-Wall -Wextra -Winline
				-Wcast-align -Wcast-qual -Wshadow
				-fno-exceptions 
				-ffunction-sections -fdata-sections
				-funsigned-bitfields -fshort-enums
				-pedantic -MD
				''')

cm_c_flags = Split('''
		-Wall -Wextra 
		-ffunction-sections -fdata-sections 
		-fno-common -MD 
		''') + Split('-I'+cm_tool+'arm-none-eabi/include')

cm_as_flags = Split('-x assembler-with-cpp -Wa,-ahlms=$(<:.s=.lst),--gstabs -MD -D_ASSEMBLER_ -mthumb')

cm_ld_flags = Split('''
		-Wall
		--static
		-nostartfiles
		-Wl,--gc-sections		
		''')+Split('-T'+cm_tool+'arm-none-eabi/share/libopencm3/board/stm32f4-discovery.ld')

#vars = Variables()
#vars.Add('TESTNUM', 'Set the test number to run', 1)
		
#cm_env = Environment(   variables=vars,
cm_env = Environment(
			#CPPDEFINES={'TESTCASE': '${TESTNUM}'},
			CCFLAGS=cm_c_flags+cm_cpu_flags+cm_opti_flags,
			CC=cm_prefix+'gcc',
			CXXFLAGS=cm_cxx_flags+cm_cpu_flags+cm_opti_flags,
			CXX=cm_prefix+'gcc',
			LINKFLAGS=cm_ld_flags+cm_cpu_flags,
			LINK=cm_prefix+'gcc',
			ASFLAGS=cm_as_flags+cm_cpu_flags+cm_opti_flags,
			AS=cm_prefix+'gcc',
			CPPPATH=Split(cm_tool+'/arm-none-eabi/include'),
			LIBPATH=Split(cm_tool+'arm-none-eabi/lib/opencm3 '+cm_tool+'arm-none-eabi/lib/armv7-m/fpu'),
			LIBS=Split('c gcc nosys opencm3_stm32f4')
			)

msp_env = Environment(
			CCFLAGS=msp_cpu_flags+opti_flags,
			CC=msp_prefix+'gcc',
			CXXFLAGS=cxx_flags+msp_cpu_flags+opti_flags,
			CXX=msp_prefix+'gcc',
			LINKFLAGS=msp_cpu_flags,
			LINK=msp_prefix+'gcc',
			CPPPATH=msp_tool+'/include',
			LIBPATH=msp_tool+'/include'
			)

scr_path = '/SConscript'


Export('cm_env')
Export('cm_dir')
Export('cm_prefix')
Export('msp_env')
Export('msp_dir')
Export('msp_prefix')

SConscript(msp_dir+scr_path)
SConscript(cm_dir+scr_path)
