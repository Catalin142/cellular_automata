workspace "cellular_automata"
	architecture "x86"
	startproject "Graphy"
	configurations { "Debug", "Release" }
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "cellular_automata"
