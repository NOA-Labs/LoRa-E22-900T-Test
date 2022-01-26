#This is the test code for LoRa E22-900T

IDE:  
	vscode + platformide + arduino  
	
Target board:  
	ESP32.  

in System_cfg.h  
	...  
	#define SYS_LORA_IS_HOST_MODE               1  
	#define SYS_LORA_IS_SLAVE_MODE              ((SYS_LORA_IS_HOST_MODE == 0) ? 1 : 0)  
	  
	if the macro SYS_LORA_IS_HOST_MODE is 1, the host mode enable.
	otherwise, the slave mode enable.