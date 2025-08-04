library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity PC_Extender is
  port (
	Offset: in std_logic_vector(23 downto 0); 
	Offset_out: out std_logic_vector(31 downto 0) 
  );
end entity;


architecture Behavioral of PC_Extender is
begin

Offset_out <= "00000000"& Offset;


end Behavioral; 
 
