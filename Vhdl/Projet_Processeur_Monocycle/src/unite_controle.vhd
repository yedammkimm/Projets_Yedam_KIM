library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity unite_controle is
  port (
    Instr : in std_logic_vector(31 downto 0);
    PSR_in : in std_logic_vector(31 downto 0);
    CLK, RST: in std_logic;
    nPCSel, RegWr, ALUSrc, PSREn, MemWr, WrSrc, RegSel, RegAff : out std_logic;
    ALUCtr : out std_logic_vector(2 downto 0)
  );
end entity;

architecture Behavioral of unite_controle is
signal PSREn_Input: std_logic;
signal con_out : std_logic_vector(31 downto 0);

begin
PSREn <= PSREn_Input;

comm_inst : entity work.commande_32(Behavioral) 
port map ( DATAIN => PSR_in, CLK => CLK, RST => RST, WE => PSREn_Input, DATAOUT => con_out);

decoder_inst : entity work.instruction_decoder(Behavioral)
port map ( Instruction => Instr, PSR_out => con_out, nPCSel => nPCSel, RegWr => RegWr, ALUSrc => ALUSrc, PSREn => PSREn_Input, MemWr => MemWr, WrSrc => WrSrc, RegSel => RegSel, RegAff => RegAff, ALUCtr => ALUCtr );


end Behavioral;

