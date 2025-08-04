----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/11/2023 11:23:41 AM
-- Design Name: 
-- Module Name: Mux_1 - Behavioral
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

entity Mux_1 is
  Port (U,D,C,M: in std_logic_vector(3 downto 0);
  Sel: in std_logic_vector(1 downto 0);
  Out_4: out std_logic_vector(3 downto 0));
end Mux_1;

architecture Behavioral of Mux_1 is

begin

with sel select

Out_4 <= U when "00",
         D when "01",
         C when "10",
         M when "11";

end Behavioral;
