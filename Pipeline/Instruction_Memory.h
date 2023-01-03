#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
using namespace std;

//* 32words = 128bytes
//* 將每個instruction(32bits=4bytes)的每個byte存成一行，因此mem_size設成128
//* 記憶體是以byte為基本單位，這樣也能符合 PC = PC + 4
#define mem_size 128

class Instruction_Memory
{
public:
    Instruction_Memory()
    {
        //* 將容器(ins_mem)大小，使其大小調整為mem_size
        ins_mem.resize(mem_size);

        //* 讀取asm.txt
        string asm_ins;
        vector<string> myString;
        ifstream inFile;
        inFile.open("E:/Computer_Organization/Project/Pipeline/Stall/Example1/asm.txt");
        if (inFile.is_open())
        {
            //* Read until no more lines in text file to read
            while (getline(inFile, asm_ins))
            {
                istringstream ss(asm_ins);
                string token;

                //* Separate string based on commas and white spaces
                getline(ss, token, ' ');
                myString.push_back(token);

                while (getline(ss, token, ','))
                {
                    ss.ignore();
                    myString.push_back(token);
                }
            }
        }
        inFile.close();

        ofstream outFile("E:/Computer_Organization/Project/Pipeline/Stall/Example1/input.txt");
        for (int i = 0; i < myString.size(); i++)
        {
            string encode;
            bitset<5> b5;
            bitset<16> b16;
            if (myString[i] == "add" || myString[i] == "sub") // R-type opCode : 000000
            {
                string opCode = "000000";

                //* determine funct
                string funct;
                myString[i] == "add" ? funct = "100000" : funct = "100010";

                //* fetch and encode register number
                i++;
                myString[i].erase(myString[i].begin());
                b5 = stoi(myString[i]);
                string rd = b5.to_string();

                i++;
                myString[i].erase(myString[i].begin());
                b5 = stoi(myString[i]);
                string rs = b5.to_string();

                i++;
                myString[i].erase(myString[i].begin());
                b5 = stoi(myString[i]);
                string rt = b5.to_string();

                //* append encoded code to variable
                encode.append(opCode);
                encode.append(rs);
                encode.append(rt);
                encode.append(rd);
                encode.append("00000"); // shamt
                encode.append(funct);
            }
            else if (myString[i] == "lw" || myString[i] == "sw")
            {
                //* determine opcode
                string opCode;
                myString[i] == "lw" ? opCode = "100011" : opCode = "101011";

                //* fecth and encode register number (rt)
                i++;
                myString[i].erase(myString[i].begin());
                b5 = stoi(myString[i]);
                string rt = b5.to_string();

                //* encode address
                i++;
                b16 = stoi(myString[i].substr(0, myString[i].find("(")));
                string address = b16.to_string();

                //* encode rs
                string sub = myString[i].substr(myString[i].find("(") + 1, myString[i].length());
                sub.erase(sub.begin());
                sub.pop_back();
                b5 = stoi(sub);
                string rs = b5.to_string();

                //* append encoded code to variable
                encode.append(opCode); // opcode
                encode.append(rs);
                encode.append(rt);
                encode.append(address);
            }
            else if (myString[i] == "beq") // opcode : 000100
            {
                string opcode = "000100";

                //* fetch and encode rs, rt register number
                i++;
                myString[i].erase(myString[i].begin());
                b5 = stoi(myString[i]);
                string rs = b5.to_string();

                i++;
                myString[i].erase(myString[i].begin());
                b5 = stoi(myString[i]);
                string rt = b5.to_string();

                //* fetch and encode address
                i++;
                b16 = stoi(myString[i]);
                string address = b16.to_string();

                //* append opcode to variable
                encode.append(opcode);
                encode.append(rs);
                encode.append(rt);
                encode.append(address);
            }
            for (int j = 0; j < encode.length(); j += 8)
            {
                for (int k = 0; k < 8; k++)
                {
                    outFile << encode[j + k];
                }
                outFile << endl;
            }
        }
        //* append a halt instruction automatically
        for (int k = 0; k < 4; k++)
        {
            outFile << "11111111\n";
        }
        outFile.close();

        //* 讀取input.txt
        ifstream file("E:/Computer_Organization/Project/Pipeline/Stall/Example1/input.txt");
        string line;
        int i = 0;
        while (getline(file, line))
        {
            ins_mem[i] = bitset<8>(line);
            i++;
        }
        file.close();
    }

    //* 根據PC抓取[PC ~ PC+3]的ins_mem中的bits，組合成一個完整32bits的instruction
    bitset<32> read(bitset<32> PC)
    {
        string ins;
        for (int k = 0; k < 4; k++)
        {
            ins.append(ins_mem[PC.to_ulong() + k].to_string());
        }
        instruction = bitset<32>(ins);

        return instruction;
    }

private:
    vector<bitset<8>> ins_mem; // 以byte(8bits)為單位
    bitset<32> instruction;
};