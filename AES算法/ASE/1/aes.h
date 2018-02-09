#ifndef AES_H
#define AES_H

extern void aesInit( unsigned char * tempbuf );
extern void aesDecrypt(unsigned char *buffer, unsigned char *chainBlock);
extern void aesEncrypt( unsigned char * buffer, unsigned char * chainBlock );

extern void aesInit( unsigned char * tempbuf );
extern void aesDecrypt( unsigned char * buffer, unsigned char * chainBlock );
extern void aesEncrypt( unsigned char * buffer, unsigned char * chainBlock );

extern unsigned char aesBlockDecrypt(bit Direct,unsigned char *ChiperDataBuf,unsigned char DataLen);




#endif // AES_H
