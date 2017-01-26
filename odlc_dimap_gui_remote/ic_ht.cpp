#include "ic_ht.h"

#include <malloc.h>

int set_acc_channel_settings(acc_channel_settings *chan, bool disabled, byte curref, byte curlim)
{
	

	if (curref < HT_CURREF_MIN || curref > HT_CURREF_MAX)
	{
		return -1;
	}
	
	if (NULL == chan)
	{
		chan = (acc_channel_settings *) malloc (sizeof(acc_channel_settings));
		if (NULL == chan)
		{
			return -1;
		}
	}
	
	chan->disabled = disabled;
	chan->curlim = curlim;
	
	switch (curref)
	{
	case HT_CURREF_750:
		chan->curref_msb = 0x03;
		chan->curref_lsb = 0xBF;
		break;
	case HT_CURREF_304:
		chan->curref_msb = 0x02;
		chan->curref_lsb = 0x49;
		break;
	case HT_CURREF_104:
		chan->curref_msb = 0x00;
		chan->curref_lsb = 0x80;
		break;
	}
	
	return 0;
	
}


// ein einzelnes register auslesen; 
int ht_read_byte_register(ht_spi_device *device, byte reg, byte *res, bool set_spi_options, byte reg_mask)
{
	
	if (NULL == device || NULL == res)
	{
		return -1;
	}
	
	SPITransferSettingsDef def;
	SPIDataTransferStatusDef def1;
	
	int ret = 0;
	
	// spi optionen setzen
	if (set_spi_options)
	{
		def = GetSPITransferSettings(device->handle);
 
		def.ActiveChipSelectValue = 0x0000 | device->cs_pin;
	
		def.IdleChipSelectValue = 0xffff;
		def.BitRate = 10000000l;
		def.BytesPerSPITransfer = 2;
 
		ret = SetSPITransferSettings(device->handle, def);	
	}
	
	byte spiCmdBuf[2];
	spiCmdBuf[1] = 0x00;
	
	spiCmdBuf[0] = HT_OP_READ + reg;
	
	def1 = SPISendReceive(device->handle, spiCmdBuf, 1);
	
	if (def1.NumberOfBytesReceived == 0)
	{
		return -1;
	}
	
	*res = (def1.DataReceived[0] & reg_mask);
	
//#ifdef DEBUG
//	printf("ht_read_byte_register: address %02X is %02X\n", reg, *res);
//#endif
	
	return 0;
}




int ht_read_settings(ht_spi_device *device)
{
	
	if (NULL == device)
	{
		return -1;
	}
	
	int ret = 0;
	
 
	byte res = 0;

	
	ht_read_byte_register(device, HT_REG_TEMP, &(device->last_temp), true, HT_MASK_NONE);
	ht_read_byte_register(device, HT_REG_CHIPREV, &(device->chiprev), false, HT_MASK_NONE);
	
	ht_read_byte_register(device, HT_REG_CH1_REF_HIGH, &(device->channels[0].curref_msb), false, HT_MASK_REF);
	ht_read_byte_register(device, HT_REG_CH2_REF_HIGH, &(device->channels[1].curref_msb), false, HT_MASK_REF);
	
	ht_read_byte_register(device, HT_REG_CH1_REF_LOW, &(device->channels[0].curref_lsb), false, HT_MASK_NONE);
	ht_read_byte_register(device, HT_REG_CH2_REF_LOW, &(device->channels[1].curref_lsb), false, HT_MASK_NONE);
	
	ht_read_byte_register(device, HT_REG_CH1_LIMIT, &(device->channels[0].curlim), false, HT_MASK_NONE);
	ht_read_byte_register(device, HT_REG_CH2_LIMIT, &(device->channels[1].curlim), false, HT_MASK_NONE);
	
	ht_read_byte_register(device, HT_REG_CH1_RMD, &(device->channels[0].resistor), false, HT_MASK_NONE);
	ht_read_byte_register(device, HT_REG_CH2_RMD, &(device->channels[1].resistor), false, HT_MASK_NONE);
	
	ht_read_byte_register(device, HT_REG_ADSNF, &(device->adsnf), true, HT_MASK_NONE); 
	
	return 0;
}


// schreibt ein register; um sicherung alter werte muss sich eine andere routine kümmern
int ht_write_byte_register(ht_spi_device *device, byte reg, byte val, bool set_spi_options)
{
	if (NULL == device)
	{
		return -1;
	}
	
	int ret = 0;
	SPITransferSettingsDef def;
	byte spiCmd[3];
	
	spiCmd[2] = 0x00;
	
	// spi optionen setzen
	if (set_spi_options)
	{
		def = GetSPITransferSettings(device->handle);
 
		def.ActiveChipSelectValue = 0x0000 | device->cs_pin;
	
		def.IdleChipSelectValue = 0xffff;
		def.BitRate = 10000000l;
		def.BytesPerSPITransfer = 2;
 
		ret = SetSPITransferSettings(device->handle, def);	
	}
	
	spiCmd[0] = HT_OP_WRITE + reg;
	spiCmd[1] = val;
	
	SPIDataTransferStatusDef def1 = SPISendReceive(device->handle, spiCmd, 2);
	
	return 0;
}



