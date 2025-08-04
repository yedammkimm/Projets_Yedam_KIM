library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity instruction_decoder is
  port (
    Instruction : in std_logic_vector(31 downto 0);
    PSR_out: in std_logic_vector(31 downto 0);
    nPCSel, RegWr, ALUSrc, PSREn, MemWr, WrSrc, RegSel, RegAff : out std_logic;
    ALUCtr : out std_logic_vector(2 downto 0)
  );
end entity;

architecture Behavioral of instruction_decoder is
  type enum_instruction is (MOV, ADDi, ADDr, CMP, LDR, STR, BAL, BLT);
  signal instr_courante: enum_instruction;
begin

  process(Instruction)
  begin
    case Instruction(31 downto 20) is
      when x"E3A" => instr_courante <= MOV;
      when x"E28" => instr_courante <= ADDi;
      when x"E08" => instr_courante <= ADDr;
      when x"E35" => instr_courante <= CMP;
      when x"E61" => instr_courante <= LDR;
      when x"E60" => instr_courante <= STR;
      when x"EAF" => instr_courante <= BAL;
      when x"BAF" => instr_courante <= BLT;
      when others => instr_courante <= MOV;
    end case;
  end process;

  process(Instruction, instr_courante)
  begin
    case instr_courante is
      when MOV =>
        RegWr <= '1'; ALUSrc <= '1'; ALUCtr <= "001"; PSREn <= '0'; MemWr <= '0'; WrSrc <= '0'; RegSel <= '0'; RegAff <= '0'; nPCSel <= '0';
      when ADDi =>
        RegWr <= '1'; ALUSrc <= '1'; ALUCtr <= "000"; PSREn <= '0'; MemWr <= '0'; WrSrc <= '0'; RegSel <= '0'; RegAff <= '0'; nPCSel <= '0';
      when ADDr =>
        RegWr <= '1'; ALUSrc <= '0'; ALUCtr <= "000"; PSREn <= '0'; MemWr <= '0'; WrSrc <= '0'; RegSel <= '0'; RegAff <= '0'; nPCSel <= '0';
      when CMP =>
        RegWr <= '0'; ALUSrc <= '1'; ALUCtr <= "010"; PSREn <= '1'; MemWr <= '0'; WrSrc <= '0'; RegSel <= '0'; RegAff <= '0'; nPCSel <= '0';
      when LDR =>
        RegWr <= '1'; ALUSrc <= '1'; ALUCtr <= "000"; PSREn <= '0'; MemWr <= '0'; WrSrc <= '1'; RegSel <= '0'; RegAff <= '0'; nPCSel <= '0';
      when STR =>
        RegWr <= '0'; ALUSrc <= '1'; ALUCtr <= "000"; PSREn <= '0'; MemWr <= '1'; WrSrc <= '0'; RegSel <= '1'; RegAff <= '1'; nPCSel <= '0';
      when BAL =>
        RegWr <= '0'; ALUSrc <= '0'; ALUCtr <= "100"; PSREn <= '0'; MemWr <= '0'; WrSrc <= '0'; RegSel <= '0'; RegAff <= '0'; nPCSel <= '1';
      when BLT =>
        RegWr <= '0'; ALUSrc <= '0'; ALUCtr <= "101"; PSREn <= '0'; MemWr <= '0'; WrSrc <= '0'; RegSel <= '0'; RegAff <= '0'; nPCSel <= PSR_out(31);
      when others =>
        RegWr <= '0'; ALUSrc <= '0'; ALUCtr <= "000"; PSREn <= '0'; MemWr <= '0'; WrSrc <= '0'; RegSel <= '0'; RegAff <= '0'; nPCSel <= '0';
    end case;
  end process;

end Behavioral;
