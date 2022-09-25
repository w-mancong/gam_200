-- Solution Premake file
workspace "Artless"
	architecture "x64"
	
	configurations {
		"Debug", 
		"Release"
	}

	startproject "ALEngine"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
include "ALEngine"