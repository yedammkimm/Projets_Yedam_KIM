library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_unite_controle is
end tb_unite_controle;

architecture Behavioral of tb_unite_controle is
  signal CLK : std_logic := '0';
  signal RST : std_logic := '0';
  signal Instr : std_logic_vector(31 downto 0) := (others => '0');
  signal PSR_in : std_logic_vector(31 downto 0) := (others => '0');
  signal RegWr, ALUSrc, PSREn, MemWr, WrSrc, RegSel, RegAff : std_logic;
  signal ALUCtr : std_logic_vector(2 downto 0);
  constant CLK_period : time := 10 ns;

begin

UNC: entity work.unite_controle(Behavioral) 
port map( Instr => Instr, PSR_in => PSR_in, CLK => CLK, RST => RST, RegWr => RegWr, ALUSrc => ALUSrc, PSREn => PSREn, MemWr => MemWr, WrSrc => WrSrc, RegSel => RegSel, RegAff => RegAff, ALUCtr => ALUCtr);


  CLK_process :process
  begin
    CLK <= '0';
    wait for CLK_period/2;
    CLK <= '1';
    wait for CLK_period/2;
  end process;

  proc: process
  begin		

    RST <= '1';
    wait for CLK_period*2;
    RST <= '0';

    -- MOV instruction
    Instr(31 downto 20) <= X"E3A"; 
    wait for CLK_period;

    -- ADDi instruction
    Instr(31 downto 20) <= X"E28"; 
    wait for CLK_period;

    -- ADDr instruction
    Instr(31 downto 20) <= X"E08"; 
    wait for CLK_period;

    -- CMP instruction
    Instr(31 downto 20) <= X"E35"; 
    wait for CLK_period;

    -- LDR instruction
    Instr(31 downto 20) <= X"E61"; 
    wait for CLK_period;
  
    -- STR instruction
    Instr(31 downto 20) <= X"E60"; 
    wait for CLK_period;
    
    -- BAL instruction
    Instr(31 downto 20) <= X"EAF"; 
    wait for CLK_period;
    
    -- BLT instruction
    Instr(31 downto 20) <= X"BAF"; 
    wait for CLK_period;

    wait;
  end process;

end Behavioral;

