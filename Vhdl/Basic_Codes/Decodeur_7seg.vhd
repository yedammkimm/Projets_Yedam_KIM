----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/08/2023 05:44:11 PM
-- Design Name: 
-- Module Name: Decodeur_7seg - Flot
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

entity Decodeur_7seg is
    Port ( valeur : in STD_LOGIC_VECTOR (3 downto 0);
           sept_seg : out STD_LOGIC_VECTOR (7 downto 0)
           );
end Decodeur_7seg;

architecture Flot of Decodeur_7seg is


begin


with valeur select 

   sept_seg     <= "00000011" when "0000", 
                   "10011111" when "0001",
                   "00100101" when "0010",
                   "00001101" when "0011",
                   "10011001" when "0100",
                   "01001001" when "0101",
                   "11000001" when "0110",
                   "00011111" when "0111",
                   "00000001" when "1000", 
                   "00001001" when "1001", 
                   "11111111" when others;
end Flot;
