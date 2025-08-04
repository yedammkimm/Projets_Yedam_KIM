library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity commande_32 is
  port (
    DATAIN : in std_logic_vector(31 downto 0);
    CLK, RST, WE : in std_logic;
    DATAOUT: out std_logic_vector(31 downto 0)
  );
end entity;

architecture Behavioral of commande_32 is
  signal registre: std_logic_vector(31 downto 0);

begin
  DATAOUT <= registre;
  process(CLK, RST)
  begin
    if RST = '1' then 
      registre <= (others => '0');
    elsif rising_edge(CLK) then 
      if WE = '1' then 
        registre <= DATAIN; 
      end if; 
    end if; 
  end process;
end Behavioral;
