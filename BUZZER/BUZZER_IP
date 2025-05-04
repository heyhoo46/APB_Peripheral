`timescale 1ns / 1ps

module Buzzer_Periph (
    input  logic        PCLK,
    input  logic        PRESET,
    input  logic [ 4:0] PADDR,
    input  logic [31:0] PWDATA,
    input  logic        PWRITE,
    input  logic        PENABLE,
    input  logic        PSEL,
    output logic [31:0] PRDATA,
    output logic        PREADY,
    output logic        buzzer_out
);

    logic        odr;
    logic [31:0] duty_reg;
    logic [31:0] period_reg;
    logic        mode_pwm;
    logic        pwm_out;
    
    assign buzzer_out = (mode_pwm) ? pwm_out : odr;
    
    APB_SlaveIntf_Buzzer U_APB_Intf (
        .PCLK, .PRESET, .PADDR, .PWDATA, .PWRITE, .PENABLE, .PSEL,
        .PRDATA, .PREADY,
        .odr, .duty_reg, .period_reg, .mode_pwm
    );
    
    PWM_Generator U_PWM (
        .clk(PCLK),
        .rst(PRESET),
        .duty(duty_reg),
        .period(period_reg),
        .pwm_out(pwm_out)
    );
endmodule

module APB_SlaveIntf_Buzzer (
    input  logic        PCLK,
    input  logic        PRESET,
    input  logic [4:0]  PADDR,
    input  logic [31:0] PWDATA,
    input  logic        PWRITE,
    input  logic        PENABLE,
    input  logic        PSEL,
    output logic [31:0] PRDATA,
    output logic        PREADY,
    output logic        odr,
    output logic [31:0] duty_reg,
    output logic [31:0] period_reg,
    output logic        mode_pwm
);

    logic [31:0] slv_reg0;
    logic [31:0] slv_reg1;
    logic [31:0] slv_reg2;
    logic [31:0] slv_reg3;
    logic [31:0] slv_reg4;

    assign odr        = slv_reg1[0];
    assign duty_reg   = slv_reg2;
    assign period_reg = slv_reg3;
    assign mode_pwm   = slv_reg4[0];
    
    always_ff @(posedge PCLK or posedge PRESET) begin
        if (PRESET) begin
            slv_reg0 <= 0;
            slv_reg1 <= 0;
            slv_reg2 <= 0;
            slv_reg3 <= 1000;
            slv_reg4 <= 0;
            PREADY   <= 0;
            PRDATA   <= 0;
        end else begin
            PREADY <= 1'b0;
            if (PSEL && PENABLE) begin
                PREADY <= 1'b1;
                if (PWRITE) begin
                    case (PADDR[4:2])
                        3'd0: slv_reg0 <= PWDATA;
                        3'd1: slv_reg1 <= PWDATA;
                        3'd2: slv_reg2 <= PWDATA;
                        3'd3: slv_reg3 <= PWDATA;
                        3'd4: slv_reg4 <= PWDATA;
                        default: ;
                    endcase
                end else begin
                    case (PADDR[4:2])
                        3'd0: PRDATA <= slv_reg0;
                        3'd1: PRDATA <= slv_reg1;
                        3'd2: PRDATA <= slv_reg2;
                        3'd3: PRDATA <= slv_reg3;
                        3'd4: PRDATA <= slv_reg4;
                        default: PRDATA <= 0;
                    endcase
                end
            end
        end
    end
endmodule

module PWM_Generator (
    input  logic        clk,
    input  logic        rst,
    input  logic [31:0] duty,
    input  logic [31:0] period,
    output logic        pwm_out
);

    logic [31:0] cnt;
    logic [15:0] clk_div;
    logic       clk_en;
    
    always_ff @(posedge clk or posedge rst) begin
        if (rst) begin
            clk_div <= 0;
            clk_en <= 0;
        end else begin
            if (clk_div >= 16'd1000) begin
                clk_div <= 0;
                clk_en <= 1;
            end else begin
                clk_div <= clk_div + 1;
                clk_en <= 0;
            end
        end
    end
    
    always_ff @(posedge clk or posedge rst) begin
        if (rst) begin
            cnt <= 0;
            pwm_out <= 0;
        end else if (clk_en) begin
            if (cnt >= period)
                cnt <= 0;
            else
                cnt <= cnt + 1;
                
            pwm_out <= (cnt < duty);
        end
    end
endmodule
