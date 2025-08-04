library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity registre is
  port(
    Clk: in std_logic;
    Reset: in std_logic;
    W: in std_logic_vector(31 downto 0);
    RA: in std_logic_vector(3 downto 0);
    RB: in std_logic_vector(3 downto 0);
    RW: in std_logic_vector(3 downto 0);
    WE: in std_logic;
    A: out std_logic_vector(31 downto 0);
    B: out std_logic_vector(31 downto 0)
  );
end entity;

architecture Behavioral of registre is
  type table is array(15 downto 0) of std_logic_vector(31 downto 0);

  function init_banc return table is
    variable result: table;
  begin
    for i in 14 downto 0 loop
      result(i) := (others => '0');
    end loop;
    result(15) := X"00000030";
    return result;
  end init_banc;

  signal Banc: table := init_banc;

begin
    A <= Banc(to_integer(unsigned(RA)));
    B <= Banc(to_integer(unsigned(RB)));

  process(Clk, Reset, WE)
  begin
    if Reset = '1' then
      for i in 0 to 14 loop
        Banc(i) <= (others => '0');
      end loop;
      Banc(15) <= X"00000030";
    elsif rising_edge(Clk) then
      if WE = '1' then
        Banc(to_integer(unsigned(RW))) <= W;
      end if;
    end if;
  end process;
end Behavioral;
