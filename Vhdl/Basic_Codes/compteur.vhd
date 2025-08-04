----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/27/2023 01:06:10 PM
-- Design Name: 
-- Module Name: compteur - Behavioral
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

entity compteur is
    Generic (
             N : integer := 24
             );
    Port ( IN_100 : in STD_LOGIC;
           reset : in STD_LOGIC; 
           Sel : out STD_LOGIC_VECTOR (1 downto 0)
           );
end compteur;

architecture Behavioral of compteur is

signal seq : STD_LOGIC; 
signal compt_1 : STD_LOGIC_VECTOR ((N-1) downto 0);
signal compt_2 : STD_LOGIC_VECTOR (1 downto 0); 
constant compt_1_max : STD_lOGIC_VECTOR ((N-1) downto 0) := "000000011000011010100000"; -- demi periode pour an

begin

process (IN_100) is 
begin 

if reset ='1' then 
        compt_1 <= (others => '0');
        seq <= '0'; 
elsif reset ='0' then 
    if IN_100'event and IN_100 = '1' then 
        if compt_1 < compt_1_max -'1' then 
            compt_1 <= compt_1 + '1'; 
            seq <= '1'; 
        elsif  compt_1 > compt_1_max -"10" and compt_1 < (compt_1_max*"10") - '1' then 
            compt_1 <= compt_1 + '1'; 
            seq <= '0';
        elsif compt_1 = (compt_1_max*"10") - '1' then 
            compt_1 <= (others => '0');
        end if;
    end if;
end if; 
end process;


process(seq) is 
begin 
if reset ='1' then 
        compt_2 <= "11";    
elsif reset ='0' then 
    if seq'event and seq = '1' then 
        compt_2 <= compt_2 + '1'; 
    end if; 
end if;
end process;  

sel<= compt_2; 

end Behavioral;
