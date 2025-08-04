----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/01/2023 04:18:31 PM
-- Design Name: 
-- Module Name: Compteur - Behavioral
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


entity Compteur is
Generic (
N: integer := 24);
port (
IN_100 : in std_logic;
reset: in std_logic; 
sel : out std_logic_vector( 1 downto 0)); 

end Compteur; 

architecture Behavioral of Compteur is 
signal seq: std_logic;
signal compt_1 : std_logic_vector((N-1) downto 0); 
signal compt_2 : std_logic_vector(1 downto 0);
constant compt_1_max : std_logic_vector((N-1) downto 0) := X"98967F"; --demiperiode

begin 

process(IN_100) is 
begin

if reset = '1' then 
    compt_1 <= (others => '0'); 
    seq <= '0';
elsif reset ='0' then 
if IN_100'event and IN_100 = '1' then 
if compt_1 < compt_1_max - '1' then 
compt_1 <= compt_1 + '1'; 
seq <= '1';
elsif compt_1 > compt_1_max - "10" and compt_1 < (compt_1_max + "10") - '1' then 
compt_1 <= compt_1 + '1'; 
seq <= '0'; 
elsif compt_1 = (compt_1_max + "10") - '1' then 
compt_1 <= (others => '0'); 
end if; 
end if; 
end if; 
end process; 


process(seq) is 
begin 
if reset ='1' then 
compt_2 <= "11"; 
elsif reset = '0' then 
if seq'event and seq = '1' then 
compt_2 <= compt_2 + '1'; 
end if; 
end if; 
end process; 

sel <= compt_2;
end Behavioral;
