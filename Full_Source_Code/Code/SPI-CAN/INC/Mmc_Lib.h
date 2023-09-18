#ifndef __MMC_LIB_H_
#define __MMC_LIB_H_


extern unsigned char Mmc_Init(unsigned long *port, unsigned char cspin);
extern unsigned char Mmc_Read_Sector(unsigned long sector, char *dbuff);
extern unsigned char Mmc_Write_Sector(unsigned long sector, char *dbuff);

extern unsigned char Mmc_Read_Cid(char *data_cid);
extern unsigned char Mmc_Read_Csd(char *data_csd);


#endif
