#ifndef SHA256_H
#define SHA256_H

#include <iostream>
#include <string>

#define uchar unsigned char
#define uint unsigned int

typedef struct {
    uchar data[64];
    uint datalen;
    uint bitlen[2];
    uint state[8];
} SHA256_CTX;

//void SHA256Transform(SHA256_CTX* ctx, uchar data[]);
//void SHA256Init(SHA256_CTX* ctx);
//void SHA256Update(SHA256_CTX* ctx, uchar data[], uint len);
//void SHA256Final(SHA256_CTX* ctx, uchar hash[]);
std::string SHA256(char* data);

#endif
