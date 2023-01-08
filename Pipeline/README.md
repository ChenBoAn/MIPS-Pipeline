計算機組織期末專題
===

# 環境
- Windows 10/11
- Vscode
- C/C++ standard: C11
- C/C++ compiler: g++

# 使用方式
1. 將所有指令寫成一個asm.txt，並放入input資料夾中
2. 執行程式碼後，即會產生input.txt於input資料夾中，output.txt、state.txt、register_result.txt、memory_result.txt於ouput資料夾中

# 專案架構
> * 使用 c++ 模擬具有 pipeline 功能之 cpu
> * 透過 stall 或 forwarding 解決 pipeline 時會發生的所有 hazard 問題
## 輸入、輸出檔案及資料夾說明
- Stall : 存放stall所需所有input和輸出結果
- Forwarding : 存放forwarding所需所有input和輸出結果
- asm.txt : MIPS組合語言
- input.txt : 將組合語言進行編譯，所產生的instructions
- memory_result.txt : 程序結束後，所有記憶體狀態
- output.txt : 按照ppt中的呈現輸出結果方式
- register_result.txt : 程序結束後，所有暫存器狀態
- state.txt : 每個cycle中，所有stages的詳細資訊
## 檔案架構
```
`-- Pipeline
    |-- Stall 
    |   |-- Example1
    |   |   |--asm.txt
    |   |   |--input.txt
    |   |   |--memory_result.txt
    |   |   |--output.txt
    |   |   |--register_result.txt
    |   |   |--state.txt
    |   |-- Example2
    |   |-- Example3
    |   |-- Example4
    |   |-- Example5
    |   |-- Example6
    |   |-- Example7
    |   `-- Example8
    |-- Forwarding
    |   |-- Example1
    |   |-- Example2
    |   |-- Example3
    |   |-- Example4
    |   |-- Example5
    |   |-- Example6
    |   |-- Example7
    |   `-- Example8
    |-- Forwarding.cpp
    |-- Forwarding.exe
    |-- Stall.cpp
    |-- Stall.exe
    |-- Instruction_Memory.h
    |-- Data_Memory.h
    |-- Register_File.h
    |-- State.h
    |-- IF.h
    |-- ID.h
    |-- EX.h
    |-- MEM.h
    `-- WB.h

```
# 程式功能說明
## Main Fuction
### Stall.cpp : 
- 模擬具有stall功能的pipline cpu
- 與Forwarding.cpp共用所有header files
### Forwarding.cpp:
- 模擬具有forwarding功能的pipline cpu
- 與Stall.cpp共用所有header files
## Header Files
> 每個標頭檔皆包含一個與檔名相同的class，供主函式使用
### Instruction_Memory.h
> 指令以 32 bits *(word)* 為單位，instruction memory 以 8 bits *(byte)* 為單位，因此抓取下一個指令時需 **PC = PC + 4**
- 初始化時，針對asm.txt進行編碼
- 讀取指令並對指令進行字串處理(比如空格和逗號)
- 根據指令形成instruction format
- 最後加上32個bits的 1，代表halt指令，程序結束之位置
### Register_File.h
- 讀取或寫入特定暫存器
- 輸出所有暫存器的狀態，寫檔至 *register_result.txt*
### Data_Memory.h
> data memory 以 byte 為單位，資料以 word 為單位，所以在讀取/寫入資料的時候都必須一次處理 4 bytes
- 讀取或寫入記憶體位置之資料
- 輸出所有記憶體的狀態，寫檔至 *memory_result.txt*

### IF.h:
- Instruction fetch
- 儲存當前狀態指令名稱以及是否執行
### ID.h:
- Instructioin decode & Register read
- 儲存當前狀態指令名稱、是否發生stall以及是否執行
### EX.h:
- Execute
- 儲存當前狀態指令名稱以及是否執行
- 儲存instruction field , all input , Opcode , control sign
### MEM.h:
- Memory write
- 儲存當前狀態指令名稱以及是否執行
- 儲存instruction field , memory input , control sign
### WB.h:
- Write back
- 儲存當前狀態指令名稱以及是否執行
- 儲存instruction field , register input , control sign
### State.h:
- data member 包含 class IF , ID , EX , MEM 以及 WB


## 程式碼重點

