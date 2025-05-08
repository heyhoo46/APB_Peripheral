
module SOUND_Periph (
    // global signal
    input  logic        PCLK,
    input  logic        PRESET,
    // APB Interface Signals
    input  logic [ 3:0] PADDR,
    input  logic [31:0] PWDATA,
    input  logic        PWRITE,
    input  logic        PENABLE,
    input  logic        PSEL,
    output logic [31:0] PRDATA,
    output logic        PREADY,
    // export signals
    output logic        buzzer_tone
    
);


    SOUND_Intf(.*);
   
endmodule

module SOUND_Intf (
    // global signal
    input  logic        PCLK,
    input  logic        PRESET,
    // APB Interface Signals
    input  logic [ 3:0] PADDR,
    input  logic [31:0] PWDATA,
    input  logic        PWRITE,
    input  logic        PENABLE,
    input  logic        PSEL,
    output logic [31:0] PRDATA,
    output logic        PREADY,
    // internal signals

    output logic buzzer_tone //출력으로 나갈 buzzer tone
);

logic       tick;
logic [3:0] tone_sel;
logic [7:0] tick_value;

clk_div_1khz U_1KHZ_tick (
    .PCLK(PCLK),
    .PRESET(PRESET),
    .tick_1khz(tick)
);

tone_rom U_rom (
    .tone_sel(tone_sel),
    .tick_value(tick_value)
);

buzzer_sound_gen U_buzzer_sound_gen (
    .PCLK(PCLK),
    .PRESET(PRESET),
    .tick(tick),
    .tone_sel(tone_sel),
    .tick_value(tick_value),
    .buzzer_tone(buzzer_tone)
);

    logic [31:0] slv_reg0;//, slv_reg1; //, slv_reg2, slv_reg3;



    assign tone_sel   = slv_reg0[3:0];

    always_ff @(posedge PCLK, posedge PRESET) begin
        if (PRESET) begin
            slv_reg0 <= 0;
            //slv_reg1 <= 0;
            // slv_reg2 <= 0;
            // slv_reg3 <= 0;
        end else begin
            if (PSEL && PENABLE) begin
                PREADY <= 1'b1;
                if (PWRITE) begin
                    case (PADDR[3:2])
                        2'd0: slv_reg0 <= PWDATA;
                       // 2'd1: slv_reg1 <= PWDATA;
                        // 2'd2: slv_reg2 <= PWDATA;
                        // 2'd3: slv_reg3 <= PWDATA;
                    endcase
                end else begin
                    PRDATA <= 32'bx;
                    case (PADDR[3:2])
                        2'd0: PRDATA <= slv_reg0;
                        //2'd1: PRDATA <= slv_reg1;
                        // 2'd2: PRDATA <= slv_reg2;
                        // 2'd3: PRDATA <= slv_reg3;
                    endcase
                end
            end else begin
                PREADY <= 1'b0;
            end
        end
    end

endmodule






module buzzer_sound_gen (
    input  logic        PCLK,
    input  logic        PRESET,
    input  logic        tick,           // 1ms 주기의 tick
    input  logic [3:0]  tone_sel,       // 선택된 음
    output logic        buzzer_tone,
    input  logic [7:0] tick_value             // 해당 음에 대한 반주기 tick
);

    logic [7:0] counter;
    logic       buzzer;



    assign buzzer_tone = buzzer;

    always_ff @(posedge PCLK or posedge PRESET) begin
        if (PRESET) begin
            counter <= 0;
            buzzer  <= 0;
        end else if (tick) begin
            if (tick_value != 0) begin
                if (counter >= tick_value - 1) begin
                    counter <= 0;
                    buzzer <= ~buzzer;
                end else begin
                    counter <= counter + 1;
                end
            end else begin
                counter <= 0;
                buzzer  <= 0;
            end
        end
    end

    
    // 1초 = 1000 ticks (if tick = 1ms)
    // 반 주기 tick 수 = 1000 / (2 * f)
    // DO(33Hz): 15 ticks, RE(37Hz): 13, MI(41Hz): 12


endmodule


module clk_div_1khz (
    input  logic        PCLK,        // 100MHz
    input  logic        PRESET,
    output logic        tick_1khz    // 1ms
);

    // 100_000 clock cycles = 1ms at 100MHz
    logic [$clog2(100_000)-1:0] div_counter;

    always_ff @(posedge PCLK or posedge PRESET) begin
        if (PRESET) begin
            div_counter <= 0;
            tick_1khz <= 1'b0;
        end else begin
            if (div_counter == 100_000 - 1) begin
                div_counter <= 0;
                tick_1khz <= 1'b1;
            end else begin
                div_counter <= div_counter + 1;
                tick_1khz <= 1'b0;
            end
        end
    end

endmodule


//tone select module
module tone_rom (
    input  logic [3:0] tone_sel,         
    output logic [7:0] tick_value        // 반 주기 tick (1ms 기준)
);
    always_comb begin
        case (tone_sel)
            4'd0:  tick_value = 15;  // C1
            4'd1:  tick_value = 14;  // C#1
            4'd2:  tick_value = 13;  // D1
            4'd3:  tick_value = 12;  // D#1
            4'd4:  tick_value = 12;  // E1
            4'd5:  tick_value = 11;  // F1
            4'd6:  tick_value = 11;  // F#1
            4'd7:  tick_value = 10;  // G1
            4'd8:  tick_value = 10;  // G#1
            4'd9:  tick_value = 9;   // A1
            4'd10: tick_value = 9;   // A#1
            4'd11: tick_value = 8;   // B1
            default: tick_value = 0; // 무음
        endcase
    end
endmodule


