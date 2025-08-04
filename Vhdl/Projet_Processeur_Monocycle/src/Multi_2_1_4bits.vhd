library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Multi_2_1_4bits is
  port (
    A, B : in std_logic_vector(3 downto 0);
    Sel : in std_logic;
    Y : out std_logic_vector(3 downto 0)
  );
end entity;

architecture Behavioral of Multi_2_1_4bits is
begin
  process (A, B, Sel)
  begin
    if Sel = '0' then
      Y <= A;
    else
      Y <= B;
    end if;
  end process;
end Behavioral;
