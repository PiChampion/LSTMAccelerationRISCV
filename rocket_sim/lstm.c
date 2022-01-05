
#include <stdio.h>

long long tanh_fxp(long long x)
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

long long sigmoid_fxp(long long x)
{
  return (tanh_fxp(x>>1)>>1) + 16384;
}

short saturate(long long x)
{
  if (x>32767)
    return 32767;
  if (x<-32768)
    return -32768;
  return x;
}

#define NINP 16
#define TSTEP 32

typedef struct lstmCoef {
  short Wf[NINP][NINP], Uf[NINP][NINP], bf[NINP];
  short Wg[NINP][NINP], Ug[NINP][NINP], bg[NINP];
  short Wi[NINP][NINP], Ui[NINP][NINP], bi[NINP];
  short Wo[NINP][NINP], Uo[NINP][NINP], bo[NINP];
} lstmCoef;

void lstm_layer(lstmCoef *coef, 
                short *c_t1, short *h_t1, short *c_t2, short *h_t2,
                short *x)
{
  int j,k;
  long long f[NINP],g[NINP],o[NINP],i[NINP],temp;
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
}

// Coefficinets and input moved to memory controller module
// but can be included here for simplicity, if desired.

/*
lstmCoef coef1 = {
#include "coef1.inc"
};
lstmCoef coef2 = {
#include "coef2.inc"
};

short input[TSTEP][NINP] = {
#include "input.inc"
};
*/

#include "expected.inc"

int main( int argc, char* argv[] )
{
  volatile long long *llp;
  volatile long long **llpp;
  
  printf("lstm_debug writing layer 1 coefs\n");
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00004000; // memctl coef1 address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010660; // lstmTop coef
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)sizeof(lstmCoef); // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing layer 1 coefs\n");
  
