#ifndef BQ_REG_H_
#define BQ_REG_H_

#define DIR0_ADDR_OTP			0x0 // 8 bit - 00__ ____ - bit[0:5] = DIR0_ADDR_OTP; bit[6:7] = Reserved = 0
#define DIR1_ADDR_OTP			0x1 // 8 bit - 00__ ____ - bit[0:5] = DIR0_ADDR_OTP; bit[6:7] = Reserved = 0
#define DEV_CONF				0x2 // 8 bit - b0 = HB_EN, b1 = FTONE_EN, b2 = NFAULT_EN, b3 = TWO_STOP_EN, b4 = FCOMM_EN, b5 = MULTIDROP_EN, b6 = NO_ADJ_CB, b7 = CBTWARN_DIS
#define ACTIVE_CELL				0x3 // 8 bit - 0000 ____ - b[0:3] = NUM_CELL, b[4:7] = RESERVED = 0
#define OTP_SPARE15				0x4 // 8 bit - bit[0:7] = SPARE
#define BBVC_POSN1				0x5 // 8 bit - b0 = cell 9, b1 = cell 10, b2 = cell 11, b3 = cell 12, b4 = cell 13, b5 = cell 14, b6 = cell 15, b7 = cell 16
#define BBVC_POSN2				0x6 // 8 bit - b0 = cell 1, b1 = cell 2, b2 = cell 3, b4 = cell 5, b5 = cell 6, b6 = cell 7, b7 = cell 8
#define ADC_CONF1				0x7 // 8 bit - b[0:2] = LPF_VCELL, b[3:5] LPF_BB, b[6:7] = AUX_SETTLE
#define ADC_CONF2				0x8 // 8 bit - b[0:5] = ADC_DLY (ADC delay), b[6:7] = RESERVED
#define OV_THRESH				0x9 // 8 bit - b[0:5] = OV_THRESH, b[6:7] = RESERVED
#define UV_THRESH				0xA // 8 bit - b[0:5] = UV_THRESH, b[6:7] = RESERVED
#define OTUT_THRESH				0xB // 8 bit - b[0:4] = OT_THRESH, b[5:7] = UT THRESH
#define UV_DISABLE1				0xC // 8 bit - b0 = cell 9, b1 = cell 10, b2 = cell 11, b3 = cell 12, b4 = cell 13, b5 = cell 14, b6 = 15, b7 = 16 
#define UV_DISABLE2				0xD // 8 bit - b0 = cell 1, b1 = cell 2, b2 = cell 3, b3 = cell 4, b4 = cell 5, b5 = cell 6, b6 = cell 7, b7 = cell 8
#define GPIO_CONF1				0xE // 8 bit - b[0:2] = GPIO1, b[3:5] = GPIO2, b[6] = SPI_EN, b[7] = FAULT_IN_EN
#define GPIO_CONF2				0xF // 8 bit - b[0:2] = GPIO3, b[3:5] = GPIO4, b[6] = CS_RDY_EN, b[7] = RESERVED
#define GPIO_CONF3				0x10 // 8 bit - b[0:2] = GPIO5, b[3:5] = GPIO6, b[6:7] = RESERVED
#define GPIO_CONF4				0x11 // 8 bit - b[0:2] = GPIO7, b[3:5] = GPIO8, b[6:7] = RESERVED
#define OTP_SPARE14				0x12 // 8 bit - b[0:7] = SPARE
#define OTP_SPARE13				0x13 // 8 bit - b[0:7] = SPARE
#define OTP_SPARE12				0x14 // 8 bit - b[0:7] = SPARE
#define OTP_SPARE11				0x15 // 8 bit - b[0:7] = SPARE
#define FAULT_MSK1				0x16 // 8 bit - b0 = MSK_PWR, b1 = MSK_SYS, b2 = MSK_COMP, b3 = MSK_OV, b4 = MSK_UV, b5 = MSK_OT, b6 = MSK_UT, b7 = MSK_PROT
#define FAULT_MSK2				0x17 // 8 bit - b0 = MSK_COMM1, b1 = MSK_COMM2, b2 = MSK_COMM3_HB, b3 = MSK_COMM3_FTONE, b4 = MSK_COMM3_FCOMM, b5 = MSK_OTP_DATA, b6 = MSK_OTP_CRC, b7 = RESERVED
#define PWR_TRANSIT_CONF		0x18 // 8 bit - b[0:2] = SLP_TIME, b[3:4] = TWARN_THR, b[5:7] = RESERVED
#define COMM_TIMEOUT_CONF		0x19 // 8 bit - b[0:2] = CTL_TIME, b[3] = CTL_ACT, b[4:6] = CTS_TIME, b[7] = RESERVED
#define TX_HOLD_OFF				0x1A // 8 bit - b[0:7] = TX_HOLD_OFF - Sets the number of bit periods from 0 to 255 to delay after receiving the STOP bit of a command frame and before transmitting the 1st bit of response frame.
#define MAIN_ADC_CAL1			0x1B
#define MAIN_ADC_CAL2			0x1C
#define AUX_ADC_CAL1			0x1D
#define AUX_ADC_CAL2			0x1E
#define CS_ADC_CAL1				0x1F
#define CS_ADC_CAL2				0x20
#define CUST_MISC1				0x21
#define CUST_MISC2				0x22
#define CUST_MISC3				0x23
#define CUST_MISC4				0x24
#define CUST_MISC5				0x25
#define CUST_MISC6				0x26
#define CUST_MISC7				0x27
#define CUST_MISC8				0x28
#define STACK_RESPONSE			0x29
#define BBP_LOC					0x2A
#define DAISY_CONF				0x2B
#define OTP_SPARE10				0x2C
#define OTP_SPARE9				0x2D
#define OTP_SPARE8				0x2E
#define OTP_SPARE7				0x2F
#define OTP_SPARE6				0x30
#define OTP_SPARE5				0x31
#define OTP_SPARE4				0x32
#define OTP_SPARE3				0x33
#define OTP_SPARE2				0x34
#define OTP_SPARE1				0x35
#define CUST_CRC_HI				0x36
#define CUST_CRC_LO				0x37
#define OTP_PROG_UNLOCK1A		0x300
#define OTP_PROG_UNLOCK1B		0x301
#define OTP_PROG_UNLOCK1C		0x302
#define OTP_PROG_UNLOCK1D		0x303
#define DIR0_ADDR				0x306
#define DIR1_ADDR				0x307
#define COMM_CTRL				0x308
#define CONTROL1				0x309
#define CONTROL2				0x30A
#define OTP_PROG_CTRL			0x30B
#define ADC_CTRL1				0x30D
#define ADC_CTRL2				0x30E
#define ADC_CTRL3				0x30F
#define ADC_CTRL4				0x310
#define CB_CELL16_CTRL			0x318
#define CB_CELL15_CTRL			0x319
#define CB_CELL14_CTRL			0x31A
#define CB_CELL13_CTRL			0x31B
#define CB_CELL12_CTRL			0x31C
#define CB_CELL11_CTRL			0x31D
#define CB_CELL10_CTRL			0x31E
#define CB_CELL9_CTRL			0x31F
#define CB_CELL8_CTRL			0x320
#define CB_CELL7_CTRL			0x321
#define CB_CELL6_CTRL			0x322
#define CB_CELL5_CTRL			0x323
#define CB_CELL4_CTRL			0x324
#define CB_CELL3_CTRL			0x325
#define CB_CELL2_CTRL			0x326
#define CB_CELL1_CTRL			0x327
#define VMB_DONE_THRESH			0x328
#define MB_TIMER_CTRL			0x329
#define VCB_DONE_THRESH			0x32A
#define OTCB_THRESH				0x32B
#define OVUV_CTRL				0x32C
#define OTUT_CTRL				0x32D
#define BAL_CTRL1				0x32E
#define BAL_CTRL2				0x32F
#define BAL_CTRL3				0x330
#define FAULT_RST1				0x331
#define FAULT_RST2				0x332
#define DIAG_OTP_CTRL			0x335
#define DIAG_COMM_CTRL			0x336
#define DIAG_PWR_CTRL			0x337
#define DIAG_CBFET_CTRL1		0x338
#define DIAG_CBFET_CTRL2		0x339
#define DIAG_COMP_CTRL1			0x33A
#define DIAG_COMP_CTRL2			0x33B
#define DIAG_COMP_CTRL3			0x33C
#define DIAG_COMP_CTRL4			0x33D
#define DIAG_PROT_CTRL			0x33E
#define OTP_ECC_DATAIN1			0x343
#define OTP_ECC_DATAIN2			0x344
#define OTP_ECC_DATAIN3			0x345
#define OTP_ECC_DATAIN4			0x346
#define OTP_ECC_DATAIN5			0x347
#define OTP_ECC_DATAIN6			0x348
#define OTP_ECC_DATAIN7			0x349
#define OTP_ECC_DATAIN8			0x34A
#define OTP_ECC_DATAIN9			0x34B
#define OTP_ECC_TEST			0x34C
#define SPI_CONF				0x34D
#define SPI_TX3					0x34E
#define SPI_TX2					0x34F
#define SPI_TX1					0x350
#define SPI_EXE					0x351
#define OTP_PROG_UNLOCK2A		0x352
#define OTP_PROG_UNLOCK2B		0x353
#define OTP_PROG_UNLOCK2C		0x354
#define OTP_PROG_UNLOCK2D		0x355
#define DEBUG_CTRL_UNLOCK		0x700
#define DEBUG_COMM_CTRL1		0x701
#define DEBUG_COMM_CTRL2		0x702
#define PARTID					0x500 // 0x21 - BQ79616
#define DIE_ID1					0x501
#define DIE_ID2					0x502
#define DIE_ID3					0x503
#define DIE_ID4					0x504
#define DIE_ID5					0x505
#define DIE_ID6					0x506
#define DIE_ID7					0x507
#define DIE_ID8					0x508
#define DIE_ID9					0x509
#define CUST_CRC_RSLT_HI		0x50C
#define CUST_CRC_RSLT_LO		0x50D
#define OTP_ECC_DATAOUT1		0x510
#define OTP_ECC_DATAOUT2		0x511
#define OTP_ECC_DATAOUT3		0x512
#define OTP_ECC_DATAOUT4		0x513
#define OTP_ECC_DATAOUT5		0x514
#define OTP_ECC_DATAOUT6		0x515
#define OTP_ECC_DATAOUT7		0x516
#define OTP_ECC_DATAOUT8		0x517
#define OTP_ECC_DATAOUT9		0x518
#define OTP_PROG_STAT			0x519
#define OTP_CUST1_STAT			0x51A
#define OTP_CUST2_STAT			0x51B
#define SPI_RX3					0x520
#define SPI_RX2					0x521
#define SPI_RX1					0x522
#define DIAG_STAT				0x526
#define ADC_STAT1				0x527
#define ADC_STAT2				0x528
#define GPIO_STAT				0x52A
#define BAL_STAT				0x52B
#define DEV_STAT				0x52C
#define FAULT_SUMMARY			0x52D
#define FAULT_COMM1				0x530
#define FAULT_COMM2				0x531
#define FAULT_COMM3				0x532
#define FAULT_OTP				0x535
#define FAULT_SYS				0x536
#define FAULT_PROT1				0x53A
#define FAULT_PROT2				0x53B
#define FAULT_OV1				0x53C
#define FAULT_OV2				0x53D
#define FAULT_UV1				0x53E
#define FAULT_UV2				0x53F
#define FAULT_OT				0x540
#define FAULT_UT				0x541
#define FAULT_COMP_GPIO			0x543
#define FAULT_COMP_VCCB1		0x545
#define FAULT_COMP_VCCB2		0x546
#define FAULT_COMP_VCOW1		0x548
#define FAULT_COMP_VCOW2		0x549
#define FAULT_COMP_CBOW1		0x54B
#define FAULT_COMP_CBOW2		0x54C
#define FAULT_COMP_CBFET1		0x54E
#define FAULT_COMP_CBFET2		0x54F
#define FAULT_COMP_MISC			0x550
#define FAULT_PWR1				0x552
#define FAULT_PWR2				0x553
#define FAULT_PWR3				0x554
#define CB_COMPLETE1			0x556
#define CB_COMPLETE2			0x557
#define BAL_TIME				0x558
#define VCELL16_HI				0x568
#define VCELL16_LO				0x569
#define VCELL15_HI				0x56A
#define VCELL15_LO				0x56B
#define VCELL14_HI				0x56C
#define VCELL14_LO				0x56D
#define VCELL13_HI				0x56E
#define VCELL13_LO				0x56F
#define VCELL12_HI				0x570
#define VCELL12_LO				0x571
#define VCELL11_HI				0x572
#define VCELL11_LO				0x573
#define VCELL10_HI				0x574
#define VCELL10_LO				0x575
#define VCELL9_HI				0x576
#define VCELL9_LO				0x577
#define VCELL8_HI				0x578
#define VCELL8_LO				0x579
#define VCELL7_HI				0x57A
#define VCELL7_LO				0x57B
#define VCELL6_HI				0x57C
#define VCELL6_LO				0x57D
#define VCELL5_HI				0x57E
#define VCELL5_LO				0x57F
#define VCELL4_HI				0x580
#define VCELL4_LO				0x581
#define VCELL3_HI				0x582
#define VCELL3_LO				0x583
#define VCELL2_HI				0x584
#define VCELL2_LO				0x585
#define VCELL1_HI				0x586
#define VCELL1_LO				0x587
#define BUSBAR_HI				0x588
#define BUSBAR_LO				0x589
#define TSREF_HI				0x58C
#define TSREF_LO				0x58D
#define GPIO1_HI				0x58E
#define GPIO1_LO				0x58F
#define GPIO2_HI				0x590
#define GPIO2_LO				0x591
#define GPIO3_HI				0x592
#define GPIO3_LO				0x593
#define GPIO4_HI				0x594
#define GPIO4_LO				0x595
#define GPIO5_HI				0x596
#define GPIO5_LO				0x597
#define GPIO6_HI				0x598
#define GPIO6_LO				0x599
#define GPIO7_HI				0x59A
#define GPIO7_LO				0x59B
#define GPIO8_HI				0x59C
#define GPIO8_LO				0x59D
#define DIETEMP1_HI				0x5AE
#define DIETEMP1_LO				0x5AF
#define DIETEMP2_HI				0x5B0
#define DIETEMP2_LO				0x5B1
#define AUX_CELL_HI				0x5B2
#define AUX_CELL_LO				0x5B3
#define AUX_GPIO_HI				0x5B4
#define AUX_GPIO_LO				0x5B5
#define AUX_BAT_HI				0x5B6
#define AUX_BAT_LO				0x5B7
#define AUX_REFL_HI				0x5B8
#define AUX_REFL_LO				0x5B9
#define AUX_VBG2_HI				0x5BA
#define AUX_VBG2_LO				0x5BB
#define AUX_VREF4P2_HI			0x5BC
#define AUX_VREF4P2_LO			0x5BD
#define AUX_AVAO_REF_HI			0x5BE
#define AUX_AVAO_REF_LO			0x5BF
#define AUX_AVDD_REF_HI			0x5C0
#define AUX_AVDD_REF_LO			0x5C1
#define AUX_OV_DAC_HI			0x5C2
#define AUX_OV_DAC_LO			0x5C3
#define AUX_UV_DAC_HI			0x5C4
#define AUX_UV_DAC_LO			0x5C5
#define AUX_OT_OTCB_DAC_HI		0x5C6
#define AUX_OT_OTCB_DAC_LO		0x5C7
#define AUX_UT_DAC_HI			0x5C8
#define AUX_UT_DAC_LO			0x5C9
#define AUX_VCBDONE_DAC_HI		0x5CA
#define AUX_VCBDONE_DAC_LO		0x5CB
#define AUX_VCM1_HI				0x5CC
#define AUX_VCM1_LO				0x5CD
#define VREF4P2_HI				0x5CE
#define VREF4P2_LO				0x5CF
#define REFH_HI					0x5D0
#define REFH_LO					0x5D1
#define DIAG_MAIN_HI			0x5D2
#define DIAG_MAIN_LO			0x5D3
#define DIAG_AUX_HI				0x5D4
#define DIAG_AUX_LO				0x5D5
#define CURRENT_HI				0x5D6
#define CURRENT_MID				0x5D7
#define CURRENT_LO				0x5D8
#define DEBUG_COMM_STAT			0x780
#define DEBUG_UART_RC			0x781
#define DEBUG_UART_RR_TR		0x782
#define DEBUG_COMH_BIT			0x783
#define DEBUG_COMH_RC			0x784
#define DEBUG_COMH_RR_TR		0x785
#define DEBUG_COML_BIT			0x786
#define DEBUG_COML_RC			0x787
#define DEBUG_COML_RR_TR		0x788
#define DEBUG_UART_DISCARD		0x789
#define DEBUG_COMH_DISCARD		0x78A
#define DEBUG_COML_DISCARD		0x78B
#define DEBUG_UART_VALID_HI		0x78C
#define DEBUG_UART_VALID_LO		0x78D
#define DEBUG_COMH_VALID_HI		0x78E
#define DEBUG_COMH_VALID_LO		0x78F
#define DEBUG_COML_VALID_HI		0x790
#define DEBUG_COML_VALID_LO		0x791
#define DEBUG_OTP_SEC_BLK		0x7A0
#define DEBUG_OTP_DED_BLK		0x7A1

#endif /* BQ_REG_H_ */