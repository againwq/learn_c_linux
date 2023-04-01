#include <stdio.h>
#include <arpa/inet.h>

#define BITS16 16
#define BITS32 32

typedef union 
{
    unsigned long int value;
    unsigned char byte[2];
} to16;


typedef union
{
    unsigned long int value;
    unsigned char byte[4];
} to32;

void showvalue(unsigned char *begin, int flag){
    int num = 0, i = 0;
    if(flag == BITS16){
        num = 2;
    }else if(flag == BITS32){
        num = 4;
    }
    for(i = 0; i < num; i++){
        printf("%x", *(begin + i));
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    to16 v16_orig, v16_turn1, v16_turn2;
    to32 v32_orig, v32_turn1, v32_turn2;
    v16_orig.value = 0xabcd;
    v16_turn1.value = htons(v16_orig.value); //htons: 主机字节序到网络字节序的短整型转换，反过来用ntohs
    v16_turn2.value = htons(v16_turn1.value); 

    v32_orig.value = 0x12345678;
    v32_turn1.value = htonl(v32_orig.value);   //htonl: 主机字节序到网络字节序的长整型转换，反过来用ntohl
    v32_turn2.value = htonl(v32_turn1.value);
    printf("16位转换\n");
    showvalue(v16_orig.byte, BITS16);
    showvalue(v16_turn1.byte, BITS16);
    showvalue(v16_turn2.byte, BITS16);
    printf("\n32位转换\n");
    showvalue(v32_orig.byte, BITS32);
    showvalue(v32_turn1.byte, BITS32);
    showvalue(v32_turn2.byte, BITS32);

    return 0;
}

