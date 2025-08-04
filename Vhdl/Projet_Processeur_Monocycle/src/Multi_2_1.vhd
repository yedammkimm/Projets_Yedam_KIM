library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity Multi_2_1 is
  port (
    A : in std_logic_vector(31 downto 0);
    B : in std_logic_vector(31 downto 0);
    COM : in std_logic;
    S : out std_logic_vector(31 downto 0)
  );
end entity;

architecture Behavioral of Multi_2_1 is
begin
  process(A, B, COM)
  begin
    if COM = '0' then
      S <= A;
    else
      S <= B;
    end if;
  end process;
end Behavioral;