### 大致流程
> 同一cycle之Stage流程: WB Stage -> EXE Stage -> EX Stage -> ID Stage -> IF Stage
> 為防止當前state之後續Stage的變數被前面Stage所影響、更新，因此倒置
#### Stall 流程
``` c++

State current_state; //紀錄當前狀態有哪些stages須執行，以及每一個stage的資料
State next_state; //下一個要進入的state

while(1){

    /*
    執行需執行的stages
    透過執行結果與flags，來更新 next_state 的內容
    */
    
    /*-------------------WB stage-------------------*/
    //* 更新Register File
    
    /*-------------------MEM stage------------------*/
    //* 依指令更新Write_data及Data_memory:
    // lw: load data(Write_data = DM.read(ALU_result)
    // sw: store data
    // R-type: Write_data = ALU result
    
    /*-------------------EX stage-------------------*/
    //* 依指令更新ALU_result:
    // add: Read_data1 + Read_data2
    // sub: Read_data1 - Read_data2
    // lw、sw: Read_data1 + sign-extended(address)
    
    /*-------------------ID stage-------------------*/
    //* Data Hazard
    //* Control Hazard
    //* instruction decode
    //* control sign
    
    /*-------------------IF stage-------------------*/
    //* 透過IF的PC 讀取instruction memory(IM)的指令:
    // instruction = IM.read(PC)
    //* PC = PC + 4
    /*----------------------------------------------*/
    
    // 當所有Stages都不執行時，即為結束 break
    
    // 儲存每個cycle結束後的狀態
    
    // 進入下一cycle，並更新當前狀態
    cycle = cycle + 1;
    current_state = next_state;
}
```
#### Forwarding 流程
``` c++

State current_state; //紀錄當前狀態有哪些stages須執行，以及每一個stage的資料
State next_state; //下一個要進入的state

while(1){

    /*
    執行需執行的stages
    透過執行結果與flags，來更新 next_state 的內容
    */
    
    /*-------------------WB stage-------------------*/
    //* 更新Register File
    
    /*-------------------MEM stage------------------*/
    //* 依指令更新Write_data及Data_memory:
    // lw: load data(Write_data = DM.read(ALU_result)
    // sw: store data
    // R-type: Write_data = ALU result
    
    /*-------------------EX stage-------------------*/
    //* EX Hazard
    //* MEM Hazard
    //* 依指令更新ALU_result:
    // add: Read_data1 + Read_data2
    // sub: Read_data1 - Read_data2
    // lw、sw: Read_data1 + sign-extended(address)
    
    /*-------------------ID stage-------------------*/
    //* Data Hazard
    //* Control Hazard
    //* instruction decode
    //* control sign
    
    /*-------------------IF stage-------------------*/
    //* 透過IF的PC 讀取instruction memory(IM)的指令:
    // instruction = IM.read(PC)
    //* PC = PC + 4
    /*----------------------------------------------*/
    
    // 當所有Stages都不執行時，即為結束 break
    
    // 儲存每個cycle結束後的狀態
    
    // 進入下一cycle，並更新當前狀態
    cycle = cycle + 1;
    current_state = next_state;
}
```
### hazard處理
#### stall
1. Data Hazard :
    - lw指令在EX Stage或MEM Stage的rt 與 sw指令在ID stage的rt 相同，加入stall
    - lw指令在EX Stage或MEM Stage的rt 與 R-type指令在ID stage的rs、rt 相同，加入stall
    - lw指令在EX Stage或MEM Stage的rt 與 beq指令在ID stage的rt、rt 相同，加入stall
    - R-type指令在EX Stage或MEM Stage的rd 與 sw指令在ID stage的rt 相同，加入stall
    - R-type指令在EX Stage或MEM Stage的rd 與 R-type指令在ID stage的rs、rt 相同，加入stall
    - R-type指令在EX Stage或MEM Stage的rd 與 beq指令在ID stage的rs、rt 相同，加入stall
    ``` =c++
    //* Data Hazard: lw、R-type
    //? EX -> ID
    if (current_state.EX_stage.implement && current_state.EX_stage.RegWrite) // R-type、lw: RegWrite = 1
        //* sw指令
        if (opcode.to_ulong() == 43)
            //* sw指令的rt
            if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // rt

        else if (opcode.to_ulong() == 0) //* R-type指令
            //* R-type指令的rs或rt
            if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 21))) // rs
            if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // rt

        else if (opcode.to_ulong() == 4) //* beq指令
            //* beq指令的rs或rt
            if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 21))) // rs
            if (current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // rt

    //? MEM -> ID
    if (current_state.MEM_stage.implement && current_state.MEM_stage.RegWrite) // R-type、lw: RegWrite = 1
        //* sw指令
        if (opcode.to_ulong() == 43)
            //* sw指令的rt
            if (current_state.MEM_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // rt

        else if (opcode.to_ulong() == 0) //* R-type指令
            //* R-type指令的rs或rt
            if (current_state.MEM_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 21))) // rs
            if (current_state.MEM_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // rt

        else if (opcode.to_ulong() == 4) //* beq指令
            //* beq指令的rs或rt
            if (current_state.MEM_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 21))) // rs
            if (current_state.MEM_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 16))) // rt
    ```

