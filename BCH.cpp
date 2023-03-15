/*
 * @Author: Zifeng.Tang 
 * @Date: 2023-03-01 18:30:15 
 * @Last Modified by: Zifeng.Tang
 * @Last Modified time: 2023-03-01 23:13:51
 */
#include "BCH.hpp"
#include <iostream>

BCH::BCH(int c, int g) {
    //init table
    //for all possible E(X)
    //if ex < 1000000000B rx = ex
    gx = g;
    cx = 1 << 15;
    for (int i = 0; i < 16;++i) {
        get_RX();
        table[i].first = rx;
        table[i].second = cx << 10;
        cx >>= 1;
    }
    cx = 1 << 9;    //rx = ex
    for (int i = 0; i < 10; ++i) {
        table[i + 16].first = cx;
        table[i + 16].second = cx;
        cx >>= 1;
    }
    rx = 0;

    cx = c;
    rx = code = 0;
}

BCH::~BCH() {}

//x^r * M(x) mod G(x)
uint32_t BCH::get_RX() {
    if (cx == 0 || gx == 0)
        return 0;
    uint32_t calc_cx = (cx << 10);
    int tmp = calc_cx;
    for (int i = 0; i < 16; ++i)
    {
        if (calc_cx & 0x2000000)    //0x2000000 == 0010 0000 0000 0000 0000 0000 0000
            calc_cx ^= gx;
        calc_cx <<= 1;
    }
    rx = calc_cx >> 16;
    return rx;
}

uint32_t BCH::get_Code() const{
    return code;
}

int BCH::Decode() {
    if (code == 0 || gx == 0)
        return -1;
    uint32_t calc_code = code;
    for (int i = 0; i < 16; ++i) {
        if (calc_code & 0x2000000)
            calc_code ^= gx;
        calc_code <<= 1;
    }
    calc_code >>= 16;
    //std::cout << "!" << std::hex << calc_code << std::endl;
    return calc_code;
}

int BCH::Encode() {
    if (cx == 0 || gx == 0)
        return -1;
    get_RX();
    code = ((cx << 10) | rx);
    return code;
}

void BCH::Recv_code(uint32_t recv) {
    code = recv;
}

void BCH::Show_code() const {
    std::cout << "Code: " << std::hex << code;
}

void BCH::Correct() {
    if (code == 0 || gx == 0)
        return;
    uint32_t calc_code = code;
    for (int i = 0; i < 16; ++i) {
        if (calc_code & 0x2000000)
            calc_code ^= gx;
        calc_code <<= 1;
    }
    uint32_t ans = calc_code >> 16;
    if (ans == 0)
        return;
    
    for (int i = 0; i < 26;++i) {
        if (ans == table[i].first) {
            code ^= table[i].second;
            return;
        }
    }

    for (int i = 0; i < 26;++i) {
        for (int j = i + 1; j < 26;j++) {
            uint32_t w1 = table[i].first;
            uint32_t w2 = table[j].first;
            if (ans == (w1 ^ w2)) {
                code = code ^ table[i].second ^ table[j].second;
                return;
            }
        }
    }
}

void BCH::Check() {
    Show_code();
    if (Decode() != 0)
        std::cout << " Wrong!" << std::endl;
    else
        std::cout << " Accept!" << std::endl;
}
