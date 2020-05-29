#ifndef __ADF4351_H__
#define __ADF4351_H__

//charge-pump-current
typedef enum ADF4351_ICP {
	ADF4351_ICP_031		= 0,		//0.31 mA
	ADF4351_ICP_063		= 1,		//0.63 mA
	ADF4351_ICP_094		= 2,		//0.94 mA
	ADF4351_ICP_125		= 3,		//1.25 mA
	ADF4351_ICP_156		= 4,		//1.56 mA
	ADF4351_ICP_188		= 5,		//1.88 mA
	ADF4351_ICP_219		= 6,		//2.19 mA
	ADF4351_ICP_250		= 7,		//2.50 mA
	ADF4351_ICP_281		= 8,		//2.81 mA
	ADF4351_ICP_313		= 9,		//3.13 mA
	ADF4351_ICP_344		=10,		//3.44 mA
	ADF4351_ICP_375		=11,		//3.75 mA
	ADF4351_ICP_406		=12,		//4.06 mA
	ADF4351_ICP_438		=13,		//4.38 mA
	ADF4351_ICP_469		=14,		//4.69 mA
	ADF4351_ICP_500		=15,		//5.00 mA
} ADF4351_ICP;

//output-power-level
typedef enum ADF4351_POUT {
	ADF4351_POUT_0	= 0,			// -4 dBm
	ADF4351_POUT_1	= 1,			// -1 dBm
	ADF4351_POUT_2	= 2,			// +2 dBm
	ADF4351_POUT_3	= 3,			// +5 dBm
} ADF4351_POUT;

//control latch
typedef struct {
    u16 integer; //integer
    u16 frac;    //frac
} adf4351_reg0;

//control latch
typedef struct {
    u8  prescaler; //integer
    u16 phase;     //phase
    u16 mod;       //modulus
} adf4351_reg1;

//control latch
typedef struct {
    u8  lownoise; //integer
    u8  muxout;   //pin multiplex output
    u8  refdbl;   //reference doubler
    u8  refdiv;   //reference divide by 2
    u16 rcnt;     //reference divider
    u8  dblbuff;  //double buffer
    u8  cpc;      //charge-pump current
    u8  ldf;      //lock detect
    u8  ldp;      //LDP
    u8  pdpol;    //PD polarity
    u8  pd;       //power down
    u8  cpts;     //charge pump tristate
    u8  cntreset; //counter reset
} adf4351_reg2;

//control latch
typedef struct {
    u8  bscm;       //band select clock mode
    u8  abp;
    u8  chcancel;
    u8  csr;        //integer
    u8  clkdivmode; //clock divider mode
    u16 clkdiv;     //clock divider value
} adf4351_reg3;

//control latch
typedef struct {
    u8 fbsel;     //feedback select
    u8 divsel;    //divider select
    u8 bsclkdiv;  //band select clock divider
    u8 vcopd;     //VCO power down
    u8 mtld;      //mute till lock detect
    u8 auxoutsel; //AUX out select
    u8 auxoutena; //AUX out enable
    u8 auxpwrout; //AUX out power
    u8 rfoutena;  //RF out enable
    u8 rfoutpwr;  //RF out power
} adf4351_reg4;


//control latch
typedef struct {
    u8 ldpinmod; //lock detect pin mode
} adf4351_reg5;

//
//public types
//

//device-context
typedef struct adf4351_ctx {
	BOOL init;

	adf4351_reg0 reg0;
	adf4351_reg1 reg1;
	adf4351_reg2 reg2;
	adf4351_reg3 reg3;
	adf4351_reg4 reg4;
	adf4351_reg5 reg5;

	u32          ref;
    u32          pfd;
	u32          vco_freq;  // kHz
} adf4351_ctx;

//init module
extern void adf4351_init(adf4351_ctx* ctx);
extern int adf4351_set_freq(adf4351_ctx* ctx, u32 freq);
extern BOOL adf4351_get_lock(adf4351_ctx* ctx);
extern void adf4351_set_mode(adf4351_ctx* ctx, BOOL enable);
extern void adf4351_set_tune(adf4351_ctx* ctx, u8 icp, u8 pout, BOOL lownoise);
void check_ADF5341_status(adf4351_ctx* ctx);

extern u8 adf4351_lockstat;

#endif //__ADF4351_H__
