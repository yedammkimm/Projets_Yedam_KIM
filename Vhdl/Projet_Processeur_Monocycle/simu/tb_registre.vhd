library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_signed.all;


entity tb_registre is 
end entity; 

Architecture Behavioral of tb_registre is 
signal	Clk: std_logic;
signal	Reset: std_logic;
signal	W: std_logic_vector(31 downto 0); 
signal	RA: std_logic_vector(3 downto 0);
signal	RB: std_logic_vector(3 downto 0);
signal	RW: std_logic_vector(3 downto 0);
signal	WE: std_logic;
signal	A: std_logic_vector(31 downto 0);
signal	B: std_logic_vector(31 downto 0);

begin 
reg: entity work.registre(Behavioral)
  port map (
      Clk => Clk,
      Reset => Reset,
      W => W,
      RA => RA,
      RB => RB,
      RW => RW,
      WE => WE,
      A => A,
      B => B
    );


  Clk_process :process
  begin
    while true loop
      Clk <= '0';
      wait for 10 ns;
      Clk <= '1';
      wait for 10 ns;
    end loop;
    wait;
  end process;


  proc: process
  begin

    Reset <= '1';
    RA <= "0000"; 
    RB <= "0000"; 
    RW <= "0000";
    WE <= '0';
    W <= X"00000000";
    wait for 20 ns;
    Reset <= '0';
    wait for 20 ns;


    WE <= '1';
    RW <= "0000";
    W <= X"00000001";
    wait for 20 ns;

    RW <= "0001";
    W <= X"00000002";
    wait for 20 ns;

    WE <= '0';
    RW <= "0000";
    W <= (others => '0');
    wait for 20 ns;

    RA <= "0000";
    RB <= "0001";
    wait for 20 ns;

    RA <= "1111";
    wait for 20 ns;


    WE <= '1';
    RW <= "0010";
    W <= X"00000003";
    wait for 20 ns;

    RA <= "0010";
    wait for 20 ns;

    WE <= '1';
    RW <= "1110";
    W <= X"00000004";
    wait for 20 ns;

    RA <= "1110";
    wait for 20 ns;

    wait;
end process; 
end architecture;

