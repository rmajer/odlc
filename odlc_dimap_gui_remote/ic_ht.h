/** @file
 * @defgroup API ic-HT API
 */

#ifndef IC_HT_H_
#define IC_HT_H_

using namespace std;

#include <cstdlib>
#include <iostream>

#include <cstdlib>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "hidapi.h"
#include "mcp2210.h"

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;

// Operationen
const byte HT_OP_READ = 0x80;
const byte HT_OP_WRITE = 0x40; 

// Masken
const byte HT_MASK_REF = 0x03;		// um aus 0x13 die zwei niederwertigsten bits zu bekommen
const byte HT_MASK_NONE = 0xFF;
const byte HT_MASK_ADSNF = (1 << 2) | (1 << 6);
const byte HT_MASK_ADC = 0x03;
const byte HT_MASK_COMP = (1 << 6) | (1 << 5) | (1 << 4);

// channel
const byte HT_CHANNEL_1 = 0x00;
const byte HT_CHANNEL_2 = 0x01;

// Register
const byte HT_REG_STATUS1 = 0x00;
const byte HT_REG_STATUS2 = 0x01;
const byte HT_REG_TEMP = 0x02;
const byte HT_REG_CHIPREV = 0x0F;

const byte HT_REG_ADSNF = 0x1A;

const byte HT_REG_CH1_ADC_LOW = 0x04;
const byte HT_REG_CH1_ADC_HIGH = 0x03;
const byte HT_REG_CH1_SETTINGS = 0x10;
const byte HT_REG_CH1_LIMIT = 0x11;
const byte HT_REG_CH1_RMD = 0x12;
const byte HT_REG_CH1_COMP = 0x13;
const byte HT_REG_CH1_REF_HIGH = 0x13;
const byte HT_REG_CH1_REF_LOW = 0x14;

const byte HT_REG_CH2_ADC_LOW = 0x06;
const byte HT_REG_CH2_ADC_HIGH = 0x05;
const byte HT_REG_CH2_SETTINGS = 0x15;
const byte HT_REG_CH2_LIMIT = 0x16;
const byte HT_REG_CH2_COMP = 0x18;
const byte HT_REG_CH2_REF_HIGH = 0x18;
const byte HT_REG_CH2_REF_LOW = 0x19;
const byte HT_REG_CH2_RMD = 0x17;

const byte HT_REG_MERGE = 0x1B;
const byte HT_REG_MODE = 0x1C;

// Konfigurationseintraege
const byte HT_MODE_OPERATIONAL = 0x01;
const byte HT_MODE_CONFIGURATION = 0x02;


// current limits
const byte HT_CURLIM_400 = 0x64;
const byte HT_CURLIM_900 = 0xE1;


// einträge für adcc etc

const byte HT_BIT_ADCC2 = 1 << 7;
const byte HT_BIT_ADCC1 = 1 << 6;
const byte HT_BIT_ADCC0 = 1 << 5;
const byte HT_BIT_EOC = 1 << 4;
const byte HT_BIT_DISC = 1 << 3;
const byte HT_BIT_DISP = 1 << 2;
const byte HT_BIT_ECIE = 1 << 1;
const byte HT_BIT_EACC = 1;

// gewisse default-settings hier schon vorgeben (ACC Mode, PLR, ADC Source PLR, channel disabled by default
const byte HT_BYTE_ADCC_DEFAULTS = HT_BIT_DISC | HT_BIT_EACC | HT_BIT_ADCC2 | HT_BIT_EOC;



// platzhalter; werden durch init_acc etc ausgewertet
const byte HT_CURREF_MIN = 0x01;	// kleinster definierter wert
const byte HT_CURREF_MAX = 0x03;	// hoechster definierter index

const byte HT_CURREF_750 = 0x01;
const byte HT_CURREF_304 = 0x02;
const byte HT_CURREF_104 = 0x03;


// Datentypen

typedef struct
{
	
	bool disabled;
	byte curref_msb;
	byte curref_lsb;
	byte curlim;
	byte resistor;
	word curref;
	
} acc_channel_settings;




typedef struct
{
	std::string *name;
	std::string *bemerkung;
	
	byte adsnf;
	byte chiprev;
	word cs_pin;		// welcher pin muss am mcp2210 low genommen werden um dieses device anzusprechen
	byte last_temp;
	
	bool merge;
	
	acc_channel_settings channels[2];
	
	hid_device *handle;		// usb handle zur kommunikation
	
} ht_spi_device;




// Funktionen

int set_acc_channel_settings(acc_channel_settings *chan, bool disabled, byte curref, byte curlim);


// settings auslesen
int ht_read_settings(ht_spi_device *device);
int ht_read_byte_register(ht_spi_device *device, byte reg, byte * res, bool set_spi_options, byte reg_mask);

int ht_set_safe_settings(ht_spi_device *device);
int ht_write_byte_register(ht_spi_device *device, byte reg, byte val, bool set_spi_options);

int ht_enter_configmode(ht_spi_device *device);
int ht_leave_configmode(ht_spi_device *device);

int ht_modify_register(ht_spi_device *device, byte reg, byte val, bool set_spi_options, byte mask);

int ht_channel_enable(ht_spi_device *device, byte channel);

int ht_read_status(ht_spi_device *device, byte *res);




// helferfunktionen
char *print_bitfield_8(byte bitfield);

#endif 