// modifiziert in dem register nur eintraege in mask; alle anderen werden ident zurueckgeschrieben
int ht_modify_register(ht_spi_device *device, byte reg, byte val, bool set_spi_options, byte mask)
{
	
	if (NULL == device)
	{
		return -1;
	}
	
	byte res = 0;

#ifdef DEBUG
	char *tmp1, *tmp2, *tmp3;
#endif
	// gesamtes register auslesen
	ht_read_byte_register(device, reg, &res, false, HT_MASK_NONE);
	
#ifdef DEBUG
	tmp1 = print_bitfield_8(res);
#endif
	res &= (~mask);
#ifdef DEBUG
	tmp2 = print_bitfield_8(~mask);
#endif
	res |= (val & mask);
#ifdef DEBUG
	tmp3 = print_bitfield_8(res);
	char *tmp4 = print_bitfield_8(mask);
	char *tmp5 = print_bitfield_8(val&mask);
	
	printf("ht_modify_register modifying reg %02X with value %02X and mask %s\n", reg, val, tmp4);
	printf("(org & ~mask) | (val & mask): (%s & %s) | %s -> final: %s\n", tmp1, tmp2, tmp5, tmp3);
	free(tmp1); free(tmp2); free(tmp3); free(tmp4); free(tmp5);
#endif
	ht_write_byte_register(device, reg, res, false);
	
	return 0;
}



int ht_enter_configmode(ht_spi_device *device)
{
	if (NULL == device)
	{
		return -1;
	}
	
#ifdef DEBUG
	printf("Entering configmode...\n");
#endif
	int ret = ht_write_byte_register(device, HT_REG_MODE, 0x02, true);
	return ret;
}


int ht_leave_configmode(ht_spi_device *device)
{
	if (NULL == device)
	{
		return -1;
	}
	
#ifdef DEBUG
	printf("Exiting configmode.\n");
#endif
	
	int ret =  ht_write_byte_register(device, HT_REG_MODE, 0x01, false);	
	return ret;
}


// Channel1: 0, Channel2: 1
int ht_channel_enable(ht_spi_device *device, byte channel)
{
	if (NULL == device || channel > 1)
	{
		return -1;
	}
	
	byte reg = (channel == 0 ? HT_REG_CH1_SETTINGS : HT_REG_CH2_SETTINGS);
	
	int ret = ht_modify_register(device, reg, 0, false, HT_BIT_DISC);
	return ret;
}






int ht_set_safe_settings(ht_spi_device *device)
{

	if (NULL == device)
	{
		return -1;
	}
	

	ht_enter_configmode(device);
	
#ifdef DEBUG
	printf("Configuring device with safe default settings.\n");
#endif		
	
	// 104 mA referece
	ht_write_byte_register(device, HT_REG_CH1_REF_LOW, 0x80, false);
	ht_write_byte_register(device, HT_REG_CH2_REF_LOW, 0x80, false);
	
	ht_modify_register(device, HT_REG_CH1_REF_HIGH, 0x00, false, HT_MASK_REF);
	ht_modify_register(device, HT_REG_CH2_REF_HIGH, 0x00, false, HT_MASK_REF);
	
	// 400 mA limit
	ht_write_byte_register(device, HT_REG_CH1_LIMIT, 0x64, false);
	ht_write_byte_register(device, HT_REG_CH2_LIMIT, 0x64, false);
	
	// default settings
	ht_write_byte_register(device, HT_REG_CH1_SETTINGS, HT_BYTE_ADCC_DEFAULTS, false);
	ht_write_byte_register(device, HT_REG_CH2_SETTINGS, HT_BYTE_ADCC_DEFAULTS, false);
	
	// adsnf aktivieren
	ht_modify_register(device, HT_REG_ADSNF, HT_MASK_ADSNF, false, HT_MASK_ADSNF);
	
	// schnelle kompensation
	ht_modify_register(device, HT_REG_CH1_COMP, HT_MASK_COMP, false, HT_MASK_COMP);
	ht_modify_register(device, HT_REG_CH2_COMP, HT_MASK_COMP, false, HT_MASK_COMP);
	
	ht_leave_configmode(device);
	
	
	return 0;
}


int ht_read_status(ht_spi_device *device, byte *res)
{
	
	if (NULL == device || res == NULL)
	{
		return -1;
	}
	
	int ret = ht_read_byte_register(device, HT_REG_STATUS1, res, true, HT_MASK_NONE);
	
#ifdef DEBUG
	char *dbg = print_bitfield_8(*res);
	printf("status bitfield (%02X): %s\n", *res, dbg);
#endif
	
	return ret;
	
}




// HELFER-Funktionen


char *print_bitfield_8(byte bitfield)
{
	
	unsigned int len = 8*sizeof(bitfield);
	char *ret = (char *) malloc(len+1);
	
	ret[len] = '\0';
	for (int i = 0; i < len; i++)
	{
		ret[i] =  ((bitfield & (1 << (len - 1 -i))) > 0) ? '1' : '0';
	}
	return ret;
}