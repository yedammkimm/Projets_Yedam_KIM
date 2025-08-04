----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/10/2023 04:17:13 PM
-- Design Name: 
-- Module Name: Decondeur_7seg - Behavioral
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Decodeur_7seg_final is
port( A : in std_logic_vector(3 downto 0);
S : out std_logic_vector(11 downto 0));

end Decodeur_7seg_final;

architecture Behavioral of Decodeur_7seg_final is
begin

With A select

S <= "000100111111" when "0000",
"000100000110" when "0001",
"000101011011" when "0010",
"000101001111" when "0011",
"000101100110" when "0100",
"000101101101" when "0101",
"000101111100" when "0110",
"000100100111" when "0111",
"000101111111" when "1000",
"000101100111" when "1001",

"000101110111" when "1010",
"000101111100" when "1011",
"000101111001" when "1100",
"000101011110" when "1101",
"000101111001" when "1110",
"000101110001" when "1111";


end Behavioral;
