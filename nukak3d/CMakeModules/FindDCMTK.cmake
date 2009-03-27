# - find DCMTK libraries
#

#  DCMTK_INCLUDE_DIR   - Directories to include to use DCMTK
#  DCMTK_LIBRARIES     - Files to link against to use DCMTK
#  DCMTK_FOUND         - If false, don't try to use DCMTK
#  DCMTK_DIR           - (optional) Source directory for DCMTK
#
# DCMTK_DIR can be used to make it simpler to find the various include
# directories and compiled libraries if you've just compiled it in the
# source tree. Just set it to the root of the tree where you extracted
# the source.
#
# Written for VXL by Amitha Perera.
# Modified by Alexander Pinzon Fernandez
# 

SET(DCMTK_PROGRAM_PATHS ${DCMTK_DIR}
	${DCMTK_DIR}/bin
	${DCMTK_DIR}/../install/bin
	${DCMTK_DIR}/../dcmtk-3.5.4-linux-i686
	${DCMTK_DIR}/../dcmtk-3.5.4-linux-i686/bin
	${DCMTK_DIR}/../dcmtk-3.5.4-solaris9-sparc
	${DCMTK_DIR}/../dcmtk-3.5.4-solaris9-sparc/bin
	${DCMTK_DIR}/../dcmtk-3.5.4-win32-i386
	${DCMTK_DIR}/../dcmtk-3.5.4-win32-i386/bin 
	/usr/bin
	/usr/local/bin
	/opt/local/bin
)

FIND_PROGRAM(DCMTK_dump2dcm_PROGRAM
	NAMES dump2dcm
	PATHS ${DCMTK_PROGRAM_PATHS}
)
FIND_PROGRAM(DCMTK_echoscu_PROGRAM 
	NAMES echoscu
	PATHS ${DCMTK_PROGRAM_PATHS}
)
FIND_PROGRAM(DCMTK_findscu_PROGRAM
	NAMES findscu
	PATHS ${DCMTK_PROGRAM_PATHS}
)
FIND_PROGRAM(DCMTK_movescu_PROGRAM
	NAMES movescu
	PATHS ${DCMTK_PROGRAM_PATHS}
)
FIND_PROGRAM(DCMTK_storescp_PROGRAM
	NAMES storescp 
	PATHS ${DCMTK_PROGRAM_PATHS}
)


IF( DCMTK_dump2dcm_PROGRAM
    AND DCMTK_echoscu_PROGRAM 
    AND DCMTK_findscu_PROGRAM
    AND DCMTK_movescu_PROGRAM
    AND DCMTK_storescp_PROGRAM)

  SET( DCMTK_FOUND "YES" )
  
ENDIF( DCMTK_dump2dcm_PROGRAM
    AND DCMTK_echoscu_PROGRAM
    AND DCMTK_findscu_PROGRAM
    AND DCMTK_movescu_PROGRAM
    AND DCMTK_storescp_PROGRAM)

IF( NOT DCMTK_FOUND )
  SET( DCMTK_DIR "" CACHE PATH "Root of DCMTK source tree (optional)." )
ENDIF( NOT DCMTK_FOUND )
