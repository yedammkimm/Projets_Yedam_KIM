library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Assemblage_unite_trait_updated is
  port( 
    CLK : in  std_logic;
    RESET : in  std_logic;
    RegSel,ALUSrc, Reg_WE, MemWr, RegAff,WrSrc : in std_logic;
    IMM8 : in std_logic_vector(7 downto 0);
    RA, Rm, Rd, RW : in  std_logic_vector(3 downto 0);
    W : in std_logic_vector(31 downto 0);
    ALUCtr : in std_logic_vector(2 downto 0);
    Afficheur: out std_logic_vector(31 downto 0);
    N, Z, C, V : out std_logic
  );
end entity;

architecture Behavioral of Assemblage_unite_trait_updated is
  signal busA, busB, ALU_B_Input, W_interne, exten_out, Data_Out, ALU_out : std_logic_vector(31 downto 0);
  signal RB_Addr : std_logic_vector(3 downto 0); 
begin

mux_RB: entity work.Multi_2_1_4bits(Behavioral) 
port map ( A => Rm, B => Rd, Sel => RegSel, Y => RB_Addr ); --RegSel


  reg: entity work.registre(Behavioral)
    port map (Clk => Clk, Reset => Reset, W => W_interne, RA => RA, RB => RB_Addr, RW => RW, WE => Reg_WE, A => busA, B => busB);


  ext: entity work.exten_signe(Behavioral)
    port map (E => IMM8, S => exten_out);


  mux_ALUSrc: entity work.Multi_2_1(Behavioral)
    port map (A => busB, B => exten_out, COM => ALUSrc, S => ALU_B_Input); --ALUSrc


  ALU_inst: entity work.UAL(Behavioral)
    port map (A => busA, B => ALU_B_Input, OP => ALUCtr, S => ALU_out, N => N, Z => Z, C => C, V => V); --ALUCtr


  memoire_de_donnees_inst: entity work.memoire_de_donnees(Behavioral)
    port map (CLK => Clk, Reset => Reset, DataIn => busB, Addr => ALU_out(5 downto 0), WrEn => MemWr, DataOut => Data_Out); --MemWr


  mux_end: entity work.Multi_2_1(Behavioral)
    port map (A => ALU_out, B => Data_Out, COM => WrSrc, S => W_interne); --WrSrc

  Aff: entity work.commande_32(Behavioral)
    port map(DATAIN => busB,CLK => CLK, RST=> RESET,WE => RegAff,DATAOUT=> Afficheur);

end Behavioral;
