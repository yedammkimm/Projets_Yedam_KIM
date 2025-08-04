library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity memoire_de_donnees is
  port (
    CLK: in std_logic;
    Reset: in std_logic;
    DataIn: in std_logic_vector(31 downto 0);
    Addr: in std_logic_vector(5 downto 0);
    WrEn: in std_logic;
    DataOut: out std_logic_vector(31 downto 0)
  );
end entity;

architecture Behavioral of memoire_de_donnees is
type mem_array is array (0 to 63) of std_logic_vector(31 downto 0);
  signal RAM : mem_array := (
    32 => X"00000001", -- 0x20
    33 => X"00000002", -- 0x21
    34 => X"00000003", -- 0x22
    35 => X"00000004", -- 0x23
    36 => X"00000005", -- 0x24
    37 => X"00000006", -- 0x25
    38 => X"00000007", -- 0x26
    39 => X"00000008", -- 0x27
    40 => X"00000009", -- 0x28
    41 => X"0000000A", -- 0x29
    42 => X"0000000B", -- 0x2A
    others => (others => '0')
  );

begin
  process(CLK, Reset)
  begin
    if Reset = '1' then
    elsif rising_edge(CLK) then
      if WrEn = '1' then
        RAM(to_integer(unsigned(Addr))) <= DataIn;
      end if;
    end if;
  end process;

  process(Addr, RAM)
  begin
    DataOut <= RAM(to_integer(unsigned(Addr)));
  end process;
end Behavioral;
