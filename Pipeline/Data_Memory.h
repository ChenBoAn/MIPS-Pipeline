#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
using namespace std;

//* 32words = 128bytes
//* 將每個data(32bits=4bytes)的每個byte存成一行，因此mem_size設成128
//* 記憶體是以byte為基本單位，這樣也能符合 PC = PC + 4
#define mem_size 128

class Data_Memory
{
public:
    Data_Memory()
    {
        //* 將容器(data_mem)大小，使其大小調整為mem_size，並先將全部初始化為0
        data_mem.resize(mem_size, bitset<8>(0)); 

        //* 每個word(4bytes)初始化為1
        for (int i = 3; i < mem_size; i += 4)
        {
            data_mem[i] = bitset<8>(1);
        }
    }

    //* 根據input: address抓取[address ~ address+3]的data_mem中的bits，組合成一個完整32bits的data
    //* 用於lw
    bitset<32> read(bitset<32> address)
    {
        string data;
        for (int k = 0; k < 4; k++)
        {
            data.append(data_mem[address.to_ulong() + k].to_string());
        }
        read_data = bitset<32>(data);

        return read_data;
    }

    //* 根據address，將write_data每個byte寫入data_mem
    //* 用於sw
    void write(bitset<32> address, bitset<32> write_data)
    {
        for (int k = 0; k < 4; k++)
        {
            data_mem[address.to_ulong() + k] = bitset<8>(write_data.to_string().substr(k * 8, 8));
        }
    }

    //* 輸出memory的state(word為單位)
    void save_file()
    {
        ofstream file("E:/Computer_Organization/Project/Pipeline/Stall/Example7/memory_result.txt", ios_base::out | ios_base::app);
        int words = mem_size / 4;
        for (int i = 0; i < words; i++)
        {
            string result;
            for (int j = 0; j < 4; j++)
            {
                result.append(data_mem[i * 4 + j].to_string());
            }
            file << "W" << i << ": " << result << " " << bitset<32>(result).to_ulong() << endl;
        }
        file.close();
    }

private:
    vector<bitset<8>> data_mem; // 以byte(8bits)為單位
    bitset<32> read_data;
};