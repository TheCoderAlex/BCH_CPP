/*
 * @Author: Zifeng.Tang 
 * @Date: 2023-03-01 18:30:15 
 * @Last Modified by: Zifeng.Tang
 * @Last Modified time: 2023-03-01 23:13:43
 */
#pragma once
#include <stdint.h>
#include <utility>

//BCH(26,16)
class BCH {
    private:
        uint32_t cx;    //16bits
        uint32_t gx;    
        uint32_t rx;    //10bits
        uint32_t code;

        std::pair<uint32_t, uint32_t> table[26]; //pair<RX,EX>

    public:
        BCH(int c = 0, int g = 0);
        ~BCH();
        uint32_t get_RX();
        uint32_t get_Code() const;
        int Decode();
        int Encode();
        void Recv_code(uint32_t recv);
        void Show_code() const;
        void Correct();
        void Check();
};
