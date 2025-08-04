----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/24/2023 01:58:52 PM
-- Design Name: 
-- Module Name: TD_2 - Behavioral
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

entity binary_decoder.vhd is
Port(A,B,C,D: in std_logic;
     S: out std_logic_vector(3 downto 0));
end entity binary_decoder.vhd;


architecture Behavioral of binary_decoder.vhd is
signal sol: std_logic_vector(3 downto 0);
signal EA: std_logic_vector(3 downto 0):= "0001";
signal EB: std_logic_vector(3 downto 0):= "0010";
signal EC: std_logic_vector(3 downto 0):= "0100";
signal ED: std_logic_vector(3 downto 0):= "1000";

begin 
sol(0) <= A;
sol(1) <= not(A) and B; 
sol(2) <= not(A) and not(B) and C; 
sol(3) <= not(A) and not(B) and not(C) and D; 


with sol select 
S <= EA when "0001", 
     EB when "0010", 
     EC when "0100",
     ED when "1000",
     "0000" when others;

end Behavioral;
