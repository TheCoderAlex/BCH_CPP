/*
 * @Author: Zifeng.Tang 
 * @Date: 2023-03-01 18:30:15 
 * @Last Modified by: Zifeng.Tang
 * @Last Modified time: 2023-03-01 23:14:05
 */
#include <iostream>
#include "BCH.hpp"
using namespace std;

const uint32_t GX = 0x05b9; // 10110111001

int main()
{
    const uint32_t mycx = 0xfcdf;   //1111110011011111
    const uint32_t mygx = GX << 15; 
    BCH a(mycx, mygx);
    a.Encode();
    a.Check();

    uint32_t wrong1 = 0x220000;     //00001000100000000000000000B
    uint32_t tmp = a.get_Code();
    tmp = tmp ^ wrong1;
    a.Recv_code(tmp);
    a.Check();

    a.Correct();

    cout << "Now The Decode is " << a.Decode() << endl;
    a.Check();

    return 0;
}