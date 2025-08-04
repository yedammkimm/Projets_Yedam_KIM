library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;



entity Assemble is 
port(
	Clk: in std_logic;
	Reset: in std_logic;
	RA: in std_logic_vector(3 downto 0);
	RB: in std_logic_vector(3 downto 0);
	RW: in std_logic_vector(3 downto 0);
	WE: in std_logic;
	OP: in std_logic_vector(2 downto 0)
); 

end entity; 


Architecture Behaviour of Assemble is 

signal Win: std_logic_vector(31 downto 0); 
signal A,B: std_logic_vector(31 downto 0); 

begin 

reg: entity work.registre(Behaviour) 
port map(Clk => Clk, Reset => Reset, W  =>Win, RA  => RA, RB  => RB, RW => RW, WE => WE, A  => A, B  => B);

UAL: entity work.UAL(Behavioral)
port map(A  => A, B  => B, OP  => OP, S  => Win); 

end Behaviour; 


