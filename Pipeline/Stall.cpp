#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>

#include "State.h"
#include "Instruction_Memory.h"
#include "Register_File.h"
#include "Data_Memory.h"

using namespace std;

void save_state(int cycle, State state)
{
    ofstream file("E:/Computer_Organization/Project/Pipeline/Stall/Example1/state.txt", ios_base::out | ios_base::app);

    file << "Cycle " << cycle << endl;

    file << "\n[IF_stage]" << endl;
    file << "instruction_name:\t" << state.IF_stage.ins_name << endl;
    file << "implement:\t" << state.IF_stage.implement << endl;
    file << "PC:\t" << state.IF_stage.PC.to_ulong() << endl;

    file << "\n[ID_stage]" << endl;
    file << "instruction_name:\t" << state.ID_stage.ins_name << endl;
    file << "implement:\t" << state.ID_stage.implement << endl;
    file << "ins:\t" << state.ID_stage.ins << endl;

    file << "\n[EX_stage]" << endl;
    file << "instruction_name:\t" << state.EX_stage.ins_name << endl;
    file << "implement:\t" << state.EX_stage.implement << endl;
    file << "rs:\t" << state.EX_stage.rs << endl;
    file << "rt:\t" << state.EX_stage.rt << endl;
    file << "Write_reg:\t" << state.EX_stage.Write_reg << endl;
    file << "I_address:\t" << state.EX_stage.I_address << endl;
    file << "Read_data1:\t" << state.EX_stage.Read_data1 << endl;
    file << "Read_data2:\t" << state.EX_stage.Read_data2 << endl;
    file << "ALUOp:\t" << state.EX_stage.ALUOp << endl;
    file << "type:\t" << state.EX_stage.type << endl;
    file << "RegDst:\t" << state.EX_stage.RegDst << endl;
    file << "ALUSrc:\t" << state.EX_stage.ALUSrc << endl;
    file << "Branch:\t" << state.EX_stage.Branch << endl;
    file << "MemRead:\t" << state.EX_stage.MemRead << endl;
    file << "MemWrite:\t" << state.EX_stage.MemWrite << endl;
    file << "RegWrite:\t" << state.EX_stage.RegWrite << endl;
    file << "MemtoReg:\t" << state.EX_stage.MemtoReg << endl;

    file << "\n[MEM_stage]" << endl;
    file << "instruction_name:\t" << state.MEM_stage.ins_name << endl;
    file << "implement:\t" << state.MEM_stage.implement << endl;
    file << "rs:\t" << state.MEM_stage.rs << endl;
    file << "rt:\t" << state.MEM_stage.rt << endl;
    file << "Write_reg:\t" << state.MEM_stage.Write_reg << endl;
    file << "ALU_result:\t" << state.MEM_stage.ALU_result << endl;
    file << "Write_data:\t" << state.MEM_stage.Write_data << endl;
    file << "Branch:\t" << state.MEM_stage.Branch << endl;
    file << "MemRead:\t" << state.MEM_stage.MemRead << endl;
    file << "MemWrite:\t" << state.MEM_stage.MemWrite << endl;
    file << "RegWrite:\t" << state.MEM_stage.RegWrite << endl;
    file << "MemtoReg:\t" << state.MEM_stage.MemtoReg << endl;

    file << "\n[WB_stage]" << endl;
    file << "instruction_name:\t" << state.WB_stage.ins_name << endl;
    file << "implement:\t" << state.WB_stage.implement << endl;
    file << "rs:\t" << state.WB_stage.rs << endl;
    file << "rt:\t" << state.WB_stage.rt << endl;
    file << "Write_reg:\t" << state.WB_stage.Write_reg << endl;
    file << "Write_data:\t" << state.WB_stage.Write_data << endl;
    file << "RegWrite:\t" << state.WB_stage.RegWrite << endl;
    file << "MemtoReg:\t" << state.WB_stage.MemtoReg << endl;

    file << endl;
    file.close();
}

