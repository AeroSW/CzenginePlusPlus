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
| GoogleTest | 1.15.2 | Yes |
| JoMock | - | Yes |

## Setup <sup><sub>*(Work in Progress)*</sub></sup>
1) After cloning repository locally, also execute `git submodule update --init --recursive` to recursively install submodules.
   - I plan to add this to the `setup.py` at some point, but it isn't necessary just yet.
1) Install Premake5
1) Install MinGW
1) Create a **symlink** for MinGW's `mingw32-make.exe` to a file named `make.exe` in the same directory.
   - [Symlink instructions for windows (HowToGeek Website)](https://www.howtogeek.com/16226/complete-guide-to-symbolic-links-symlinks-on-windows-or-linux/)
1) Install Python 3.12, After Python 3.12 is installed, follow sub-steps below
   1) Execute `python scripts/src/setup/setup.py` and follow text prompts. 
      - This installs dependencies used by project. (Excluding **git** submodules)
   1) Run the command `python -m pip install ./scripts`.  This installs the `czengine` command line tool for building and launching Czengine.
   1) Now try `czengine --help`. It should print out the commands [czengine] can execute.
   1) Now try `czengine build --help`.  It should print out the help guide for the `build` command.
1) Open CLI Utility (Terminal on Unix) and execute `czengine build --target CzenginePlusPlus`
   - This should execute `premake5 gmake2` and `make CzenginePlusPlus CXX=g++ config=debug_win64`
1) Ensure Vulkan library can be found by Premake5's makefiles

## TODO
- [ ] Review Dear ImGui Vulkan Implementation.  (I should be able to pull windows out of window.) <---- Next task(s)
- [ ] Start writing unit tests for key features (In Progress)
- [ ] Integrate Linear Math library (most likely Eigen)
- [ ] Work on Render Pipeline
- [X] Replace usage of `build.sh` with another Python script/flag (Finished)
- [ ] Add installs for Python and Premake5
- [ ] Combine Python `setup.py` as another command in `czengine.py`.