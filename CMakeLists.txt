cmake_minimum_required(VERSION 3.10)
project(Voxelyze)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(Utils)


add_library(Voxelyze
        Utils/Array3D.cpp
        Utils/Array3D.h
        Utils/MarchCube.cpp
        Utils/MarchCube.h
        Utils/Mesh.cpp
        Utils/Mesh.h
        Utils/tinyxml.cpp
        Utils/tinyxml.h
        Utils/tinyxmlerror.cpp
        Utils/tinyxmlparser.cpp
        Utils/XML_Rip.cpp
        Utils/XML_Rip.h
        VX_Benchmark.cpp
        VX_Benchmark.h
        VX_Bond.cpp
        VX_Bond.h
        VX_Environment.cpp
        VX_Environment.h
        VX_FEA.cpp
        VX_FEA.h
        VX_FRegion.cpp
        VX_FRegion.h
        VX_MeshUtil.cpp
        VX_MeshUtil.h
        VX_Object.cpp
        VX_Object.h
        VX_Sim.cpp
        VX_Sim.h
        VX_SimGA.cpp
        VX_SimGA.h
        VX_Voxel.cpp
        VX_Voxel.h
        VXS_Bond.cpp
        VXS_Bond.h
        VXS_BondCollision.cpp
        VXS_BondCollision.h
        VXS_BondInternal.cpp
        VXS_BondInternal.h
        VXS_Voxel.cpp
        VXS_Voxel.h)