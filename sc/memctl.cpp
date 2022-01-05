/*************************************************

SystemC DDR SDRAM Controller Model
(c) 10/28/2020 W. Rhett Davis (rhett_davis@ncsu.edu)

This module models a DDR SDRAM with very simple delays.
The number of cycles for a read transfer is calculated
from the values of CCD and the number of DRAM data bits.
This delay is imcremented by CL (for all reads),
RCD (if an ACTIVE command is resuited), and RP
(if a PRECHARGE command is required).
It keeps track of whether or not each bank has been
initialized and the last address accessed for each 
bank. 

Things to notice:

 - Row-high addressing is assumed.

 - The memory_size argument to the constructor
   is much smaller than the actual size of the DRAM.
   This is to prevent the simulation from running out
   of memory.  In practice, some creative way is needed
   read and write the data assocated with each 
   transaction.

Room for improvement:

 - Only the base address is checked. If a
   transaction is large enough to cross a row boundary,
   then no RCD or RP latency is added.  This feature could
   be added with additional checking.

 - The address is not checked to see if it
   aligns with SDRAM burst.  

 - Write delays are assumed to be perfectly buffered and
   written out later, incurring only the delay of 
   8-byte transfers over the AXI slave port.  There
   is no addition of RCD or RP latencies or checking
   of read-after-write dependencies.

**************************************************/

#include "memctl.h"
#include <string>
#include <iostream>
#include <iomanip>

using namespace  std;

#define NINP 16
#define TSTEP 32

typedef struct lstmCoef {
  short Wf[NINP][NINP], Uf[NINP][NINP], bf[NINP];
  short Wg[NINP][NINP], Ug[NINP][NINP], bg[NINP];
  short Wi[NINP][NINP], Ui[NINP][NINP], bi[NINP];
  short Wo[NINP][NINP], Uo[NINP][NINP], bo[NINP];
} lstmCoef;

lstmCoef coef1 = {
#include "coef1.inc"
};
lstmCoef coef2 = {
#include "coef2.inc"
};
short input[TSTEP][NINP] = {
#include "input.inc"
};


SC_HAS_PROCESS(memctl);
memctl::memctl( sc_core::sc_module_name module_name, sc_dt::uint64 memory_size )
  : sc_module (module_name)
  , m_memory_size (memory_size)
{
  unsigned long i; 
  slave.register_b_transport(this, &memctl::custom_b_transport);
  data=new unsigned char[m_memory_size];
  for (i=0 ; i<8 ; i++ )
    m_initialized[i]=false;

  // Initialize memory with LSTM Coefficient and Input values
  unsigned char *src;
  src=reinterpret_cast<unsigned char*>(&input);
  for (i=0;i<sizeof(input);i++) {
    data[0x2000+i]=src[i];
  }
  src=reinterpret_cast<unsigned char*>(&coef1);
  for (i=0;i<sizeof(coef1);i++) {
    data[0x4000+i]=src[i];
  }
  src=reinterpret_cast<unsigned char*>(&coef2);
  for (i=0;i<sizeof(coef2);i++) {
    data[0x6000+i]=src[i];
  }

}

memctl::~memctl()
{
  delete data;
}

#define CL  16
#define CCD 4
#define RCD 16
#define RP  16
#define CLK_PERIOD 1
#define DATA_BITS  32

void                                        
memctl::custom_b_transport
 ( tlm::tlm_generic_payload &gp, sc_core::sc_time &delay )
{
  sc_dt::uint64    address   = gp.get_address();
  tlm::tlm_command command   = gp.get_command();
  unsigned long    length    = gp.get_data_length();
  unsigned long    i,bank,num_reads,bytes_per_read,cycles;
  unsigned char    *dp       = gp.get_data_ptr();
  sc_core::sc_time mem_delay(1,sc_core::SC_NS);

  bank=(unsigned long)((address & 0x0000000000007000)>>12);
  
  if (address < m_memory_size) {
    switch (command) {
      case tlm::TLM_WRITE_COMMAND:
      {
        // Assume WRITES are buffered and written later.
        // We need only consider the time for 8-byte transfers over the
        // 64-bit bus
        cycles=(length/8 + length%8);
        mem_delay=sc_core::sc_time(cycles*CLK_PERIOD,sc_core::SC_NS);
        wait(delay+mem_delay);
	if (!m_initialized[bank])
	  m_initialized[bank]=true;
        m_last_addr[bank]=address;
        cout << sc_core::sc_time_stamp() << " " << sc_object::name();
        cout << " WRITE len:0x" << hex << length << " addr:0x" << address; 
        if (dp) {
          cout << " data:0x";
          for (i=length;i>0;i--) {
            cout << hex << setfill('0') << setw(2) << (unsigned int)dp[i-1];
            data[address+i-1]=dp[i-1];
	  }
          cout << endl;
	}
        else
          cout << endl;

        gp.set_response_status( tlm::TLM_OK_RESPONSE );
        break;
      }
      case tlm::TLM_READ_COMMAND:
      {
        bytes_per_read=(2*CCD*DATA_BITS/8);
        num_reads=(length/bytes_per_read + length%bytes_per_read);
	if (!m_initialized[bank]) {
          // Open Row for the first time
	  cycles=CCD*num_reads+CL+RCD;
	  m_initialized[bank]=true;
	}
        else if ((address & 0xFFFFFFFFFFFF8000) == 
                    (m_last_addr[bank] & 0xFFFFFFFFFFFF8000))
	  // Same Row
	  cycles=CCD*num_reads+CL;
	else
	  // New Row
	  cycles=CCD*num_reads+CL+RCD+RP;
        m_last_addr[bank]=address;
        mem_delay=sc_core::sc_time(cycles*CLK_PERIOD,sc_core::SC_NS);
        wait(delay+mem_delay);
        
        cout << sc_core::sc_time_stamp() << " " << sc_object::name();
        cout << " READ len:0x" << hex << length << " addr:0x" << address; 
        if (dp) {
          cout << " data:0x";
          for (i=length;i>0;i--) {
            cout << hex << setfill('0') << setw(2) << (unsigned int)data[address+i-1];
            dp[i-1]=data[address+i-1];
	  }
          cout << endl;
	}
        else
          cout << endl;


        gp.set_response_status( tlm::TLM_OK_RESPONSE );
        break;
      }
      default:
      {
        cout << " ERROR Command " << command << " not recognized" << endl;
        gp.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
      } 
    }
  }
  else {
    cout << " ERROR Address 0x" << hex << address << " out of range" << endl;
    gp.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
  }  

  return;     
}






