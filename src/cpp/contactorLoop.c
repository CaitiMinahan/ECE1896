#include <stdio.h>
// Include any chip specific libraries here
// Include SOH, SOC Model Libraries here

void EPO(){
	// Emergency Power OFF of the System
}

void BMS_Wake_Up(){
	// ECU Power On Communications
	// BMS Wakes UP
}

int32_t Fault_Check(){
	// Check each pin with a fault flag on GPIO
	// Return status which can be bit flagged
	// for each individual fault
	// Bits will be a 1 if there is a fault
}

int8_t PreCharge_Contactor_Read(){
	// Read the GPIO Pins associated with the 
	// Pre-Charge Contactor
}

void PreCharge_Contactor_Write(bool LSD, bool HSD){
	// Write to the GPIO Pins associated with the 
	// Pre-Charge Contactor
}

bool Vehicle_Voltage_Check(){
	// Vehicle/ECU Voltage >= 90% of Battery Pack Voltage
}

int8_t Negative_Contactor_Read(){
	// Read the GPIO Pins associated with the 
	// Negative Contactor
}

void Negative_Contactor_Write(bool LSD, bool HSD){
	// Write to the GPIO Pins associated with the 
	// Negative Contactor
}

int8_t Positive_Contactor_Read(){
	// Read the GPIO Pins associated with the 
	// Positive Contactor
}

void Positive_Contactor_Write(bool LSD, bool HSD){
	// Write to the GPIO Pins associated with the 
	// Positive Contactor
}

void init() {
	
	// LOW_VOLTAGE_REQUEST up to 3 retries
	for (int i = 0; i < 4; i++){
		LOW_VOLTAGE_REQUEST = GPIO_PIN->2;
		if (LOW_VOLTAGE_REQUEST == 1){
			break;
		} else if (i == 3){
			printf("Low Voltage Request Failed... Starting EPO!");
			EPO(); // Shut the System Down Immediatley 
		} else {
			SysTickWait1ms(1000); // Wait a few seconds to do the request
		}
	}
	
	// Turn on the BMS
	BMS_Wake_Up();
	
	// Check for Intial Faults
	int32_t fault_status = Fault_Check();
	if (fault_status != 0) { EPO(); } // Power OFF if there's a fault
	
	// Check if Pre-Charge Contactor Closed
	int8_t precharge_status;
	for (int i = 0; i < 4; i++){
		// Enable the Pre-Charge Contactor
		PreCharge_Contactor_Write(1,1); // Close Low-Side, High-Side Driver
		precharge_status = PreCharge_Contactor_Read();
		if (precharge_status == 1){
			break;
		} else if (i == 3){
			printf("Pre-Charge Contactor Close Request Failed... Starting EPO!");
			EPO(); // Shut the System Down Immediatley 
		} else {
			SysTickWait1ms(1000); // Wait a few seconds to do the request
		}
	}
	
	// Vehicle/ECU Voltage >= 90% of Battery Pack Voltage
	SysTickWait1ms(1000*10); // Wait 10 seconds
	bool vehicle_status = Vehicle_Voltage_Check();
	if (vehicle_status != 0) { EPO(); } // Power OFF if there's a fault
	
	// Check if Negative Contactor Closed
	int8_t negative_status;
	for (int i = 0; i < 4; i++){
		// Enable the Negative Contactor
		Negative_Contactor_Write(1,1); // Close Low-Side, High-Side Driver
		negative_status = Negative_Contactor_Read();
		if (negative_status == 1){
			break;
		} else if (i == 3){
			printf("Negative Contactor Close Request Failed... Starting EPO!");
			EPO(); // Shut the System Down Immediatley 
		} else {
			SysTickWait1ms(1000); // Wait a few seconds to do the request
		}
	}
	
	// Check if Positive Contactor Closed
	int8_t positive_status;
	for (int i = 0; i < 4; i++){
		// Enable the Positive Contactor
		Positive_Contactor_Write(1,1); // Close Low-Side, High-Side Driver
		positive_status = Positive_Contactor_Read();
		if (positive_status == 1){
			break;
		} else if (i == 3){
			printf("Positive Contactor Close Request Failed... Starting EPO!");
			EPO(); // Shut the System Down Immediatley 
		} else {
			SysTickWait1ms(1000); // Wait a few seconds to do the request
		}
	}
	
	// Check if Pre-Charge Contactor Open
	for (int i = 0; i < 4; i++){
		// Enable the Pre-Charge Contactor
		PreCharge_Contactor_Write(0,0); // Close Low-Side, High-Side Driver
		precharge_status = PreCharge_Contactor_Read();
		if (precharge_status == 0){
			break;
		} else if (i == 3){
			printf("Pre-Charge Contactor Open Request Failed... Starting EPO!");
			EPO(); // Shut the System Down Immediatley 
		} else {
			SysTickWait1ms(1000); // Wait a few seconds to do the request
		}
	}
	
	// Check for Final Faults
	fault_status = Fault_Check();
	if (fault_status != 0) { EPO(); } // Power OFF if there's a fault
	
}

void Safe_Shut_Down(){
	
	// Check if Positive Contactor Open
	int8_t positive_status;
	for (int i = 0; i < 4; i++){
		// Enable the Positive Contactor
		Positive_Contactor_Write(0,0); // Close Low-Side, High-Side Driver
		positive_status = Positive_Contactor_Read();
		if (positive_status == 0){
			break;
		} else if (i == 3){
			printf("Positive Contactor Close Request Failed... Starting EPO!");
			EPO(); // Shut the System Down Immediatley 
		} else {
			SysTickWait1ms(1000); // Wait a few seconds to do the request
		}
	}
	
	// Check if Negative Contactor Open
	int8_t negative_status;
	for (int i = 0; i < 4; i++){
		// Enable the Negative Contactor
		Negative_Contactor_Write(0,0); // Close Low-Side, High-Side Driver
		negative_status = Negative_Contactor_Read();
		if (negative_status == 0){
			break;
		} else if (i == 3){
			printf("Negative Contactor Close Request Failed... Starting EPO!");
			EPO(); // Shut the System Down Immediatley 
		} else {
			SysTickWait1ms(1000); // Wait a few seconds to do the request
		}
	}
	
	// Check for Final Final Faults
	fault_status = Fault_Check();
	if (fault_status != 0) { Log_Faults(); } // Save Faults to Disk
	
	bool sleep = BMS_Sleep();
	if (!sleep) { EPO(); } // BMS Fails Sleep, EPO
	
	//Low Voltage Power Off and Disable Vehicle/ECU Comms
	bool lf_pwr_off = LV_Power_OFF();
	if (!lv_pwr_off) { 
		Log_Faults(); // Report to User for Immediate Service
		EPO(); // Shut the System Down Immediatley 
	}

}

void main() {
	
	// Power ON Request from GPIO Button
	PWR_ON_REQUEST = GPIO_PIN->1;
	PWR_OFF_REQUEST = GPIO_PIN->3;
	
	int32_t fault_status;
	
	if (PWR_ON_REQUEST){
		// Initialize the BMS
		init();
	
		// Run Fault Check Loop
		while(1){
			// Check for Final Faults
			fault_status = Fault_Check();
			if (fault_status != 0) { EPO(); } // Power OFF if there's a fault
			
			if (PWR_OFF_REQUEST){
				Safe_Shut_Down();
				break; // : )
			}
		}
	} else {
		printf("Waiting for Power On Request...");
	}

}