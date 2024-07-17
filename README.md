# Czara Game Engine => Czara Engine => Czengine
*Kenneth Cornett (Czara)* Project

## About
Czengine is a Vulkan SDL project.  Originally, Czengine started as a passion project to build the same game engine in C#, C++, and Rust.  I decided to focus mainly on C++, because, I am building it with the intent to design my own game.  Czengine is currently written using the C++23 standard from GNU.  I am developing it on Windows using MinGW.  I am using Premake5 to build the makefiles used to build the executable of the project.  Forewarning though, I am illiterate with Lua, so my Premake5 file is relatively rough to look at, currently. 

The project is using Dear ImGui for implementing an interface, and I'm utilizing the Viewport + Docking branch of Dear ImGui.  I am using TOML++ and PugiXML to process documents.  I wrote my own logging library which outputs to log files.  Log files generated are either time-based or size-based.

## Dependencies

| Library | Version | Source Code Built in Project |
|:---------|:------|:--------------------------------|
| C++ | 23 | - |
| Python | 3.12 | - |
| Premake | 5 | - |
| Vulkan | 1.3.283.0 | No |
| SDL2 | 2.30.0 | No |
| SDL2_ttf | 2.22.0 | No |
| Dear ImGui | 1.90.8 | Yes |
| TOML++ | 3.4.0 | Yes |
| PugiXML | 1.14 | Yes |


## Setup <sup><sub>*(Work in Progress)*</sub></sup>
1) Install Premake5
1) Install MinGW
1) Install Python 3.12 (No additional libraries should be needed.)
1) Execute `python czengine.py` and follow text prompts.
1) Open bash window and execute `./build.sh --build`
   - This should execute `premake5 gmake2` and `make CXX=g++`
1) Ensure Vulkan library can be found by Premake5's makefiles
1) Currently Premake5 and makefiles  build with `--debug` flag, so output of executable will be in `bin/Win64/Debug`

## TODO
- [x] Start on Python3 install script
- [ ] Update Python install script to kick off executable install (for Vulkan)
- [ ] Add instructions to Python script to parse and download libraries belonging to `/extern/` directory.
- [ ] Implement Vulkan Render APIs for SDL2 and Dear ImGui
- [ ] Start writing unit tests for key features
- [ ] Restructure Czengine to be Git Repo with Sub Repos rather than one large repo.
- [ ] Integrate Linear Math library (most likely Eigen)
- [ ] Work on Render Pipeline
- [ ] Replace usage of `build.sh` with another Python script/flag