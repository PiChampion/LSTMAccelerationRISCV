/*************************************************

SystemC Transaction Level Modeling Tutorial
(c) 11/25/2019 W. Rhett Davis (rhett_davis@ncsu.edu)

**************************************************/


#ifndef __LSTMUNIT_H__ 
#define __LSTMUNIT_H__

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"


class lstmUnit: public sc_core::sc_module
{
  public:  

  static const unsigned int NINP=16;
  static const unsigned int TSTEP=16;

  class lstmCoef {
    public:
    short Wf[NINP][NINP], Uf[NINP][NINP], bf[NINP];
    short Wg[NINP][NINP], Ug[NINP][NINP], bg[NINP];
    short Wi[NINP][NINP], Ui[NINP][NINP], bi[NINP];
    short Wo[NINP][NINP], Uo[NINP][NINP], bo[NINP];
  };

  class registers {
    public:
    long long st;
    long long ctrl;
    unsigned char reserved1[16];
    short x[TSTEP][NINP];
    short c[TSTEP+1][NINP];
    short h[TSTEP+1][NINP];
    lstmCoef my_coef;
  };
  registers *regs;
  unsigned char *data;

  static const unsigned int buswidth=64;
  sc_dt::uint64  m_memory_size;
  sc_core::sc_mutex m_mutex;
  
  void calculate ( );
  void calculate_layer (lstmCoef *coef, 
                short *c_t1, short *h_t1, short *c_t2, short *h_t2,
                short *x);
  short saturate (long long x);
  long long sigmoid_fxp (long long x);
  long long tanh_fxp (long long x);

  lstmUnit( sc_core::sc_module_name module_name);

  ~lstmUnit();

  tlm_utils::simple_target_socket<lstmUnit,buswidth>  slave;
 
  private:
	    

  void custom_b_transport
  ( tlm::tlm_generic_payload &gp, sc_core::sc_time &delay );

};


#endif /* __LSTMTOP_H__ */
