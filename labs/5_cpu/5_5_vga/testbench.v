`timescale 1 ns / 1 ns

`define CLK_FREQUENCY (50 * 1000 * 1000)

module testbench;

    logic clk;
      reg rst;
    logic [11:0] x;
    logic [11:0] y;
    logic [9:0] CounterX=0, CounterY=0;

    wire [12:0] text_symbol;
    reg [12:0] text_symbol_r;
    
    wire [11:0] ry;
  
  initial
  begin
    clk = '0;
    forever # 1 clk = ~ clk;
  end

   
    initial
  begin
    `ifdef __ICARUS__
      $dumpvars;
    `endif
        rst = 1;
        repeat(2) @(posedge clk);
        rst = 0;

    
    for (int i = 0; i < 10000; i ++)
    begin
             @(posedge clk) CounterX <= (CounterX==799) ? 0 : CounterX+1;  
             @(posedge clk) if(CounterX==799) CounterY <= (CounterY==524) ? 0 : CounterY+1;
    end

 



    $display("\n");
    `ifdef MODEL_TECH  // Mentor ModelSim and Questa
      $stop;
    `else
      $finish;
    `endif
  end

    my_module uut (
        .clk(clk),
        .rst(rst),
        .x(CounterX),
        .y(CounterY),
        .text_symbol(text_symbol),
        .ry(ry)
    );

            always @(clk) begin
            text_symbol_r<=text_symbol;
            end
  
    initial begin
        $dumpfile("simulation.vcd");
        $dumpvars(0,testbench);
    end
endmodule