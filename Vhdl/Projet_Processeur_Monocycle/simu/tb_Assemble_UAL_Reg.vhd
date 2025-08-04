library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity tb_Assemble is
end entity tb_Assemble;





Architecture tb of tb_Assemble is 
signal	Clk: std_logic;
signal	Reset: std_logic;
signal	Win: std_logic_vector(31 downto 0); 
signal	RA: std_logic_vector(3 downto 0);
signal	RB: std_logic_vector(3 downto 0);
signal	RW:  std_logic_vector(3 downto 0);
signal	WE:  std_logic;
signal	OP:  std_logic_vector(2 downto 0);



begin 

tb: entity work.Assemble(Behaviour) 
port map(Clk,Reset,RA,RB,RW,WE,OP); 


Clock: process
  begin
    for i in 0 to 99999 loop
      Clk <= '0';
      wait for 5 NS;
      Clk <= '1';
      wait for 5 NS;
    end loop;
    wait;
  end process;

Reset <= '0';

--Question 1 
-- R(1) = R(15) 
-- we read the value of the R(15) first
--we write the value of R(15) qui est le win dans wout

--Question2 
--R(1) = R(1) + R(15);
-- We read the value of R(1) and R(15) 
-- OP = +, RA = 1 , RB = 15

OP <= "011",		--Send R(15)
	"011" after 50ns,
	"000" after 100ns, --ADD R(15) and R(1) Q2
	"011" after 150ns, 
	"000" after 200ns, --ADD R(15) and R(1) Q3
	"011" after 250ns,
	"010" after 300ns, --SUB R(1) and R(15) Q4 
	"011" after 350ns,
	"010" after 400ns, --SUB R(7) and R(15) Q5
	"011" after 450ns;
WE <= '0',		--Read R(15) Q1
	'1' after 50ns, --Write R(15) in R(1) Q1 
	'0' after 100ns, --Read R(15) and R(1) Q2 
	'1' after 150ns, --Write R(15) + R(1) in R(1);
	'0' after 200ns, --Read R(15) and R(1) Q3
	'1' after 250ns, --Write R(15)+R(1) dans  Q3
	'0' after 300ns, --Read R(15) and R(1) Q4
	'1' after 350ns, --Write R(15)- R(1) in R(3) Q4
	'0' after 400ns, --Read R(15) and R(7) Q5
	'1' after 450ns, --Write R(7) -R(15) in R(5)
	'0' after 500ns; 
RA <= "1111",		--R(15) Q1
	"0001" after 100ns, --R(1) Q2
	"0001" after 300ns, --R(1) Q4 
	"0110" after 400ns, --R(7) Q5 
	"0101" after 450ns; --R(5) Q5

RB <= "1111",			--R(15) Q2
	"1111" after 150ns,	--R(15) Q3		
	"1111" after 300ns,	--R(15) Q4	
	"1111" after 400ns;	--R(15) Q5	

RW <= "0001",		--R(1) Q1 et Q2
	"0010" after 250ns,		--R(2) Q3
	"0011" after 300ns,		--R(2) Q4
	"0101" after 400ns; 		--R(5) Q5


end Architecture; 
