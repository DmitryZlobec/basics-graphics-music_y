// SHoul be added 
module tb;

    logic        clk;
    logic        rst;

    wire  [31:0] imAddr;   // instruction memory address
    wire  [31:0] imData;   // instruction memory data

    logic [ 4:0] regAddr;  // debug access reg address
    wire  [31:0] regData;  // debug access reg data

    sr_cpu cpu
    (
        .clk     ( clk     ),
        .rst     ( rst     ),

        .imAddr  ( imAddr  ),
        .imData  ( imData  ),

        .regAddr ( regAddr ),
        .regData ( regData )
    );

    instruction_rom # (.SIZE (1024)) rom
    (
        .a       ( imAddr  ),
        .rd      ( imData  )
    );

    //------------------------------------------------------------------------

    initial
    begin
        clk = 1'b0;

        forever
            # 5 clk = ~ clk;
    end

    //------------------------------------------------------------------------

    initial
    begin
        rst <= 1'bx;
        repeat (2) @ (posedge clk);
        rst <= 1'b1;
        repeat (2) @ (posedge clk);
        rst <= 1'b0;
    end

    //------------------------------------------------------------------------

    initial
    begin
        `ifdef __ICARUS__
            // Uncomment the following `define
            // to generate a VCD file and analyze it using GTKwave

           $dumpvars;
        `endif
endmodule
