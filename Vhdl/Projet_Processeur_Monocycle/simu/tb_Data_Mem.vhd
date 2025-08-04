library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tb_Data_Mem is
end tb_Data_Mem;

architecture Behavioral of tb_Data_Mem is
    signal CLK : std_logic := '0';
    signal Reset : std_logic := '0';
    signal DataIn : std_logic_vector(31 downto 0) := (others => '0');
    signal DataOut : std_logic_vector(31 downto 0);
    signal Addr : std_logic_vector(5 downto 0) := (others => '0');
    signal WrEn : std_logic := '0';
    constant CLK_period : time := 10 ns;

begin
    Data: entity work.memoire_de_donnees(Behavioral) 
port map ( CLK => CLK, Reset => Reset, DataIn => DataIn, DataOut => DataOut, Addr => Addr, WrEn => WrEn );

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

        WrEn <= '1';
        Addr <= "000001"; 
        DataIn <= X"12345678"; 
        wait for CLK_period;

        Addr <= "000010"; 
        DataIn <= X"87654321"; 
        wait for CLK_period;

        WrEn <= '0';
        Addr <= "000001"; 
        wait for CLK_period;

        Addr <= "000010"; 
        wait for CLK_period;

        wait;
    end process;

end Behavioral;