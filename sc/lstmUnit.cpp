/*************************************************

SystemC Transaction Level Modeling Tutorial
(c) 11/25/2019 W. Rhett Davis (rhett_davis@ncsu.edu)

**************************************************/

#include "lstmUnit.h"
#include <string>
#include <iostream>
#include <iomanip>

using namespace  std;


SC_HAS_PROCESS(lstmUnit);
lstmUnit::lstmUnit( sc_core::sc_module_name module_name)
  : sc_module (module_name)
{
  slave.register_b_transport(this, &lstmUnit::custom_b_transport);
  m_memory_size=sizeof(registers);
  data=new unsigned char[m_memory_size];
  regs=reinterpret_cast<registers*>(data);
}

lstmUnit::~lstmUnit()
{
  delete data;
}

long long lstmUnit::tanh_fxp(long long x)
{
  static short tanhLUT[256] = {
        0x0000, 0x0200, 0x0400, 0x05ff, 0x07fd, 0x09fb, 0x0bf7, 0x0df2, 
        0x0feb, 0x11e2, 0x13d7, 0x15c9, 0x17b9, 0x19a6, 0x1b90, 0x1d76, 
        0x1f59, 0x2139, 0x2314, 0x24ec, 0x26bf, 0x288e, 0x2a58, 0x2c1e, 
        0x2ddf, 0x2f9a, 0x3151, 0x3302, 0x34ae, 0x3655, 0x37f6, 0x3991, 
        0x3b27, 0x3cb6, 0x3e40, 0x3fc4, 0x4142, 0x42ba, 0x442c, 0x4598, 
        0x46fd, 0x485d, 0x49b6, 0x4b09, 0x4c56, 0x4d9d, 0x4edd, 0x5018, 
        0x514d, 0x527b, 0x53a3, 0x54c6, 0x55e2, 0x56f9, 0x580a, 0x5914, 
        0x5a1a, 0x5b19, 0x5c13, 0x5d07, 0x5df6, 0x5edf, 0x5fc4, 0x60a2, 
        0x617c, 0x6250, 0x6320, 0x63ea, 0x64b0, 0x6571, 0x662d, 0x66e4, 
        0x6797, 0x6846, 0x68f0, 0x6995, 0x6a37, 0x6ad4, 0x6b6e, 0x6c03, 
        0x6c95, 0x6d22, 0x6dac, 0x6e33, 0x6eb5, 0x6f35, 0x6fb0, 0x7029, 
        0x709e, 0x7110, 0x717f, 0x71eb, 0x7254, 0x72ba, 0x731e, 0x737e, 
        0x73dc, 0x7437, 0x7490, 0x74e6, 0x753a, 0x758b, 0x75da, 0x7627, 
        0x7672, 0x76ba, 0x7701, 0x7745, 0x7788, 0x77c8, 0x7807, 0x7844, 
        0x787f, 0x78b8, 0x78f0, 0x7926, 0x795b, 0x798e, 0x79bf, 0x79ef, 
        0x7a1e, 0x7a4b, 0x7a77, 0x7aa2, 0x7acb, 0x7af4, 0x7b1b, 0x7b41, 
        0x7b65, 0x7b89, 0x7bab, 0x7bcd, 0x7bee, 0x7c0d, 0x7c2c, 0x7c49, 
        0x7c66, 0x7c82, 0x7c9d, 0x7cb8, 0x7cd1, 0x7cea, 0x7d02, 0x7d19, 
        0x7d30, 0x7d46, 0x7d5b, 0x7d70, 0x7d84, 0x7d97, 0x7daa, 0x7dbc, 
        0x7dce, 0x7ddf, 0x7df0, 0x7e00, 0x7e0f, 0x7e1f, 0x7e2d, 0x7e3c, 
        0x7e49, 0x7e57, 0x7e64, 0x7e70, 0x7e7d, 0x7e88, 0x7e94, 0x7e9f, 
        0x7eaa, 0x7eb4, 0x7ebf, 0x7ec8, 0x7ed2, 0x7edb, 0x7ee4, 0x7eed, 
        0x7ef5, 0x7efd, 0x7f05, 0x7f0d, 0x7f14, 0x7f1c, 0x7f23, 0x7f29, 
        0x7f30, 0x7f36, 0x7f3d, 0x7f43, 0x7f48, 0x7f4e, 0x7f54, 0x7f59, 
        0x7f5e, 0x7f63, 0x7f68, 0x7f6c, 0x7f71, 0x7f75, 0x7f7a, 0x7f7e, 
        0x7f82, 0x7f86, 0x7f89, 0x7f8d, 0x7f91, 0x7f94, 0x7f97, 0x7f9b, 
        0x7f9e, 0x7fa1, 0x7fa4, 0x7fa6, 0x7fa9, 0x7fac, 0x7fae, 0x7fb1, 
        0x7fb3, 0x7fb6, 0x7fb8, 0x7fba, 0x7fbc, 0x7fbe, 0x7fc0, 0x7fc2, 
        0x7fc4, 0x7fc6, 0x7fc8, 0x7fca, 0x7fcb, 0x7fcd, 0x7fce, 0x7fd0, 
        0x7fd1, 0x7fd3, 0x7fd4, 0x7fd6, 0x7fd7, 0x7fd8, 0x7fd9, 0x7fdb, 
        0x7fdc, 0x7fdd, 0x7fde, 0x7fdf, 0x7fe0, 0x7fe1, 0x7fe2, 0x7fe3, 
        0x7fe4, 0x7fe5, 0x7fe5, 0x7fe6, 0x7fe7, 0x7fe8, 0x7fe9, 0x7fe9, 
        };
    long sign,x0i,x1i;
    long long x0,x1,y0,y1;
    sign=1;
    if (x<0) {
      sign=-1;
      x=-x;
    }
    x0i=x>>9;
    if (x0i>=255)
      return sign*tanhLUT[255];
    x0 = x0i<<9;
    y0 = tanhLUT[x0i];
    if (x0==x)
      return sign*y0;
    x1i=x0i+1;
    x1 = x1i<<9;
    y1 = tanhLUT[x1i];
    return sign*((y0*(x1-x)+y1*(x-x0))>>9);
}

