library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity exten_signe is
generic(N : integer := 8);
port (
    E : in std_logic_vector(N-1 downto 0);
    S : out std_logic_vector(31 downto 0)
  );
end entity;

architecture Behavioral of exten_signe is
begin
  process(E)
  begin
    if E(N-1) = '1' then
      S <= (31 downto N => '1') & E;
    else
      S <= (31 downto N => '0') & E;
    end if;
  end process;
end Behavioral;
