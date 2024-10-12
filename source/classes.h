#pragma once
#include <cstdio>
#include <cstdint>
#include <Windows.h>
#include <thread>
#include <math.h>

class Vector3
{
public:
    float x;
    float y;
    float z;
};

class Player
{
public:
    void* vtable;
    Vector3 o; //0x0004
    char pad_0010[24]; //0x0010
    Vector3 pos; //0x0028
    float yaw; //0x0034
    float pitch; //0x0038
    float roll; //0x003C
    char pad_0040[172]; //0x0040
    int32_t health; //0x00EC
    char pad_00F0[864]; //0x00F0
    int32_t team; //0x030C

}; //Size: 0x0450

//class EntityList
//{
//public:
//    Player* players[64];
//};