void output(int cycle, State state)
{
    ofstream file("E:/Computer_Organization/Project/Pipeline/Stall/Example1/ouput.txt", ios_base::out | ios_base::app);

    file << "Cycle " << cycle << endl;

    if (state.WB_stage.implement)
    {
        file << state.WB_stage.ins_name << ": ";
        file << "WB ";

        file << state.WB_stage.RegWrite << " ";
        file << state.WB_stage.MemtoReg << endl;
    }

    if (state.MEM_stage.implement)
    {
        file << state.MEM_stage.ins_name << ": ";
        file << "MEM ";

        file << state.MEM_stage.Branch << " ";
        file << state.MEM_stage.MemRead << " ";
        file << state.MEM_stage.MemWrite << " ";
        file << state.MEM_stage.RegWrite << " ";
        file << state.MEM_stage.MemtoReg << endl;
    }

    if (state.EX_stage.implement)
    {
        file << state.EX_stage.ins_name << ": ";
        file << "EX ";

        file << state.EX_stage.RegDst << " ";
        file << state.EX_stage.ALUSrc << " ";
        file << state.EX_stage.Branch << " ";
        file << state.EX_stage.MemRead << " ";
        file << state.EX_stage.MemWrite << " ";
        file << state.EX_stage.RegWrite << " ";
        file << state.EX_stage.MemtoReg << endl;
    }

    if (state.ID_stage.implement)
    {
        file << state.ID_stage.ins_name << ": ";
        file << "ID" << endl;
    }

    if (state.IF_stage.implement)
    {
        file << state.IF_stage.ins_name << ": ";
        file << "IF" << endl;
    }

    file << endl;
    file.close();
}

//* 向右位移 n bits
unsigned long shift_bits(bitset<32> ins, int start)
{
    return ((ins.to_ulong()) >> start); //* 需轉成int才能位移
}

//* 將16bits sign-extend 成32bits
bitset<32> sign_extend(bitset<16> i_address)
{
    string extended_address;
    //* 若最大位數為0，則extend 16個0
    if (i_address[15] == 0)
    {
        extended_address = "0000000000000000" + i_address.to_string<char, std::string::traits_type, std::string::allocator_type>();
    }
    else
    {
        extended_address = "1111111111111111" + i_address.to_string<char, std::string::traits_type, std::string::allocator_type>();
    }
    return (bitset<32>(extended_address));
}

