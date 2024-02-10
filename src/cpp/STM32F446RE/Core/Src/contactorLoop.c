#include <stdio.h>
// Include SOH, SOC Model Libraries here

// Structs for Failure Modes
typedef struct {
    bool Cell_OverVoltage;              // >= 4.2V
    bool Cell_UnderVoltage;             // <= 2.85V

    bool Cell_OverTemperature;          // >= 60C
    bool Cell_UnderTemperature;         // <= 0C

    bool Module_OverVoltage;            // >= (# of Cells in Series * 4.2V)
    bool Module_UnderVoltage;           // <= (# of Cells in Series * 2.85V)

    bool Pack_OverVoltage;              // >= (# of Modules in Series * # of Cells in Series * 4.2V)
    bool Pack_UnderVoltage;             // <= (# of Modules in Series * # of Cells in Series * 2.85V)
    bool Pack_OverCurrent;              // >= Power Limit / Pack Voltage

    bool Power_Fault;                   // >= Power Limit / Pack Voltage
    bool Isolation_Fault_Resistance;    // <= 500 Ohms/V

    bool SOH_Fault;                     // <= 70%
    bool SOC_Warning;                   // <= 30%
    bool SOC_Fault;                     // <= 5%

    bool PreC_HSD_Stuck_OFF;            // Pre-Charge High-Side Driver Stuck OFF
    bool PreC_LSD_Stuck_OFF;            // Pre-Charge Low-Side Driver Stuck OFF
    bool PreC_CON_Stuck_OFF;            // Pre-Charge Contactor Stuck OFF
    bool PreC_HSD_Stuck_ON;             // Pre-Charge High-Side Driver Stuck ON
    bool PreC_LSD_Stuck_ON;             // Pre-Charge Low-Side Driver Stuck ON
    bool PreC_CON_Stuck_ON;             // Pre-Charge Contactor Stuck ON

    bool Pos_HSD_Stuck_OFF;             // Positive High-Side Driver Stuck OFF
    bool Pos_LSD_Stuck_OFF;             // Positive Low-Side Driver Stuck OFF
    bool Pos_CON_Stuck_OFF;             // Positive Contactor Stuck OFF
    bool Pos_HSD_Stuck_ON;              // Positive High-Side Driver Stuck ON
    bool Pos_LSD_Stuck_ON;              // Positive Low-Side Driver Stuck ON
    bool Pos_CON_Stuck_ON;              // Positive Contactor Stuck ON

    bool Neg_HSD_Stuck_OFF;             // Negative High-Side Driver Stuck OFF
    bool Neg_LSD_Stuck_OFF;             // Negative Low-Side Driver Stuck OFF
    bool Neg_CON_Stuck_OFF;             // Negative Contactor Stuck OFF
    bool Neg_HSD_Stuck_ON;              // Negative High-Side Driver Stuck ON
    bool Neg_LSD_Stuck_ON;              // Negative Low-Side Driver Stuck ON
    bool Neg_CON_Stuck_ON;              // Negative Contactor Stuck ON
} Faults;

// Structs for GPIO Pins
typedef struct {
    bool PWR_ON_REQUEST;                // Power On Request
    bool PWR_OFF_REQUEST;               // Power Off Request
    bool LOW_VOLTAGE_REQUEST;           // Low Voltage Request
    bool PREC_HSD_CON;                  // Pre-Charge High-Side Driver
    bool PREC_LSD_CON;                  // Pre-Charge Low-Side Driver
    bool PREC_CON_CON;                  // Pre-Charge Contactor
    bool POS_HSD_CON;                   // Positive High-Side Driver
    bool POS_LSD_CON;                   // Positive Low-Side Driver
    bool POS_CON_CON;                   // Positive Contactor
    bool NEG_HSD_CON;                   // Negative High-Side Driver
    bool NEG_LSD_CON;                   // Negative Low-Side Driver
    bool NEG_CON_CON;                   // Negative Contactor
} GPIO;

// Structs for CAN Messages
typedef struct {
    uint8_t ID;                         // CAN Message ID
    uint8_t DLC;                        // CAN Message Data Length
    uint8_t DATA[8];                    // CAN Message Data
} CAN;

// Structs for BMS State
typedef struct {
    bool BMS_ON;                        // BMS Power Status
    bool BMS_SLEEP;                     // BMS Sleep Status
    bool ECU_ON;                        // ECU Power Status
    bool LV_POWER_OFF;                  // Low Voltage Power Off Status
    int32_t FAULT;                      // BMS Fault Status
    int32_t VEHICLE_VOLTAGE;            // Vehicle Voltage Status
    int32_t VEHICLE_CURRENT;            // Vehicle Current Status
    int32_t VEHICLE_TEMPERATURE;        // Vehicle Temperature Status
    int32_t BATTERY_VOLTAGE;            // Battery Voltage Status
    int32_t BATTERY_CURRENT;            // Battery Current Status
    int32_t BATTERY_TEMPERATURE;        // Battery Temperature Status
    int32_t CELL_SOC;                   // Battery State of Charge Status
    int32_t CELL_SOH;                   // Battery State of Health Status
} BMS;

void EPO(void){
	// Emergency Power OFF of the System
}

void BMS_Wake_Up(void){
	// ECU Power On Communications
	// BMS Wakes UP
}

int32_t Fault_Check(void){
	// Check each pin with a fault flag on GPIO
	// Return status which can be bit flagged
	// for each individual fault
	// Bits will be a 1 if there is a fault
}

int8_t PreCharge_Contactor_Read(void){
	// Read the GPIO Pins associated with the 
	// Pre-Charge Contactor
}

void PreCharge_Contactor_Write(bool LSD, bool HSD){
	// Write to the GPIO Pins associated with the 
	// Pre-Charge Contactor
}

bool Vehicle_Voltage_Check(void){
	// Vehicle/ECU Voltage >= 90% of Battery Pack Voltage
}

int8_t Negative_Contactor_Read(void){
	// Read the GPIO Pins associated with the 
	// Negative Contactor
}

void Negative_Contactor_Write(bool LSD, bool HSD){
	// Write to the GPIO Pins associated with the 
	// Negative Contactor
}

int8_t Positive_Contactor_Read(void){
	// Read the GPIO Pins associated with the 
	// Positive Contactor
}

void Positive_Contactor_Write(bool LSD, bool HSD){
	// Write to the GPIO Pins associated with the 
	// Positive Contactor
}

void Log_Faults(void){
    // Log the Faults to Disk
}

bool BMS_Sleep(void){
    // Put the BMS to Sleep
}

bool LV_Power_OFF(void){
    // Low Voltage Power Off and Disable Vehicle/ECU Comms
}

void init(void) {
	
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

void Safe_Shut_Down(void){
	
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

void loop(void) {

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
			if (fault_status != 0){
			    EPO(); // Power OFF if there's a fault
            } else if (PWR_OFF_REQUEST){
				Safe_Shut_Down();
				break; // : )
			} else {
			    printf("System Running with No Faults...");
            }
		}
	} else {
		printf("Waiting for Power On Request...");
	}

}