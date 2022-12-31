#include <iostream>
#include <bitset>
using namespace std;

//! Instruction decode & register read
class ID
{
public:
    ID()
    {
        implement = 0; // implement初始化為0
    }

    //* instruction
    bitset<32> INS;

    //* 在當前clock cycle，此stage是否在執行
    bool implement;
};