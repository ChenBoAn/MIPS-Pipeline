#include <iostream>
#include <vector>
#include <bitset>
#include <fstream>
using namespace std;

#define reg_num 32

class Register_File
{
public:
    Register_File()
    {
        //* 將容器(reg)大小，使其大小調整為reg_num
        reg.resize(reg_num); 

        //* $0暫存器的初始值為0
        reg[0] = bitset<32>(0);

        //* 其他暫存器的初始值為1
        for (int i = 1; i < reg_num; i++)
        {
            reg[i] = bitset<32>(1);
        }
    }

    //* 根據 read_register: rs、rt 的code解碼(轉十進位)後，讀取該暫存器data
    bitset<32> read(bitset<5> reg_code)
    {
        reg_data = reg[reg_code.to_ulong()];
        return reg_data;
    }

    //* 根據 write_register: rd[R-type] / rt[I-type] 的code解碼(轉十進位)後，將write_data寫入該暫存器
    void write(bitset<5> reg_code, bitset<32> write_data)
    {
        reg[reg_code.to_ulong()] = write_data;
    }

    //* 輸出register的state
    void save_file()
    {
        ofstream file("E:/Computer_Organization/Project/Pipeline/Stall/Example7/register_result.txt", ios_base::out | ios_base::app);
        for (int i = 0; i < reg_num; i++)
        {
            file << "$" << i << ": " << reg[i] << " " << bitset<32>(reg[i]).to_ulong() << endl;
        }
        file.close();
    }

private:
    vector<bitset<32>> reg; // register為保留字
    bitset<32> reg_data;
};