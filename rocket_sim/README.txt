Quick Start Instructions:
$ source addsetup.sh  (to set up environment for NC State EOS system)
$ source varsetup.sh  (to set up environment for NC State EOS system)
$ make
$ make sim
$ make vsim

Makefile Targets:

default:  Generates $(PROGNAME).riscv, which is the elf-file binary for
          executing the program.  This target also generates 
          $(PROGNAME).riscv.dump, which is a human-readable version of
          the binary file contents.

sim:      Executes $(PROGNAME).riscv with the Spike Instruction-Set 
          Simulator.  In addition to the console output given above,
          the disassembled instruction trace is printed in 
          $(PROGNAME).spike.out along with the output of the Linux
          "time" command.

vsim:     Same as the "sim" target above, but uses the Verilog
          simulation executable compiled from the Rocket-Chip
          Verilator emulator.  The disassembled instruction trace is 
          printed in $(PROGNAME).emulator.out.

vcd:	  Same as the "vsim" target above, but also generates
          a Value Change Dump (VCD) file for the simulation.

vpd:	  Converts the VCD to a Synopsys compressed "VCDPlus" or VPD file.

view:	  Launches Synopsys DVE to view waveforms.

clean:    Deleted the output from the targets listed above.


Remember to change the PROGNAME variable in the Makefile when running
other programs.  Remember also to change the MAXCYCLES variable to
increase the maximum number of cycles for capturing all of the desired
Verilog emulator activity.


          

