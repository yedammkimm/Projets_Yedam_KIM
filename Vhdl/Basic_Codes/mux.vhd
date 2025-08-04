----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/27/2023 02:43:56 PM
-- Design Name: 
-- Module Name: mux - Behavioral
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

entity mux is
    Port ( Sel : in STD_LOGIC_VECTOR (1 downto 0);
           mux_out : out STD_LOGIC_VECTOR (3 downto 0)
           );
end mux;

architecture Behavioral of mux is
signal a: STD_LOGIC_VECTOR (3 downto 0) := "1110";
signal b: STD_LOGIC_VECTOR (3 downto 0) := "1101";
signal c: STD_LOGIC_VECTOR (3 downto 0) := "1011";
signal d: STD_LOGIC_VECTOR (3 downto 0) := "0111";

begin

with Sel select 

   mux_out <= a when "00", 
         b when "01", 
         c when "10", 
         d when "11";

end Behavioral;
