library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity Assemblage_unite_trait is
port( 
           CLK : in  std_logic;
           RESET : in  std_logic;
           WE,COM,COM2,Reg_WE,WrEn : in  std_logic;
	   IMM: in std_logic_vector(7 downto 0);
           RA, RB, RW : in  std_logic_vector(3 downto 0);
           W : in  std_logic_vector(31 downto 0);
	   ALU_OP: in std_logic_vector(2 downto 0)
           );
end entity;


architecture Behavioral of Assemblage_unite_trait is
  signal busA, busB, ALU_B_Input, W_interne,exten_out,Data_Out,ALU_out: std_logic_vector(31 downto 0);
  signal N, Z, C, V: std_logic;

begin

  reg: entity work.registre(Behavioral)
    port map (Clk => Clk,Reset => Reset,W => W_interne,RA => RA,RB => RB,RW => RW,WE => Reg_WE,A => busA, B => busB);

  ext: entity work.exten_signe(Behavioral)
	port map(E => IMM, S=> exten_out);
	
  mux_ALUSrc: entity work.multiplexeur_2to1(Behavioral)
    port map (A => busB,B => exten_out,COM => COM,S => ALU_B_Input);

  UAL_inst: entity work.UAL(Behavioral)
    port map (A => busA,B => ALU_B_Input,OP => ALU_OP,S => ALU_out,N => N,Z => Z,C => C,V => V);
 
  memoire_de_donnees_inst: entity work.memoire_de_donnees(Behavioral)
    port map (CLK => Clk,Reset => Reset,DataIn => busB,Addr => ALU_out(5 downto 0),WrEn => WrEn,DataOut => Data_Out);

  mux_end: entity work.multiplexeur_2to1(Behavioral)
    port map (A => ALU_out,B => Data_Out,COM => COM2,S => W_interne);


end Behavioral;