2. Control Hazard : 
    - 若未發生stall且為beq指令時
        - 當其rs = rt，PC = (PC + 4) + [address(16bits) sign-extended to 32bits and shift left 2bits(*4)]
        - 當其rs != rt，PC = PC + 4
    ``` =c++
    //* Control Hazard
    if (!current_state.ID_stage.stall && opcode.to_ulong() == 4) //* beq
    {
        if (RF.read(reg1).to_ulong() == RF.read(reg2).to_ulong()) //* rs = rt
        {
            next_state.ID_stage = current_state.ID_stage; // 維持在ID stage
            bitset<1> sign_bit = bitset<1>(shift_bits(current_state.ID_stage.ins, 15));
            int i_addr = 0;

            //* 判斷正負
            if (sign_bit.to_ulong() == 1)
            {
                //* (-2^15 + 後15bits轉成的整數) * 4
                i_addr = ((-32768) + bitset<15>(shift_bits(current_state.ID_stage.ins, 0)).to_ulong()) * 4;
            }
            else
            {
                //* 後15bits轉成的整數 * 4
                i_addr = bitset<15>(shift_bits(current_state.ID_stage.ins, 0)).to_ulong() * 4;
            }

            //* (PC + 4) + iddr[等於經過sign_extend_32，然後shift_left_2]
            next_state.IF_stage.PC = bitset<32>((current_state.IF_stage.PC.to_ulong()) + i_addr);
        }
    }
    ```
#### forwarding
1. Data Hazard
    - load-use data hazard
    ``` =c++
    if ((current_state.EX_stage.type && current_state.EX_stage.MemRead)) // type=1 -> I-type，MemRead只有lw指令為1
            {
                //* 判斷是否為sw指令，sw的opcode[31~26]為101011->43
                if ((bitset<6>(shift_bits(current_state.ID_stage.ins, 26))).to_ulong() == 43)
                {
                    //* 因為lw為I-type指令，因此write_register為rt
                    //* 當lw還沒從記憶體讀取data，sw便無法從rs取得資料與sign_extend後的I-address做相加，來取得data_memory address
                    if (current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 21)))
                }
                else //* R-type指令
                {
                    //* 當lw還沒從記憶體讀取data，R-type指令便無法從rs和rt取得資料
                    if (current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 21)) && current_state.EX_stage.implement && bitset<6>(shift_bits(current_state.ID_stage.ins, 26)) == bitset<6>("000000"))
                    if (current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 16)) && current_state.EX_stage.implement && bitset<6>(shift_bits(current_state.ID_stage.ins, 26)) == bitset<6>("000000")) // opcode全為0 -> R-type
                }
            }
    ```
    - branch data hazard
    ``` =c++
     //* 前指令(EX階段)為lw 當lw還沒從記憶體讀取data beq指令便無法從rs和rt取得資料
    if (current_state.ID_stage.ins_name == "beq" && current_state.EX_stage.ins_name == "lw" && current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 21)) && current_state.EX_stage.implement)
    else if (current_state.ID_stage.ins_name == "beq" && current_state.EX_stage.ins_name == "lw" && current_state.EX_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 16)) && current_state.EX_stage.implement)
    //* 前指令(EX階段)為add 當add尚未運算 beq指令便無法從rs和rt取得資料
    else if (current_state.ID_stage.ins_name == "beq" && !current_state.EX_stage.type && current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 21)) && current_state.EX_stage.implement)
    else if (current_state.ID_stage.ins_name == "beq" && !current_state.EX_stage.type && current_state.EX_stage.Write_reg == bitset<5>(shift_bits(current_state.ID_stage.ins, 16)) && current_state.EX_stage.implement)
    // 前前指令(MEM階段))當lw還沒從記憶體讀取data beq指令便無法從rs和rt取得資料
    else if (current_state.ID_stage.ins_name == "beq" && current_state.MEM_stage.ins_name == "lw" && current_state.MEM_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 21)) && current_state.MEM_stage.implement)
    else if (current_state.ID_stage.ins_name == "beq" && current_state.MEM_stage.ins_name == "lw" && current_state.MEM_stage.rt == bitset<5>(shift_bits(current_state.ID_stage.ins, 16)) && current_state.MEM_stage.implement)
    ```
