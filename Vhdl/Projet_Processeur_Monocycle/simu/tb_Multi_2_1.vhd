library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tb_Multi_2_1 is
end tb_Multi_2_1;

architecture Behavioral of tb_Multi_2_1 is

    signal A, B, S : std_logic_vector(31 downto 0) := (others => '0');
    signal COM : std_logic := '0';
    constant CLK_period : time := 10 ns;

begin
Mul: entity work.Multi_2_1(Behavioral) 
port map ( A => A, B => B, COM => COM, S => S );
    
    proc: process
    begin
        A <= X"AAAAAAAA";
        B <= X"55555555";
        COM <= '0';
        wait for CLK_period;
        COM <= '1';
        wait for CLK_period;
        COM <= '0';
        wait for CLK_period;
        COM <= '1';
        wait for CLK_period;
        wait;
    end process;
end Behavioral;