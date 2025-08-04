----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/11/2023 09:28:16 AM
-- Design Name: 
-- Module Name: Compt_Codage_BCD - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Compt_Codage_BCD is
  Port (
  IN_1: in std_logic; 
  U,D,M,C: inout std_logic_vector(3 downto 0));
end Compt_Codage_BCD;

architecture Behavioral of Compt_Codage_BCD is

begin

process(IN_1) is 
begin
if rising_edge(IN_1) then 
U <= U+1;
if U = "1111" then 
if D = "1111" then 
if C = "1111" then 
if M = "1111"  then 
U <= "0000";
D <= "0000";
M <= "0000"; 
C <= "0000";
else 
M <= M+1;
C <= "0000";
end if;
else 
C <= C+1;
D <= "0000";
end if;
else 
D <= D+1;
U <= "0000";
end if; 
end if; 
end if;
end process;  

end Behavioral;
