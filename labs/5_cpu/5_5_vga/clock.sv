module ClockModule (
    input wire clk_27,        
    input wire reset,            
    output reg [15:0] clock_out  
);

    reg [25:0] counter;
    wire clk_100Hz;

    always @(posedge clk_27 or posedge reset) begin
        if (reset) begin
            counter <= 26'd0;
        end else if (counter >= 26'd269999) begin
            counter <= 26'd0;
        end else begin
            counter <= counter + 1;
        end
    end

    assign clk_100Hz = (counter == 26'd269999);

    reg [15:0] seconds_counter;

    always @(posedge clk_27 or posedge reset) begin
        if (reset) begin
            seconds_counter <= 32'd0;
        end else if (clk_100Hz) begin
            seconds_counter <= seconds_counter + 1;
        end
    end

    always @(posedge clk_27 or posedge reset) begin
        if (reset) begin
            clock_out <= 32'd0;
        end else begin
            clock_out <= seconds_counter;
        end
    end

endmodule