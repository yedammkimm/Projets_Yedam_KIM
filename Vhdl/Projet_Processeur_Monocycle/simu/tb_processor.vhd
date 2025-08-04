library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_processor is
end tb_processor;

architecture Behavioral of tb_processor is
  signal CLK : std_logic := '0';
  signal RESET : std_logic := '0';
  signal Afficheur: std_logic_vector(31 downto 0);
  constant CLK_period : time := 5 ns;

begin

  proc: entity work.processor(Behavioral) 
  port map (
      CLK => CLK,
      RESET => RESET,
      Afficheur => Afficheur
  );

  CLK_process :process
  begin
    CLK <= '1';
    wait for CLK_period/2;
    CLK <= '0';
    wait for CLK_period/2;
  end process;

  stim_proc: process
  begin		
    -- Reset the Unit
    RESET <= '1';
    wait for 2ns;
    RESET <= '0';

    -- Wait for the processor to execute instructions
    wait for 100000 ns;

    -- End simulation
    wait;
  end process;

end Behavioral;

