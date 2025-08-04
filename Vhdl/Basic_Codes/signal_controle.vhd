----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/01/2023 02:53:29 PM
-- Design Name: 
-- Module Name: signal_controle - Behavioral
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

entity signal_controle is
  Port ( 
  clk: in std_logic; 
  compteur: in std_logic_vector(1 downto 0);
  an: out std_logic_vector(3 downto 0));
end signal_controle;


architecture Behavioral of signal_controle is
signal mul0,mul1,mul2,mul3: std_logic_vector(3 downto 0);
 

begin

mul0 <= "1110";
mul1 <= "1101";
mul2 <= "1011";
mul3 <= "0111";

with compteur select
an <= mul0 when "00",
mul1 when "01",
mul2 when "10",
mul3 when "11";

end Behavioral;





