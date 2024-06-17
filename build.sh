#!/bin/bash

NC='\033[1;0m'
RED='\033[1;31m'
GREEN='\033[1;32m'
CYAN='\033[1;36m'
YELLOW='\033[1;33m'
LB='\033[1;94m'
LM='\033[1;95m'

MAKEFILE="Makefile"
CZENGINE_MAKE="CzenginePlusPlus.make"
## Attempt to assign argument 1 if provided, if not, then default to building.
opt="${1:=--build}"

function setupBuildMode() {
    BUILD_MODE="$1"
    if [[ "$BUILD_MODE" != "--prod" && "$BUILD_MODE" != "--debug" ]]; then
        printf "${RED}Invalid second argument for build command.  Valid arguments: --production, --debug.\n"
        exit 2
    fi
}
function cleanPremake() {
    ## If Makefile or CzenginePlusPlus.make exists
    if [ -e "${MAKEFILE}" -o -e "${CZENGINE_MAKE}" ]; then
        printf "${CYAN}Deleting generated Makefiles.\n"
        ## Delete the files
        rm -rf "${MAKEFILE}"
        rm -rf "${CZENGINE_MAKE}"
        printf "${CYAN}Generated Makefiles deleted.\n"
    fi
}
function cleanSolution() {
    printf "${CYAN}Cleaning object files.\n"
    rm -rf obj/*
    printf "${CYAN}Cleaning executable, logs, and assets.\n"
    rm -rf bin/*
    printf "${CYAN}Finished cleaning solution.\n"
}
function buildSolution() {
    if [[ ! -f "${MAKEFILE}" || ! -f "${CZENGINE_MAKE}" ]]; then
        cleanPremake
        buildPremake
    fi
    printf "${GREEN}Building solution:\n"
    MAKE_OO=$(make CXX=g++)
    printf "${GREEN}Make output:\n"
    printf "${GREEN}${MAKE_OO}\n"
    printf "${GREEN}Copying assets.\n"
    if [ ! -d "./bin/Win64/Debug/czengine-interface" ]; then
        cp -rf "./czengine-interface" "bin/Win64/Debug/czengine-interface"
    fi
}
function buildPremake() {
    cleanPremake
    printf "${GREEN}Generating makefiles\n"
    PREMAKE_OO=$(premake5 gmake2)
    printf "${GREEN}Premake Output:\n"
    printf "${GREEN}${PREMAKE_OO}"
}
function displayHelp() {
    printf "Printing help\n"
    printf "Valid commands:\n"
    printf "\t${LB}--build [{--debug}|--prod]       ${LM}> Attempts to run 'make CXX=g++'.\n"
    printf "\t${LB}--premake [{--debug}|--prod]     ${LM}> Generates fresh Makefile(s) and runs '--build'.\n"
    printf "\t${LB}--clean-build [{--debug}|--prod] ${LM}> Cleans '--build' artifacts and reruns '--build'.\n"
    printf "\t${LB}--clean                          ${LM}> Deletes '--premake' and '--build' artifacts.\n"
    printf "\t${LB}--help                           ${LM}> Displays **this help message.\n"
    printf "\n\n${NC}"
    printf "\tExample: '${YELLOW}build.sh --build${NC}'\n"
    printf "\tExample: '${YELLOW}build.sh --build --debug${NC}' (Same as previous command. Displaying for completeness.)\n"
    printf "\tExample: '${YELLOW}build.sh --premake --prod${NC}'\n"
    printf "\tExample: '${YELLOW}build.sh --clean${NC}'\n"
}

case "$opt" in
    "--build")
        if [ $# -eq 2 ]; then
            MODE="$2"
        else
            MODE="--debug"
        fi
        setupBuildMode "${MODE}"
        buildSolution;;
    "--premake")
        if [ $# -eq 2 ]; then
            MODE="$2"
        else
            MODE="--debug"
        fi
        cleanSolution
        setupBuildMode "${MODE}"
        buildPremake
        buildSolution;;
    "--clean-build")

        if [ $# -eq 2 ]; then
            MODE="$2"
        else
            MODE="--debug"
        fi
        cleanSolution
        setupBuildMode "${MODE}"
        buildSolution;;
    "--clean")
        cleanPremake
        cleanSolution;;
    "--help")
        displayHelp;;
esac