long long lstmUnit::sigmoid_fxp(long long x)
{
  return (tanh_fxp(x>>1)>>1) + 16384;
}

short lstmUnit::saturate(long long x)
{
  if (x>32767)
    return 32767;
  if (x<-32768)
    return -32768;
  return x;
}

void lstmUnit::calculate_layer(lstmCoef *coef, 
                short *c_t1, short *h_t1, short *c_t2, short *h_t2,
                short *x)
{
  unsigned int j,k;
  long long f[NINP],g[NINP],o[NINP],i[NINP];
  for (j=0; j<NINP; j++) {
    f[j]=0;
    for (k=0; k<NINP; k++)
      f[j]+=(((long long)coef->Wf[j][k])*x[k])>>15;
    for (k=0; k<NINP; k++)
      f[j]+=(((long long)coef->Uf[j][k])*h_t1[k])>>15;
    f[j]+=coef->bf[j];
    f[j]=sigmoid_fxp(f[j]);
  }
  for (j=0; j<NINP; j++) {
    g[j]=0;
    for (k=0; k<NINP; k++)
      g[j]+=(((long long)coef->Wg[j][k])*x[k])>>15;
    for (k=0; k<NINP; k++)
      g[j]+=(((long long)coef->Ug[j][k])*h_t1[k])>>15;
    g[j]+=coef->bg[j];
    g[j]=tanh_fxp(g[j]);
  }
  for (j=0; j<NINP; j++) {
    i[j]=0;
    for (k=0; k<NINP; k++)
      i[j]+=(((long long)coef->Wi[j][k])*x[k])>>15;
    for (k=0; k<NINP; k++)
      i[j]+=(((long long)coef->Ui[j][k])*h_t1[k])>>15;
    i[j]+=coef->bi[j];
    i[j]=sigmoid_fxp(i[j]);
  }
  for (j=0; j<NINP; j++) {
    o[j]=0;
    for (k=0; k<NINP; k++)
      o[j]+=(((long long)coef->Wo[j][k])*x[k])>>15;
    for (k=0; k<NINP; k++)
      o[j]+=(((long long)coef->Uo[j][k])*h_t1[k])>>15;
    o[j]+=coef->bo[j];
    o[j]=sigmoid_fxp(o[j]);
  }
  for (j=0; j<NINP; j++) {
    c_t2[j]=saturate((f[j]*c_t1[j]+i[j]*g[j])>>15);
  }
  for (j=0; j<NINP; j++) {
    h_t2[j]=saturate((o[j]*tanh_fxp(c_t2[j]))>>15);
  }
  
  return;
}

