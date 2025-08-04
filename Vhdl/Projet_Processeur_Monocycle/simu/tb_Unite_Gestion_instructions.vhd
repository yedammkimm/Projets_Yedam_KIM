library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_Unite_Gestion_instructions is
end tb_Unite_Gestion_instructions;

architecture Behavioral of tb_Unite_Gestion_instructions is
    signal CLK : std_logic := '0';
    signal Reset : std_logic := '0';
    signal Offset : std_logic_vector(23 downto 0) := (others => '0');
    signal nPCSel : std_logic := '0';
    signal Instruction : std_logic_vector(31 downto 0);
    constant CLK_period : time := 10 ns;

begin
UGI: entity work.Unite_Gestion_instructions(Behavioral) 
port map ( Offset => Offset, nPCSel => nPCSel, CLK => CLK, Reset => Reset, Instruction => Instruction );

    CLK_process :process
    begin
        CLK <= '0';
        wait for CLK_period/2;
        CLK <= '1';
        wait for CLK_period/2;
    end process;


    proc: process
    begin		
        Reset <= '1';
        wait for CLK_period*2;
        Reset <= '0';

        nPCSel <= '0'; 
        wait for CLK_period;

        Offset <= X"000001";
        nPCSel <= '1'; 
        wait for CLK_period;

        wait;
    end process;

end Behavioral;

