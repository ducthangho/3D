Overview
=================

This directory contains source code for C++ implementation of y3d, gRPC.

Pre-requisites
=================

You need to have CMake, Visual Studio, Nuget command-line and Git installed on your computer before proceeding.

Environment Setup
=================

Open the appropriate Command Prompt from the Start menu.

For example VS2013 x64 Native Tools Command Prompt:

C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64>
Change to your working directory:

C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64>cd C:\Path\to\y3d
C:\Path\to\y3d>
Where C:\Path\to\y3d is path to your real working directory.

Add Y3D_ROOT pointing to the real y3d directory to your system variable.

Getting Sources
=================
Go to the Service project folder:
	C:\Path\to\y3d>cd Service
 	C:\Path\to\y3d\Service>

Execute the script	update_src.cmd, to clone grpc from github
 	C:\Path\to\y3d\Service>update_src.cmd

Build sources
===================
Execute the script	grpc_build.bat to build grpc libs
 	C:\Path\to\y3d\Service>grpc_build.bat

Dependencies
===================
Need TCLAP library to handle command line parameter parser. See LoaderCmd project

Test server
===================
1. Build all Service, YMax and LoaderCmd projects
2. Start 3ds max
3. When 3ds is fully loader, start the service by the following command
  LoaderCmd.exe -l 1
(type LoaderCmd.exe --help for more info. LoaderCmd.exe is in y3d\bin folder)
4. After the Service has been started, you can change the Service and ServiceImpl source code (Service depends on ServiceImpl for actual implementation of the Service). 
5. Build the Service project without restarting 3ds max
6. To manually stop the Service server from command line, use
   LoaderCmd.exe -l 0