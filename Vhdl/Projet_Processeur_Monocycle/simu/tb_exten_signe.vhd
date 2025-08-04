library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tb_exten_signe is
end tb_exten_signe;

architecture Behavioral of tb_exten_signe is
    component exten_signe
        generic (N : integer := 8);
        Port ( 
            E : in std_logic_vector(N-1 downto 0);
            S : out std_logic_vector(31 downto 0)
        );
    end component;
    signal E : std_logic_vector(7 downto 0) := (others => '0');
    signal S : std_logic_vector(31 downto 0);

    constant CLK_period : time := 10 ns;
begin
    ext: exten_signe generic map (N => 8) port map ( E => E, S => S );

  stim_proc: process
  begin

    E <= X"00";
    wait for 20 ns;

    E <= X"80";
    wait for 20 ns;

    E <= X"7F";
    wait for 20 ns;

    E <= X"FF";
    wait for 20 ns;

    E <= X"01";
    wait for 20 ns;

    E <= X"FE";
    wait for 20 ns;

    E <= X"55";
    wait for 20 ns;

    E <= X"AA";
    wait for 20 ns;


    wait;
  end process;
end Behavioral;