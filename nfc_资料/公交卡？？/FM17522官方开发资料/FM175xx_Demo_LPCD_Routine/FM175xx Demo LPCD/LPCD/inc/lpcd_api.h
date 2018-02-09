#ifndef LPCD_API_H
#define LPCD_API_H

extern unsigned char LpcdParamInit(void);
extern unsigned char LpcdRegisterInit(void);
extern unsigned char LpcdInitCalibra(unsigned char *CalibraFlag);
extern unsigned char ADCResultCenter;		//LPCDÂö¿íÖÐÐÄµã
extern void LpcdCardIn_IRQHandler(void);
extern void LpcdAutoWakeUp_IRQHandler(void);
//extern unsigned char LpcdAuxSelect(unsigned char OpenClose);
//extern unsigned char LpcdAutoTest(void);

#endif


