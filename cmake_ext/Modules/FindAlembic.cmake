include(FindPackageHandleStandardArgs)

# List of the valid HDF5 components
set( ALEMBIC_VALID_COMPONENTS
    Material
    Geom
    Ogawa
    HDF5
)

SET( ALEMBIC_Ogawa_COMP 
	  AlembicAbcCoreOgawa
          AlembicOgawa
 )
SET( ALEMBIC_HDF5_COMP AlembicAbcCoreHDF5 )
SET( ALEMBIC_Geom_COMP AlembicAbcGeom )
SET( ALEMBIC_Material_COMP AlembicAbcMaterial )

SET( ALEMBIC_LIBS
     AlembicAbcMaterial
     AlembicAbcGeom
     AlembicAbcCoreFactory
     AlembicAbc
     AlembicAbcCoreHDF5
     AlembicAbcCoreOgawa
     AlembicAbcCoreAbstract
     AlembicOgawa
     AlembicUtil
)
	
if( NOT Alembic_FIND_COMPONENTS )
    set( ALEMBIC_COMPS "" )
else()
    foreach( component ${Alembic_FIND_COMPONENTS} )
        list( FIND ALEMBIC_VALID_COMPONENTS ${component} component_location )
        if( ${component_location} EQUAL -1 )
            message( FATAL_ERROR "No ${component} component" )
	endif( ${component_location} EQUAL -1 )
    endforeach( component ${Alembic_FIND_COMPONENTS} )
    foreach( component ${ALEMBIC_VALID_COMPONENTS} )
	list( FIND Alembic_FIND_COMPONENTS ${component} component_location )
	if( ${component_location} EQUAL -1 )
		foreach( component_entry ${ALEMBIC_${component}_COMP} )
		  list( REMOVE_ITEM ALEMBIC_LIBS ${component_entry})
		endforeach( component_entry ${ALEMBIC_${component}_COMP} )
	endif( ${component_location} EQUAL -1 )
    endforeach( component ${ALEMBIC_VALID_COMPONENTS} )
endif( NOT Alembic_FIND_COMPONENTS )

foreach( LIB ${ALEMBIC_LIBS} )
	find_library( ALEMBIC_${LIB}_LIBRARY
			NAMES ${LIB}
			PATHS ENV ALEMBIC_ROOT
                	PATH_SUFFIXES lib Lib lib/static Lib/Static )
	if( ALEMBIC_${LIB}_LIBRARY )
		SET( Alembic_FOUND TRUE )
		list( APPEND Alembic_LIBRARIES ${ALEMBIC_${LIB}_LIBRARY})
	else( ALEMBIC_${LIB}_LIBRARY )
        set( Alembic_FOUND off )
	endif( ALEMBIC_${LIB}_LIBRARY )
endforeach( LIB ${ALEMBIC_LIBS} )

find_path( Alembic_INCLUDE_DIR NAMES Alembic/Abc/All.h
		PATHS ENV ALEMBIC_ROOT
		PATH_SUFFIXES include
)

find_package_handle_standard_args( Alembic DEFAULT_MSG
    Alembic_LIBRARIES
    Alembic_INCLUDE_DIR
)