2. Control Hazard
    - 若未發生stall且為beq指令時
        - 當其rs = rt，PC = (PC + 4) + [address(16bits) sign-extended to 32bits and shift left 2bits(*4)]
        - 當其rs != rt，PC = PC + 4
    ``` =c++
    if (current_state.ID_stage.ins_name == "beq")
        int beq_rs = RF.read(reg1).to_ulong();
        int beq_rt = RF.read(reg2).to_ulong();
        //* 先判斷 beq 須從哪裡拿取資料 (除前指令為R-type外 其他狀況皆可直接拿reg)
        if (branch_hazzard != 0)
        {
            if (branch_hazzard == 1)
            { // beq data hazzard 發生在beq的rs
                beq_rs = current_state.MEM_stage.ALU_result.to_ulong();
            }
            else if (branch_hazzard == 2)
            { // beq data hazzard 發生在beq的rs
                beq_rt = current_state.MEM_stage.ALU_result.to_ulong();
            }
            branch_hazzard = 0;
        }

        if (beq_rs == beq_rt) //* rs = rt
        {
            next_state.ID_stage.implement = 0;
            bitset<1> sign_bit = bitset<1>(shift_bits(current_state.ID_stage.ins, 15));
            int i_addr = 0;
            //* 判斷正負
            if (sign_bit.to_ulong() == 1)
            {
                //* (-2^15 + 後15bits轉成的整數) * 4
                i_addr = ((-32768) + bitset<15>(shift_bits(current_state.ID_stage.ins, 0)).to_ulong()) * 4;
            }
            else
            {
                //* 後15bits轉成的整數 * 4
                i_addr = bitset<15>(shift_bits(current_state.ID_stage.ins, 0)).to_ulong() * 4;
            }

            //* (PC + 4) + iddr[等於經過sign_extend_32，然後shift_left_2]
            next_state.IF_stage.PC = bitset<32>((current_state.IF_stage.PC.to_ulong()) + i_addr); // 更新PC
        }
    }
    ```
3. EX hazard
    - 確認是否已有執行至MEM_stage 
    - 判斷前前指令必須是會更新暫存器的 
    - 寫入的暫存器必須非$0  
    - write_reg(R-type: rd，Lw: rt) = rs / rt 時發生
    ``` =c++
    if (current_state.WB_stage.implement && MEM_condition && !(EX_condition && current_state.MEM_stage.Write_reg == current_state.EX_stage.rs) && current_state.WB_stage.Write_reg == current_state.EX_stage.rs)
    if (current_state.WB_stage.implement && MEM_condition && !(EX_condition && current_state.MEM_stage.Write_reg == current_state.EX_stage.rt) && current_state.WB_stage.Write_reg == current_state.EX_stage.rt)
    ```
4. MEM hazard
    - 確認是否已有執行至WB_stage
    - 判斷前前指令必須是會更新暫存器的 
    - 寫入的暫存器必須非$0 
    - 在EX_hazard不成立的情況，也就是前指令無法給資料
    - write_reg(R-type: rd，Lw: rt) = rs / rt 時發生
    ```
    if (current_state.MEM_stage.implement && EX_condition && current_state.MEM_stage.Write_reg == current_state.EX_stage.rs)
    {
        current_state.EX_stage.Read_data1 = current_state.MEM_stage.ALU_result;
    }
    if (current_state.MEM_stage.implement && EX_condition && current_state.MEM_stage.Write_reg == current_state.EX_stage.rt)
    {
        current_state.EX_stage.Read_data2 = current_state.MEM_stage.ALU_result;
    }
    ```

    
### bitset
> 需大量處理2進位資料，因此引入bitset。利用此型態，不管是處理shift、extend、轉十進位及轉為字串都十分快速且方便

## 問題與解決方法

1. 有states的程式碼，debug具有一定難度
    * solution 1 : 將每個cycle中，所有stages的訊息詳細印出來，藉此判斷在哪個出現問題
    * solution 2 : 設立多個中斷點，以利看出是哪一步出現問題
2. 如何用github協作專題，經常發生嚴重merge conflict
    * solution 1 : 隨時更新當前進度，不要太久沒merge
    * solution 2 : 將源代碼pull到local，與自己的code merge，將conflict解決後再重新發送pull request
3. 思考整個程式基礎框架，需哪些標頭檔及其內容
    * solution 1 : 依上課投影片介紹，規劃每個stage的input與output，及control sign於各階段的使用情形，並且將IM、RF、DM的功能實做出來
    * solution 2 : 在程式實作中，依編寫情形更新標頭檔內容，逐步完善
4. 依不同指令將其轉成machine code，並用其來完成後續所有過程，在過程中判斷machine code與將其進行運算很繁瑣
    * solution 1 : 上網查詢指令之opcode與funct
    * solution 2 : 須明確了解所有指令instruction field功能及差別
    * solution 3 : 因錯誤通常都不明顯，花費大量時間debug
5. Stall與Forwarding的判斷
    * solution 1 : 查閱ppt與網路資料
    * solution 2 : 仔細思考所有條件並瘋狂debug

## 工作分配
|      | 工作內容               |
| ---- | ---------------------- |
| 沈品豪 | forwarding、word|
| 黃泓茗 | stall、forwarding、readme|
| 鄭惠心 | stall、word|
| 陳柏安 | stall、forwarding、readme|