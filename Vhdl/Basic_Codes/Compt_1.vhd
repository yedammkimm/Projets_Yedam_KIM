----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/11/2023 09:31:08 AM
-- Design Name: 
-- Module Name: Compt_1 - Behavioral
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

entity Compt_1 is
  Port ( IN_100 : in STD_LOGIC; 
  OUT_1: inout std_logic );
end Compt_1;

architecture Behavioral of Compt_1 is
signal sclk: std_logic_vector(24 downto 0); 
--1 ms = 11000011010100000
--0.5 s = 10111110101111000010000000
-- 1s = 100110001001011010000000

begin 

process(IN_100) is 
begin
if rising_edge(IN_100) then 
    if sclk = "10111110101111000010000000" then 
        OUT_1 <= not OUT_1;
        sclk <= "0000000000000000000000000";
    else
        sclk <= sclk+1;
        end if; 
       end if;
end process;

end Behavioral;
