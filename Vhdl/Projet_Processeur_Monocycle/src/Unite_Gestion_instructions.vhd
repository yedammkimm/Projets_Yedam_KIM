library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Unite_Gestion_instructions is
    port(
        Offset: in std_logic_vector(23 downto 0);
        nPCSel, CLK, Reset: in std_logic;
        Instruction: out std_logic_vector(31 downto 0)
    );
end entity;

architecture Behavioral of Unite_Gestion_instructions is
    signal Extend_out: std_logic_vector(31 downto 0);
    signal PC: std_logic_vector(31 downto 0);
begin

    Extend: entity work.PC_Extender(Behavioral)
        port map (Offset => Offset, Offset_out => Extend_out);

    Sel: entity work.PC_sel(Behavioral)
        port map (CLK => CLK, Reset => Reset, nPCsel => nPCsel, Offset_in => Extend_out, PC => PC);

    IRQ: entity work.instruction_memory_irq(Behavioral)
        port map (PC => PC(5 downto 0), Instruction => Instruction);

end Behavioral;
