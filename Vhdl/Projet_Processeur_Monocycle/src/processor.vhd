library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity processor is
  port (
    CLK : in std_logic;
    RESET : in std_logic;
    Afficheur: out std_logic_vector(31 downto 0)
  );
end entity;

architecture Behavioral of processor is
  signal nPCSel : std_logic;
  signal RegSel, RegWr, ALUSrc, PSREn, MemWr, WrSrc, RegAff : std_logic;
  signal ALUCtr : std_logic_vector(2 downto 0);
  signal busA, busB, ALU_B_Input, W_interne, exten_out, Data_Out, ALU_out : std_logic_vector(31 downto 0);
  signal N, Z, C, V : std_logic;
  signal Instr_interne : std_logic_vector(31 downto 0);  
  signal PSR_in: std_logic_vector(31 downto 0);

begin

PSR_in <= N&Z&C&V&x"0000000";

UGI: entity work.Unite_Gestion_instructions(Behavioral)  
port map ( Offset => Instr_interne(23 downto 0), nPCSel => nPCSel, CLK => CLK, Reset => RESET, Instruction => Instr_interne);

Unite_cont: entity work.unite_controle(Behavioral) 
port map ( Instr => Instr_interne, PSR_in => PSR_in, CLK => CLK, RST => RESET, nPCSel => nPCSel, RegWr => RegWr, ALUSrc => ALUSrc, PSREn => PSREn, MemWr => MemWr, WrSrc => WrSrc, RegSel => RegSel, RegAff => RegAff, ALUCtr => ALUCtr );

unit_trait: entity work.Assemblage_unite_trait_updated(Behavioral) 
port map ( CLK => CLK, RESET => RESET,RegSel => RegSel, ALUSrc => ALUSrc, WrSrc => WrSrc, Reg_WE => RegWr,RegAff => RegAff, MemWr => MemWr, IMM8 => Instr_interne(7 downto 0), RA => Instr_interne(19 downto 16), Rm => Instr_interne(3 downto 0), Rd => Instr_interne(15 downto 12), RW => Instr_interne(15 downto 12), W => W_interne, ALUCtr => ALUCtr, Afficheur => Afficheur, N => N, Z => Z, C => C, V => V );


end Behavioral;
