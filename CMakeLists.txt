cmake_minimum_required(VERSION 2.6)
 
# Name of project
project(South)

# Specify project files: header files and source files
set(HDRS
	./ogre_application.h ./SoundManager.h ./Gorilla.h ./CollisionTools.h
)
 
set(SRCS
	./SoundManager.cpp ./Gorilla.cpp ./CollisionTools.cpp ./ogre_application.cpp ./SoundManager.cpp ./main.cpp ./material/MaterialVp.glsl ./material/MaterialFp.glsl ./material/MaterialFile.material ./material/ExhaustMaterialFp.glsl ./material/ExhaustMaterialGp.glsl ./material/ExhaustMaterialVp.glsl ./material/FireMaterialFp.glsl ./material/FireMaterialGp.glsl ./material/FireMaterialVp.glsl ./material/MaterialFp.glsl ./material/MaterialVp.glsl ./material/ParticleMaterial2Fp.glsl ./material/ParticleMaterial2Gp.glsl ./material/ParticleMaterial2Vp.glsl ./material/ParticleMaterial3Fp.glsl ./material/ParticleMaterial3Vp.glsl ./material/ParticleMaterial4Vp.glsl ./material/PointsMaterialFp.glsl ./material/PointsMaterialGp.glsl ./material/PointsMaterialVp.glsl ./material/RingMaterialFp.glsl ./material/RingMaterialVp.glsl ./material/ShockMaterialFp.glsl ./material/ShockMaterialVp.glsl ./material/SplineMaterialFp.glsl ./material/SplineMaterialGp.glsl ./material/SplineMaterialVp.glsl ./material/water.material ./material/Material.000.material ./material/Material.001.material ./material/Material.0000.material ./material/Material.002.material ./material/Material.003.material ./material/Material.004.material ./material/MaterialFile.material ./material/MaterialSky.material ./material/Particle.material ./material/cactus.material

)

# The rules here are specific to Windows Systems
if(WIN32)
    # Get Ogre directory from the environment variable
    set(OGRE_HOME $ENV{OGRE_HOME})

    # Set up Ogre include directories
    include_directories(
        "${OGRE_HOME}/include"
        "${OGRE_HOME}/include/OIS"
        "${OGRE_HOME}/include/OGRE"
        "${OGRE_HOME}/boost"
    )

    # Set up Ogre library directories
    link_directories(
        "${OGRE_HOME}/lib/"
        "${OGRE_HOME}/boost/lib"
    )

    # Applications using Ogre need to be run from inside the Ogre
    # directory. This cmake command needs to come before add_executable
    set(EXECUTABLE_OUTPUT_PATH "${OGRE_HOME}/bin/")

    # Add path name
    configure_file(path_config.h.in path_config.h)

    # Add executable based on the source files
    add_executable(South ${HDRS} ${SRCS})

    # Set up names of Ogre libraries
    target_link_libraries(South
        "OgreMain_d.lib"
        "OIS_d.lib"
        "OgreOverlay_d.lib"
    )

    # Avoid ZERO_CHECK target 
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode
    set_target_properties(South PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
