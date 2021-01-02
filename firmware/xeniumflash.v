/* 4-bit "BitBus" to Flash translator for OpenXenium

Copyright (C) 2019-2020 Koos du Preez (kdupreez@hotmail.com)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


module xeniumflash(
			// led
			output reg [2:0] led,

			//bitbus in/out
			inout	[3:0]	bitbus_data,
			input 			bitbus_clk,
			input 			bitbus_mode,

			//flash in/out
			output	[20:0]	flash_addr,
			inout		[7:0] 	flash_data,
			output reg flash_oe,
			output reg flash_we
			);

	// state machine
	`define flash_addr_3_0 0
	`define flash_addr_7_4 1
	`define flash_addr_11_8 2
	`define flash_addr_15_12 3
	`define flash_addr_19_16 4
	`define flash_addr_23_20 5
	`define flash_addr_end 6
	`define flash_data_3_0 7
	`define flash_data_7_4 8
	`define flash_data_end 9
	reg [4:0] flash_state;
	// bitbus
	reg [3:0] bitbus_data_d;
	reg bitbus_mode_q;
	// flash 
	reg [20:0] flash_addr_d;
	reg [7:0] flash_data_d;

	//initialization
	initial begin
		//internal state
		flash_state = 5'b`flash_addr_3_0;		
		led = 3'b001; //led[R|G|B]
		//flash 
		flash_addr_d = 21'b0;
		flash_data_d = 8'b0;
		flash_oe = 1'b1;
		flash_we = 1'b1;
		//bitbus
		bitbus_data_d = 4'b0;
		bitbus_mode_q = 1'b0;
	end
	
	// flash assigns
	assign flash_addr = flash_addr_d;
	assign flash_data[7:0] = (flash_we == 0) ? flash_data_d[7:0] : 8'bZ;
	//bitbus assigns
	assign bitbus_data = (flash_oe == 0) ? bitbus_data_d[3:0] : 4'bZ;

	// process bitbus clock
	always @(posedge bitbus_clk)
	begin
		// change in modes = reset
		if (bitbus_mode_q != bitbus_mode)
		begin
			// state reset
			flash_state <= `flash_addr_3_0;
			led <= 3'b001; 
			//bitbus reset
			bitbus_mode_q <= bitbus_mode;	
			//flash reset
			flash_oe <= 1;
			flash_we <= 1;
		end		
		//  if not reset traverse through state machine 
		//  once per bitbus clock to set address and data.
		else 
		begin			
			// green = read, red = write
			led[0] <= 0;
			led[1] <= bitbus_mode;
			led[2] <= ~bitbus_mode;			
			case (flash_state)		
				// clock lower nibble of address byte 1
				`flash_addr_3_0: begin
					flash_addr_d[3:0] <= bitbus_data[3:0];
					flash_state <= `flash_addr_7_4;
				end
				// clock upper nibble of address byte 1
				`flash_addr_7_4: begin
					flash_addr_d[7:4] <= bitbus_data[3:0];
					flash_state <= `flash_addr_11_8;
				end				
				// clock lower nibble of address byte 2
				`flash_addr_11_8: begin
					flash_addr_d[11:8] <= bitbus_data[3:0];
					flash_state <= `flash_addr_15_12;
				end				
				// clock upper nibble of address byte 2
				`flash_addr_15_12: begin
					flash_addr_d[15:12] <= bitbus_data[3:0];
					flash_state <= `flash_addr_19_16;
				end				
				// clock lower nibble of address byte 3
				`flash_addr_19_16: begin
					flash_addr_d[19:16] <= bitbus_data[3:0];
					flash_state <= `flash_addr_23_20;
				end				
				// clock upper nibble of address byte 3
				`flash_addr_23_20: begin
					flash_addr_d[20] <= bitbus_data[0];
					flash_state <= `flash_addr_end;
				end				
				//clock WE and OE
				`flash_addr_end: begin
					flash_we <= bitbus_mode; 
					flash_oe <= ~bitbus_mode;					
					flash_state <= `flash_data_3_0;
				end
				// clock lower nibble of data byte
				`flash_data_3_0: begin
					if (bitbus_mode==0)
						flash_data_d[3:0] <= bitbus_data[3:0]; //WRITE
					else
						bitbus_data_d[3:0] <= flash_data[3:0]; //READ
					flash_state <= `flash_data_7_4;
				end
				// clock upper nibble of data byte
				`flash_data_7_4: begin
					if (bitbus_mode==0) 
						flash_data_d[7:4] <= bitbus_data[3:0]; //WRITE
					else 
						bitbus_data_d[3:0] <= flash_data[7:4]; //READ
					flash_state <= `flash_data_end;
				end
				//clock WE and OE
				`flash_data_end: begin					
					flash_oe <= 1;
					flash_we <= 1;
					flash_state <= `flash_addr_3_0;
				end		
				endcase							
		end //end else change in dir		
	end // and always bitbus clock

endmodule