int main()
{
    Register_File RF;
    Instruction_Memory IM;
    Data_Memory DM;

    State current_state;
    State next_state;

    int cycle = 1;

    while (true)
    {
        //! IF Stage: Instruction fetch from memory
        //* IF Stage執行時
        if (current_state.IF_stage.implement)
        {
            ///* 透過IF的PC 讀取instruction memory(IM)的指令
            bitset<32> instruction = IM.read(current_state.IF_stage.PC);
            next_state.ID_stage.ins = instruction;

            //* 先向右位移  透過bitset 只存取最右邊6bits
            bitset<6> opcode = bitset<6>(shift_bits(instruction, 26));
            bitset<6> funct = bitset<6>(shift_bits(instruction, 0));

            //* 判斷指令名稱
            if (!opcode.to_ulong()) // R-type
            {
                if (funct.to_ulong() == 32) // add
                {
                    current_state.IF_stage.ins_name = "add";
                }
                else // sub
                {
                    current_state.IF_stage.ins_name = "sub";
                }
            }
            else if (opcode.to_ulong() == 35) // lw
            {
                current_state.IF_stage.ins_name = "lw";
            }
            else if (opcode.to_ulong() == 43) // sw
            {
                current_state.IF_stage.ins_name = "sw";
            }
            else // beq
            {
                current_state.IF_stage.ins_name = "beq";
            }

            //* 32bits個1的指令表示結束
            if (instruction.to_string<char, std::string::traits_type, std::string::allocator_type>() == "11111111111111111111111111111111")
            {
                next_state.IF_stage.implement = current_state.IF_stage.implement = 0;
                next_state.IF_stage.PC = current_state.IF_stage.PC;
            }
            else
            {
                //* PC = PC + 4
                next_state.IF_stage.PC = bitset<32>(current_state.IF_stage.PC.to_ulong() + 4);
            }
            
            next_state.ID_stage.ins_name = current_state.IF_stage.ins_name;
            next_state.ID_stage.implement = current_state.IF_stage.implement;
        }
        else
        {
            next_state.ID_stage.implement = current_state.IF_stage.implement;
        }

        //! ID Stage: Instruction decode & register read
        //* ID Stage執行時
        if (current_state.ID_stage.implement)
        {
            bitset<6> opcode = bitset<6>(shift_bits(current_state.ID_stage.ins, 26));
            bitset<6> funct = bitset<6>(shift_bits(current_state.ID_stage.ins, 0));

            bitset<5> reg1 = bitset<5>(shift_bits(current_state.ID_stage.ins, 21));
            bitset<5> reg2 = bitset<5>(shift_bits(current_state.ID_stage.ins, 16));
            next_state.EX_stage.rs = reg1;
            next_state.EX_stage.rt = reg2;
            next_state.EX_stage.I_address = bitset<16>(shift_bits(current_state.ID_stage.ins, 0));

            //* 若為R-type指令: write_register = rd
            if (opcode.to_ulong() == 0)
            {
                next_state.EX_stage.Write_reg = bitset<5>(shift_bits(current_state.ID_stage.ins, 11));
                next_state.EX_stage.type = 0;
            }
            else //* 反之為I-type指令: write_register = rt
            {
                next_state.EX_stage.Write_reg = reg2;
                next_state.EX_stage.type = 1;
            }

            //* ALU input
            next_state.EX_stage.Read_data1 = RF.read(reg1);
            next_state.EX_stage.Read_data2 = RF.read(reg2);

            //* lw、sw、add -> add(1)，sub -> sub(0)
            if (opcode.to_ulong() == 35 || opcode.to_ulong() == 43 || funct.to_ulong() == 32)
            {
                next_state.EX_stage.ALUOp = 1;
            }
            else
            {
                next_state.EX_stage.ALUOp = 0;
            }

            //* Control Sign
            if (opcode.to_ulong() == 43 || opcode.to_ulong() == 4) // sw、beq
            {
                next_state.EX_stage.RegDst = 'X';
                next_state.EX_stage.MemtoReg = 'X';
                next_state.EX_stage.RegWrite = 0;
            }
            else if (opcode.to_ulong() == 35) // lw
            {
                next_state.EX_stage.RegDst = '0';
                next_state.EX_stage.MemtoReg = '1';
                next_state.EX_stage.RegWrite = 1;
            }
            else // R-type
            {
                next_state.EX_stage.RegDst = '1';
                next_state.EX_stage.MemtoReg = '0';
                next_state.EX_stage.RegWrite = 1;
            }

            next_state.EX_stage.ALUSrc = (opcode.to_ulong() == 35 || opcode.to_ulong() == 43) ? 1 : 0;
            next_state.EX_stage.Branch = (opcode.to_ulong() == 4) ? 1 : 0;
            next_state.EX_stage.MemRead = (opcode.to_ulong() == 35) ? 1 : 0;
            next_state.EX_stage.MemWrite = (opcode.to_ulong() == 43) ? 1 : 0;

            next_state.EX_stage.implement = current_state.ID_stage.implement;
            next_state.EX_stage.ins_name = current_state.ID_stage.ins_name;
            //* Stall (Write_reg: R-type = rd、lw = rt)
            if (current_state.EX_stage.implement)
            {
                cout << cycle << endl;
                cout << "1-1" << endl;
                if ((bitset<6>(shift_bits(current_state.ID_stage.ins, 26))).to_ulong() == 0) //* R-type: opcode=0
                {
                    cout << "1-2-1" << endl;
                    //* 當 Write_reg = R-type的rs/rt，若還沒更新暫存器，R-type指令便無法從rs和rt取得正確資料
                    if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 21))) // rs
                    {
                        cout << "1-3-1" << endl;
                        next_state.ID_stage = current_state.ID_stage; // 維持在ID stage
                    }
                    if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // rt
                    {
                        cout << "1-3-2" << endl;
                        next_state.ID_stage = current_state.ID_stage; // 維持在ID stage
                    }
                }
                else //* I-type
                {
                    cout << "1-2-2" << endl;
                    //* 當 Write_reg = I-type的rs，若還沒更新暫存器，I-type指令便無法從rs取得正確資料與sign_extend後的I-address做相加，來取得data_memory address
                    if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 21))) // rs
                    {
                        cout << "1-3-3" << endl;
                        next_state.ID_stage = current_state.ID_stage; // 維持在ID stage
                    }
                }
            }
        }
        else
        {
            next_state.EX_stage.implement = current_state.ID_stage.implement;
        }

        //! EX Stage: Execute operation or calculate address
        //* EX Stage執行時
        if (current_state.EX_stage.implement)
        {
            //* 執行ALU，減法(0): sub、加法(1): add、lw、sw
            //* 依指令更新ALU_result[RegWrite: 0=sw、beq，1=R-type、lw]
            if (!current_state.EX_stage.ALUSrc && !current_state.EX_stage.type && current_state.EX_stage.ALUOp) // add
            {
                next_state.MEM_stage.ALU_result = bitset<32>(current_state.EX_stage.Read_data1.to_ulong() + current_state.EX_stage.Read_data2.to_ulong());
            }
            else if (!current_state.EX_stage.ALUSrc && !current_state.EX_stage.type && !current_state.EX_stage.ALUOp) // sub
            {
                next_state.MEM_stage.ALU_result = bitset<32>(current_state.EX_stage.Read_data1.to_ulong() - current_state.EX_stage.Read_data2.to_ulong());
            }
            else if (current_state.EX_stage.ALUSrc && current_state.EX_stage.type && current_state.EX_stage.ALUOp) // lw、sw
            {
                next_state.MEM_stage.ALU_result = bitset<32>(current_state.EX_stage.Read_data1.to_ulong() + sign_extend(current_state.EX_stage.I_address).to_ulong());
            }
            else // TODO beq
            {
                next_state.MEM_stage.ALU_result = bitset<32>(0);
            }

            //* 更新下一Stage資訊
            next_state.MEM_stage.rs = current_state.EX_stage.rs;
            next_state.MEM_stage.rt = current_state.EX_stage.rt;
            next_state.MEM_stage.Write_reg = current_state.EX_stage.Write_reg;

            next_state.MEM_stage.Write_data = current_state.EX_stage.Read_data2;

            next_state.MEM_stage.Branch = current_state.EX_stage.Branch;
            next_state.MEM_stage.MemRead = current_state.EX_stage.MemRead;
            next_state.MEM_stage.MemWrite = current_state.EX_stage.MemWrite;
            next_state.MEM_stage.RegWrite = current_state.EX_stage.RegWrite;
            next_state.MEM_stage.MemtoReg = current_state.EX_stage.MemtoReg;

            next_state.MEM_stage.implement = current_state.EX_stage.implement;
            next_state.MEM_stage.ins_name = current_state.EX_stage.ins_name;
            //* Stall (Write_reg: R-type = rd、lw = rt)
            if (current_state.EX_stage.implement)
            {
                cout << cycle << endl;
                cout << "2-1" << endl;
                if ((bitset<6>(shift_bits(current_state.ID_stage.ins, 26))).to_ulong() == 0) //* R-type: opcode=0
                {
                    cout << "2-2-1" << endl;
                    //* 當 Write_reg = R-type的rs/rt，若還沒更新暫存器，R-type指令便無法從rs和rt取得正確資料
                    if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 21))) // rs
                    {
                        cout << "2-3-1" << endl;
                        next_state.EX_stage.implement = 0; // 維持在ID stage
                    }
                    if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // rt
                    {
                        cout << "2-3-2" << endl;
                        next_state.EX_stage.implement = 0; // 維持在ID stage
                    }
                }
                else //* I-type
                {
                    cout << "2-2-2" << endl;
                    //* 當 Write_reg = I-type的rs，若還沒更新暫存器，I-type指令便無法從rs取得正確資料與sign_extend後的I-address做相加，來取得data_memory address
                    if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 21))) // rs
                    {
                        cout << "2-3-3" << endl;
                        next_state.EX_stage.implement = 0; // 維持在ID stage
                    }
                }
            }
        }
        else
        {
            next_state.MEM_stage.implement = current_state.EX_stage.implement;
        }

        //! MEM Stage: Access memory operand
        //* MEM Stage執行時
        if (current_state.MEM_stage.implement)
        {
            //* 依指令更新Write_data及Data_memory
            if (current_state.MEM_stage.MemRead) //* lw
            {
                next_state.WB_stage.Write_data = DM.read(current_state.MEM_stage.ALU_result); // load data
            }
            else if (current_state.MEM_stage.MemWrite) //* sw
            {
                DM.write(current_state.MEM_stage.ALU_result, current_state.MEM_stage.Write_data); // store data
                next_state.WB_stage.Write_data = current_state.WB_stage.Write_data;               // MemtoReg = don't care，不會Write Back回暫存器，因此Write_data保持不變即可
            }
            else if (current_state.MEM_stage.Branch) //* beq
            {
                next_state.WB_stage.Write_data = current_state.WB_stage.Write_data; // MemtoReg = don't care，不會Write Back回暫存器，因此Write_data保持不變即可
            }
            else //* R-type
            {
                next_state.WB_stage.Write_data = current_state.MEM_stage.ALU_result; // 要寫回暫存器之資料，即為ALU運算結果
            }

            //* 更新下一Stage資訊
            next_state.WB_stage.rs = current_state.MEM_stage.rs;
            next_state.WB_stage.rt = current_state.MEM_stage.rt;
            next_state.WB_stage.Write_reg = current_state.MEM_stage.Write_reg;

            next_state.WB_stage.RegWrite = current_state.MEM_stage.RegWrite;
            next_state.WB_stage.MemtoReg = current_state.MEM_stage.MemtoReg;

            next_state.WB_stage.implement = current_state.MEM_stage.implement;
            next_state.WB_stage.ins_name = current_state.MEM_stage.ins_name;
        }
        else
        {
            next_state.WB_stage.implement = current_state.MEM_stage.implement;
        }

        //! WB Stage: Write result back to register
        //* WB Stage執行時
        if (current_state.WB_stage.implement)
        {
            //* 更新Register File
            if (current_state.WB_stage.RegWrite) // R-type、lw
            {
                RF.write(current_state.WB_stage.Write_reg, current_state.WB_stage.Write_data);
            }
        }

        //! 當所有Stage不執行時，即為結束
        if (!current_state.IF_stage.implement && !current_state.ID_stage.implement && !current_state.EX_stage.implement && !current_state.MEM_stage.implement && !current_state.WB_stage.implement)
        {
            break;
        }

        //* 儲存每個cycle結束後的狀態
        save_state(cycle, current_state);
        output(cycle, current_state);

        //* 進入下一cycle，並更新當前狀態
        cycle = cycle + 1;
        current_state = next_state;
    }

    //* 儲存 register result
    RF.save_file();

    //* 儲存 memory result
    DM.save_file();

    return 0;
}