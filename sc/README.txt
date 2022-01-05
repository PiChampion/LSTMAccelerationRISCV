 - Log in to a Linux Lab Machine
 - Change to the directory that contains this file
 - Source the setup script with the command "source setup.sh"
 - Build the main.x executable with the command "make"
 - Change to another directory containing the rocket_sim
     tutorial files (already copied for you in ../rocket_sim)
 - Find the following line in the rocket_sim Makefile:
     RISCV_SIM ?= spike --isa=rv$(XLEN)gc and change "spike"
     to the path to the main.x executable that you compiled 
     earlier (this change is already made for you) 
 - Execute (simulate) the project with the commands "make"
     and  "make sim" in the rocket_sim directory.
 - Repeat the "make" commands in each directory, followed by
     "make sim" in the rocket_sim directory as needed to rebuild
     and re-simulate the project.

Notes:
 - Use the "make clean" command to delete all generated files, in order
     to prepare the directory for archiving.

