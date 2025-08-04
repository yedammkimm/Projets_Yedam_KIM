library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_signed.all;


entity tb_UAL is 
end entity; 

Architecture Behavioral of tb_UAL is 
signal 	A: std_logic_vector(31 downto 0); 
signal	B: std_logic_vector(31 downto 0);
signal	OP: std_logic_vector(2 downto 0); 
signal	S: std_logic_vector(31 downto 0);
signal	N,Z,C,V: std_logic;

begin 
U: entity work.UAL(Behavioral)
port map(A,B,OP,S,N,Z,C,V);


  OP <= "000", 
        "001" after 150ns,
        "010" after 300ns,
        "011" after 450ns,
        "100" after 600ns,
        "101" after 750ns,
        "110" after 900ns,
        "111" after 1050ns,
	"000" after 1100ns;

    A <= x"00000000", 
         x"00000001" after 10ns,
         x"00000002" after 20ns,
         x"00000003" after 30ns,
         x"00000004" after 40ns,
         x"00000005" after 50ns,
         x"00000006" after 60ns,
         x"00000007" after 70ns,
         x"00000008" after 80ns,
         x"00000009" after 90ns,
         x"00000010" after 100ns,
         x"00000011" after 110ns,
         x"00000012" after 120ns,
         x"00000013" after 130ns,
         x"00000014" after 140ns,
         x"FFFFFFFF" after 150ns;
        

    B <= x"00000016",
         x"00000017" after 10ns,
         x"00000018" after 20ns,
         x"00000019" after 30ns,
         x"00000020" after 40ns,
         x"00000021" after 50ns,
         x"00000022" after 60ns,
         x"00000023" after 70ns,
         x"00000024" after 80ns,
         x"00000025" after 90ns,
         x"00000026" after 100ns,
         x"00000027" after 110ns,
         x"00000028" after 120ns,
         x"00000029" after 130ns,
         x"00000030" after 140ns,
         x"FFFFFFFF" after 150ns;

    

 
end architecture;

