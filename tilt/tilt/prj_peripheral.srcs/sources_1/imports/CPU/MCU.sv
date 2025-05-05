`timescale 1ns / 1ps

module MCU (
    input  logic       clk,
    input  logic       reset,
    // output logic [7:0] GPOA,
    // input  logic [7:0] GPIB,
    // inout  logic [7:0] GPIOC,
    inout  logic [7:0] GPIOA,
    // inout  logic [7:0] GPIOB,
    // output logic [7:0] fndFont,
    // output logic [3:0] fndCom,
    input  logic       tilt_sensor
    // output logic       red,
    // output logic       green,
    // output logic       blue
);
    // global signals
    logic        PCLK;
    logic        PRESET;
    // APB Interface Signals
    logic [31:0] PADDR;
    logic [31:0] PWDATA;
    logic        PWRITE;
    logic        PENABLE;
    logic        PSEL_RAM; 
    logic        PSEL_TILT;
    logic        PSEL_GPIOA;
    // logic        PSEL_GPIOB;
    logic        PSEL_RGB;
    logic [31:0] PRDATA_RAM;
    logic [31:0] PRDATA_TILT;
    logic [31:0] PRDATA_GPIOA;
    // logic [31:0] PRDATA_GPIOB;
    logic [31:0] PRDATA_RGB;
    logic        PREADY_RAM; 
    logic        PREADY_TILT;
    logic        PREADY_GPIOA;
    // logic        PREADY_GPIOB;
    logic        PREADY_RGB;

    // CPU - APB_Master Signals
    // Internal Interface Signals
    logic        transfer;  // trigger signal
    logic        ready;  
    logic [31:0] addr;
    logic [31:0] wdata;
    logic [31:0] rdata;
    logic        write;  // 1:write, 0:read
    logic        dataWe;
    logic [31:0] dataAddr;
    logic [31:0] dataWData;
    logic [31:0] dataRData;

    // ROM Signals
    logic [31:0] instrCode;
    logic [31:0] instrMemAddr;

    assign PCLK = clk;
    assign PRESET = reset;
    assign addr = dataAddr;
    assign wdata = dataWData;
    assign dataRData = rdata;
    assign write = dataWe;

    rom U_ROM (
        .addr(instrMemAddr),
        .data(instrCode)
    );

    RV32I_Core U_Core (.*);

    APB_Master U_APB_Master (
        .*,
        .PSEL0  (PSEL_RAM), 
        .PSEL1  (),
        .PSEL2  (PSEL_GPIOA),
        .PSEL3  (PSEL_TILT),
        .PSEL4  (),
        .PSEL5  (),
        .PRDATA0(PRDATA_RAM),
        .PRDATA1(),
        .PRDATA2(PRDATA_GPIOA),
        .PRDATA3(PRDATA_TILT),
        .PRDATA4(),
        .PRDATA5(),
        .PREADY0(PREADY_RAM),
        .PREADY1(),
        .PREADY2(PREADY_GPIOA),
        .PREADY3(PREADY_TILT),
        .PREADY4(),
        .PREADY5()
    );

    ram U_RAM (
        .*,
        .PSEL  (PSEL_RAM),
        .PRDATA(PRDATA_RAM),
        .PREADY(PREADY_RAM)
    );

    tilt U_Tilt (
        .*,
        .PSEL  (PSEL_TILT),
        .PRDATA(PRDATA_TILT),
        .PREADY(PREADY_TILT)
    );

    GPIO_Periph U_GPIOA (
        .*,
        .PSEL     (PSEL_GPIOA),
        .PRDATA   (PRDATA_GPIOA),
        .PREADY   (PREADY_GPIOA),
        .inOutPort(GPIOA)
    );

    // GPIO_Periph U_GPIOB (
    //     .*,
    //     .PSEL     (PSEL_GPIOB),
    //     .PRDATA   (PRDATA_GPIOB),
    //     .PREADY   (PREADY_GPIOB),
    //     .inOutPort(GPIOB)
    // );

    // rgb_led U_RGB (
    //     .*,
    //     .PSEL  (PSEL_RGB),
    //     .PRDATA(PRDATA_RGB),
    //     .PREADY(PREADY_RGB)
    // );


endmodule


