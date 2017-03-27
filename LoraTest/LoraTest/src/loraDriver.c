/*
 * loraDriver.c
 *
 * Created: 3/24/2017 12:08:18 PM
 *  Author: mazacar8
 */

#include "loraDriver.h"
#include "uart.h"
#include <stdio.h>
#include <asf.h>
#include "saml21j18b.h"

uint8_t reset_N = PIN_PB00;

void lora_setup()
{
	
	uint8_t msg[] = "Setting up LORA...\r\n";
    usart_write_buffer_wait(&usart_instance,msg,sizeof(msg));
	port_get_config_defaults(&reset_config);
	reset_config.direction = PORT_PIN_DIR_OUTPUT;
	reset_config.input_pull = PORT_PIN_PULL_NONE;
	reset_config.powersave = false;
	port_pin_set_config(reset_N,&reset_config);
	port_pin_set_output_level(reset_N,LOW);
	port_pin_set_output_level(reset_N,HIGH);
	delay_ms(8000);
	
	uint8_t cmd[] = "mac set appeui 0000000000000100\r\n";
	send_command(cmd,sizeof(cmd)-1);
	uint8_t cmd1[] = "mac set deveui 1122334455667799\r\n";
	send_command(cmd1,sizeof(cmd1)-1);
	uint8_t cmd2[] = "mac set appkey 2b7e151628aed2a6abf7156669cf4f3c\r\n";
	send_command(cmd2,sizeof(cmd2)-1);
	uint8_t cmd3[] = "mac set dr 0\r\n";
	send_command(cmd3,sizeof(cmd3)-1);
	uint8_t cmd4[] = "mac set pwridx 5\r\n";
	send_command(cmd4,sizeof(cmd4)-1);
	uint8_t cmd5[] = "mac set adr off\r\n";
	send_command(cmd5,sizeof(cmd5)-1);

	char cmd6[25];
	for (int i = 0; i < 72; i++) {
		if (i < 8){
			sprintf(cmd6,"mac set ch status %d on\r\n",i);
			send_command_noack(cmd6,sizeof(cmd6)-1);
		} 
		else{
			sprintf(cmd6,"mac set ch status %d off\r\n",i);
			send_command_noack(cmd6,sizeof(cmd6)-1);
		}
		
		delay_ms(300);
	}
	
	uint8_t cmd7[] = "mac set devaddr 00000000\r\n";
	send_command(cmd7,sizeof(cmd7)-1);
	uint8_t cmd8[] = "radio set pwr 20\r\n";
	send_command(cmd8,sizeof(cmd8)-1);
	uint8_t cmd9[] = "radio set prlen 8\r\n";
	send_command(cmd9,sizeof(cmd9)-1);
	uint8_t cmd10[] = "mac save\r\n";
	send_command(cmd10,sizeof(cmd10)-1);
	
	try_join();
	delay_ms(10000);
	
}

void try_join() {
	int i,j;
	uint8_t cmd[] = "mac join otaa\r\n";
	for (j = 0; j < 1; j++) {
		send_command(cmd,sizeof(cmd)-1);
		for (i = 0; i < 3; i++) {
			read_response();
			delay_ms(5000);
		}
	}
}

