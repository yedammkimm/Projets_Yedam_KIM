----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/15/2023 02:32:16 PM
-- Design Name: 
-- Module Name: test_bench - Behavioral
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
use IEEE.STD_LOGIC_UNSIGNED.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity test_bench is
--  Port ( );
end test_bench;

architecture Behavioral of test_bench is
signal   clk        : STD_LOGIC;
signal   Row        : STD_LOGIC_VECTOR (3 downto 0);
signal   Col        : STD_LOGIC_VECTOR (3 downto 0);
signal   Chiffre    : STD_LOGIC_VECTOR (3 downto 0);
constant CLK_PERIOD : time := 10 ns;

begin

Codeur : entity work.Codeur(Behavioral)
         port map(clk,Row,Col,Chiffre);
         
clk_process : process is
begin
clk <= '0';
wait for CLK_PERIOD/2; 
clk <= '1';
wait for  CLK_PERIOD/2; 
end process;          
         
Row_process : process is 
begin 

    Row <= "0111"; 
    wait for 500 ns;
    Row <= "1111";
    wait for 100 us;
    
    Row <= "0111"; 
    wait for 1.2 ms;
    Row <= "1011";
    wait for 1.2 ms;
    Row <= "1101";
    wait for 1.2 ms;
    Row <= "1110";
    wait for 1.2 ms;
    

    Row <= "0111"; 
    wait for 1.2 ms;
    Row <= "1011";
    wait for 1.4 ms;
    Row <= "1101";
    wait for 1.6 ms;
    Row <= "1110";
    wait for 1.8 ms;
    
    Row <= "0111"; 
    wait for 1.2 ms;
    Row <= "1011";
    wait for 1.4 ms;
    Row <= "1101";
    wait for 1.6 ms;
    Row <= "1110";
    wait for 1.8 ms;
    
    Row <= "0111"; 
    wait for 1.2 ms;
    Row <= "1011";
    wait for 1.4 ms;
    Row <= "1101";
    wait for 1.6 ms;
    Row <= "1110";
    wait for 1.8 ms;

end process;         


end Behavioral;