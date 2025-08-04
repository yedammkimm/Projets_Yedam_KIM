----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/10/2023 04:38:00 PM
-- Design Name: 
-- Module Name: Test_bench - Behavioral
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
use IEEE.std_logic_unsigned.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Test_bench is
end Test_bench;

architecture test_bench of Test_bench is
signal X: std_logic_vector (3 downto 0);
signal Sortie: std_logic_vector (11 downto 0);
begin

test_affiche : entity work.Decodeur_7seg_final(Behavioral)
port map(X,Sortie);
X(0) <= '0', 
'1' after 10 ns,
'0' after 20 ns,
'1' after 30 ns,
'0' after 40 ns,
'1' after 50 ns,
'0' after 60 ns,
'1' after 70 ns,
'0' after 80 ns,
'1' after 90 ns,
'0' after 100 ns,
'1' after 110 ns,
'0' after 120 ns,
'1' after 130 ns,
'0' after 140 ns,
'1' after 150 ns;

X(1) <= '0', 
'0' after 10 ns,
'1' after 20 ns,
'1' after 30 ns,
'0' after 40 ns,
'0' after 50 ns,
'1' after 60 ns,
'1' after 70 ns,
'0' after 80 ns,
'0' after 90 ns,
'1' after 100 ns,
'1' after 110 ns,
'0' after 120 ns,
'0' after 130 ns,
'1' after 140 ns,
'1' after 150 ns;
 
X(2) <= '0', 
'0' after 10 ns,
'0' after 20 ns,
'0' after 30 ns,
'1' after 40 ns,
'1' after 50 ns,
'1' after 60 ns,
'1' after 70 ns,
'0' after 80 ns,
'0' after 90 ns,
'0' after 100 ns,
'0' after 110 ns,
'1' after 120 ns,
'1' after 130 ns,
'1' after 140 ns,
'1' after 150 ns;

X(3) <= '0', 
'0' after 10 ns,
'0' after 20 ns,
'0' after 30 ns,
'0' after 40 ns,
'0' after 50 ns,
'0' after 60 ns,
'0' after 70 ns,
'1' after 80 ns,
'1' after 90 ns,
'1' after 100 ns,
'1' after 110 ns,
'1' after 120 ns,
'1' after 130 ns,
'1' after 140 ns,
'1' after 150 ns;

end architecture test_bench;