////////////////////////////////////////////////////////////////////////////////////////////
// Send first 16 time steps to layer 1
  printf("lstm_debug sending input to lstm: #1\n");
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00002000; // memctl input address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010020; // lstmTop input
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)512; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug sent input to lstm: #1\n");
  /*
  printf("lstm_debug sending first c value to lstm: #1\n");
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00000000; // memctl c address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010220; // lstmTop c
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)32; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug sent first c value to lstm: #1\n");
  printf("lstm_debug sending first h value to lstm: #1\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00001000; // memctl h address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010440; // lstmTop h
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)32; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug sent first h value to lstm: #1\n");
  */
  printf("lstm_debug initiating the calculation and waiting: #1\n");
  
  llpp=(volatile long long**)0x70010008; // lstm ctrl
  *llpp=(volatile long long*)0x00000001; // starts calculation
  llp=(volatile long long*)0x70010000;   // lstm st
  
  // The following line polls the lstm st register until it is 0,
  // effectively waiting until the calculation is complete.
  while (*llp); 
  printf("lstm_debug calculation stopped: #1\n");
  printf("lstm_debug writing c result to memory: #1\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x10010220; // lstmTop c
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x00000000; // memctl c address
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)544; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing c result to memory: #1\n");
  printf("lstm_debug writing h result to memory: #1\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x10010440; // lstmTop h
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x00001000; // memctl h address
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)544; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing h result to memory: #1\n");
////////////////////////////////////////////////////////////////////////////////////////////
// Send second 16 time steps to layer 1
  printf("lstm_debug sending input to lstm: #2\n");
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00002200; // memctl input address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010020; // lstmTop input
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)512; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp);   
  printf("lstm_debug sent input to lstm: #2\n");
  printf("lstm_debug sending first c value to lstm: #2\n");

  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00000200; // memctl c address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010220; // lstmTop c
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)32; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug sent first c value to lstm: #2\n");
  printf("lstm_debug sending first h value to lstm: #2\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00001200; // memctl h address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010440; // lstmTop h
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)32; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug sent first h value to lstm: #2\n");
  printf("lstm_debug initiating the calculation and waiting: #2\n");
  
  llpp=(volatile long long**)0x70010008; // lstm ctrl
  *llpp=(volatile long long*)0x00000002; // starts calculation
  llp=(volatile long long*)0x70010000;   // lstm st
  
  // The following line polls the lstm st register until it is 0,
  // effectively waiting until the calculation is complete.
  while (*llp); 
  printf("lstm_debug calculation stopped: #2\n");
  printf("lstm_debug writing c result to memory: #2\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x10010240; // lstmTop c
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x00000220; // memctl c address
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)512; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing c result to memory: #2\n");
  printf("lstm_debug writing h result to memory: #2\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x10010460; // lstmTop h
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x00001220; // memctl h address
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)512; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing h result to memory: #2\n");
////////////////////////////////////////////////////////////////////////////////////////////
// Send first 16 time steps to layer 2
  printf("lstm_debug writing layer 2 coefs\n");
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00006000; // memctl coef2 address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010660; // lstmTop coef
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)sizeof(lstmCoef); // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing layer 2 coefs\n");

  printf("lstm_debug sending input to lstm: #3\n");
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00001020; // memctl h address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010020; // lstmTop input
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)512; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp);   
  printf("lstm_debug sent input to lstm: #3\n");
  /*
  printf("lstm_debug sending first c value to lstm: #3\n");

  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00000800; // memctl c address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010220; // lstmTop c
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)32; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug sent first c value to lstm: #3\n");
  printf("lstm_debug sending first h value to lstm: #3\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00001800; // memctl h address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010440; // lstmTop h
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)32; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug sent first h value to lstm: #3\n");
  */
  printf("lstm_debug initiating the calculation and waiting: #3\n");
  
  llpp=(volatile long long**)0x70010008; // lstm ctrl
  *llpp=(volatile long long*)0x00000001; // starts calculation
  llp=(volatile long long*)0x70010000;   // lstm st
  
  // The following line polls the lstm st register until it is 0,
  // effectively waiting until the calculation is complete.
  while (*llp); 
  printf("lstm_debug calculation stopped: #3\n");
  printf("lstm_debug writing c result to memory: #3\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x10010220; // lstmTop c
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x00000800; // memctl c address
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)544; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing c result to memory: #3\n");
  printf("lstm_debug writing h result to memory: #3\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x10010440; // lstmTop h
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x00001800; // memctl h address
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)544; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing h result to memory: #3\n");
////////////////////////////////////////////////////////////////////////////////////////////
// Send second 16 time steps to layer 2
  printf("lstm_debug sending input to lstm: #4\n");
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00001220; // memctl h address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010020; // lstmTop input
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)512; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp);   
  printf("lstm_debug sent input to lstm: #4\n");
  printf("lstm_debug sending first c value to lstm: #4\n");

  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00000A00; // memctl c address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010220; // lstmTop c
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)32; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug sent first c value to lstm: #4\n");
  printf("lstm_debug sending first h value to lstm: #4\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x00001A00; // memctl h address
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x10010440; // lstmTop h
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)32; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug sent first h value to lstm: #4\n");
  printf("lstm_debug initiating the calculation and waiting: #4\n");
  
  llpp=(volatile long long**)0x70010008; // lstm ctrl
  *llpp=(volatile long long*)0x00000002; // starts calculation
  llp=(volatile long long*)0x70010000;   // lstm st
  
  // The following line polls the lstm st register until it is 0,
  // effectively waiting until the calculation is complete.
  while (*llp); 
  printf("lstm_debug calculation stopped: #4\n");
  printf("lstm_debug writing c result to memory: #4\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x10010240; // lstmTop c
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x00000A20; // memctl c address
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)512; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing c result to memory: #4\n");
  printf("lstm_debug writing h result to memory: #4\n");
  
  llpp=(volatile long long**)0x70000010; // dma sr
  *llpp=(volatile long long*)0x10010460; // lstmTop h
  llpp=(volatile long long**)0x70000018; // dma dr
  *llpp=(volatile long long*)0x00001A20; // memctl h address
  llp=(volatile long long*)0x70000020;   // dma len
  *llp=(volatile long long)512; // starts transfer
  llp=(volatile long long*)0x70000000;   // dma st
  
  // The following line polls the dma st register until it is 0,
  // effectively waiting until the transfer is complete.
  while (*llp); 
  printf("lstm_debug done writing h result to memory: #4\n");  
 
  int j,k;

  short (*c1)[NINP]=(short *)0x60000000;
  short (*c2)[NINP]=(short *)0x60000800;
  short (*h1)[NINP]=(short *)0x60001000;
  short (*h2)[NINP]=(short *)0x60001800;
  long error;

  error=0;
  for (k=0; k<NINP; k++) {
    error+=c1_exp[TSTEP-1][k]-c1[TSTEP][k];
    error+=h1_exp[TSTEP-1][k]-h1[TSTEP][k];
    error+=c2_exp[TSTEP-1][k]-c2[TSTEP][k];
    error+=h2_exp[TSTEP-1][k]-h2[TSTEP][k];
  }
  printf("cpu main error: %d\n",error);


  return 0;
}
