#initialise le projet
init:
	mkdir build
	cd build && cmake .. -G "MinGW Makefiles"

# initialise le projet sous unix
init-unix:
	mkdir build
	cd build && cmake ..

# build du projet
build:
	cd build && cmake ..

# lancement du projet
launch:
	cd build && make && cls && game.exe

# lancement le projet sous unix avec valgrind
launch-unix:
	cd build && make && clear && valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./game

.PHONY: init build