void lstmUnit::calculate()
{
	
  unsigned int i,j;
  lstmCoef coef1;
  short input[TSTEP][NINP] = {{0}};
  short c[TSTEP+1][NINP] = {{0}};
  short h[TSTEP+1][NINP] = {{0}};
  sc_core::sc_time mem_delay(8481,sc_core::SC_NS);
  sc_core::sc_time start = sc_core::sc_time_stamp();
  
  cout << sc_core::sc_time_stamp() << " lstm_calc Calculation Begun " << endl;
  
  wait(mem_delay);
  for(i = 0; i < TSTEP; i++) {
	  for(j = 0; j < NINP*2; j+=2) {
		  input[i][j/2] = (short)data[32 + i*32 + j] + (((short)data[32 + i*32 + j + 1]) << 8);
	  }
  }
	   
  if((int)data[8] == 2) {
	for(j = 0; j < NINP*2; j+=2) {
	  c[0][j/2] = (short)data[544 + j] + (((short)data[544 + j + 1]) << 8);
	  h[0][j/2] = (short)data[1088 + j] + (((short)data[1088 + j + 1]) << 8);
    }
  }
  
  for(i = 0; i < NINP*2; i+=2) {
	  coef1.bf[i/2] = (short)data[2656 + i] + (((short)data[2656 + i + 1]) << 8);
	  coef1.bg[i/2] = (short)data[3712 + i] + (((short)data[3712 + i + 1]) << 8);
	  coef1.bi[i/2] = (short)data[4768 + i] + (((short)data[4768 + i + 1]) << 8);
	  coef1.bo[i/2] = (short)data[5824 + i] + (((short)data[5824 + i + 1]) << 8);
	  for(j = 0; j < NINP*2; j+=2) {
		  coef1.Wf[i/2][j/2] = (short)data[1632 + (i/2)*32 + j] + (((short)data[1632 + (i/2)*32 + j + 1]) << 8);
		  coef1.Wg[i/2][j/2] = (short)data[2688 + (i/2)*32 + j] + (((short)data[2688 + (i/2)*32 + j + 1]) << 8);
		  coef1.Wi[i/2][j/2] = (short)data[3744 + (i/2)*32 + j] + (((short)data[3744 + (i/2)*32 + j + 1]) << 8);
		  coef1.Wo[i/2][j/2] = (short)data[4800 + (i/2)*32 + j] + (((short)data[4800 + (i/2)*32 + j + 1]) << 8);
		  coef1.Uf[i/2][j/2] = (short)data[2144 + (i/2)*32 + j] + (((short)data[2144 + (i/2)*32 + j + 1]) << 8);
		  coef1.Ug[i/2][j/2] = (short)data[3200 + (i/2)*32 + j] + (((short)data[3200 + (i/2)*32 + j + 1]) << 8);
		  coef1.Ui[i/2][j/2] = (short)data[4256 + (i/2)*32 + j] + (((short)data[4256 + (i/2)*32 + j + 1]) << 8);
		  coef1.Uo[i/2][j/2] = (short)data[5312 + (i/2)*32 + j] + (((short)data[5312 + (i/2)*32 + j + 1]) << 8);
	  }
  }
  
  printf("lstm_debug c contents: ");
  for(i = 0; i < TSTEP + 1; i++) {
	  for(j = 0; j < NINP; j++) {
		  printf("0x%04x, ", (unsigned short)c[i][j]);
	  }
  }
  printf("\n");
  
  printf("lstm_debug h contents: ");
  for(i = 0; i < TSTEP + 1; i++) {
	  for(j = 0; j < NINP; j++) {
		  printf("0x%04x, ", (unsigned short)h[i][j]);
	  }
  }
  printf("\n");
  
  printf("lstm_debug input contents: ");
  for(i = 0; i < TSTEP; i++) {
	  for(j = 0; j < NINP; j++) {
		  printf("%d, ", input[i][j]);
	  }
  }
  printf("\n");
  
  for (j=0; j<TSTEP; j++) 
    calculate_layer(&coef1,c[j],h[j],c[j+1],h[j+1],input[j]);

  for(i = 0; i < TSTEP + 1; i++) {
	for(j = 0; j < NINP*2; j+=2) {
		data[544 + i*32 + j] = (char)(c[i][j/2] & 0xFF);
		data[544 + i*32 + j + 1] = (char)((c[i][j/2] & 0xFF00) >> 8);
		data[1088 + i*32 + j] = (char)(h[i][j/2] & 0xFF);
		data[1088 + i*32 + j + 1] = (char)((h[i][j/2] & 0xFF00) >> 8);
	}
  }
  data[0] = 0;		// Signals end of calculation to CPU
  cout << sc_core::sc_time_stamp() << " lstm_calc Calculation Completed in " << sc_core::sc_time_stamp() - start << endl;
  printf("lstm_debug calculation complete\n");
}

void lstmUnit::custom_b_transport
 ( tlm::tlm_generic_payload &gp, sc_core::sc_time &delay )
{
  sc_dt::uint64    address   = gp.get_address();
  tlm::tlm_command command   = gp.get_command();
  unsigned long    length    = gp.get_data_length();
  unsigned long    i, cycles;
  unsigned char    *dp       = gp.get_data_ptr();
  sc_core::sc_time mem_delay(1,sc_core::SC_NS);
  cycles=(length/8 + length%8);
  mem_delay=sc_core::sc_time(cycles,sc_core::SC_NS);
  wait(delay+mem_delay);

  cout << sc_core::sc_time_stamp() << " " << sc_object::name();
  if (address < m_memory_size) {
    switch (command) {
      case tlm::TLM_WRITE_COMMAND:
      {
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
		if (address==0x00000008) {
			data[0] = 1;		// Set high to signal calculation in progress
			calculate();
		}
        gp.set_response_status( tlm::TLM_OK_RESPONSE );
        break;
      }
      case tlm::TLM_READ_COMMAND:
      {
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

 
  cout << sc_core::sc_time_stamp() << " " << sc_object::name();
  cout << " sizeof(registers) " << hex << sizeof(registers) 
       << " Wf[0][0] "  << regs->my_coef.Wf[0][0]  << " " << *((short*)(data+0x0660)) << " eac2"
       << " bo[15] "  << regs->my_coef.bo[15] << " " << *((short*)(data+0x16de)) << " a047"
       << endl;
  


  return;     
}







