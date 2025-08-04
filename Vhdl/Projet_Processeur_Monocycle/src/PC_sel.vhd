library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity PC_sel is
  port (
	CLK: in std_logic; 
	Reset: in std_logic; 
	Offset_in: in std_logic_vector(31 downto 0); 
	nPCsel: in std_logic;
	PC: out std_logic_vector(31 downto 0)
  );
end entity;


architecture Behavioral of PC_sel is
signal PC_interne: std_logic_vector(31 downto 0);
begin

PC <= PC_interne;

Process(CLK,Reset,nPCsel)
begin 
	if Reset = '1' then 
		PC_interne <= "00000000000000000000000000000000";
	else 
		if rising_edge(CLK) then 
			if nPCsel = '0' then 
				PC_interne <= Std_logic_vector(To_unsigned(To_integer(Unsigned(PC_interne)) + 1,32)); 
			else 
				PC_interne <= Std_logic_vector(To_unsigned(To_integer(Unsigned(PC_interne) + Unsigned(Offset_in)) + 1,32)); 

			end if; 
		end if;
	end if;  
end process; 
end Behavioral; 
