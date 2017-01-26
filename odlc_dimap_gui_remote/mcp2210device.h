#pragma once

//#ifndef _MCP2210DEVICE_H
//#define _MCP2210DEVICE_H

#include "odlc_dimap_gui.h"

#include "ic_ht.h"
#include "mcp2210.h"
#include "hidapi.h"

#include <QString>


class MCP2210Device : public QObject
{
	
	Q_OBJECT
		
public:
	
	MCP2210Device()
	{
		_name = "MCP2210";
		_desc = "USB-SPI Interface";
		_handle = InitMCP2210();
		if (_handle != NULL)
		{
			_isInitialized = true;
		}
	}
	
	
	~MCP2210Device()
	{
		if (_handle != NULL)
		{
			ReleaseMCP2210(_handle);
			emit lineToLog(_name + ": USB Handle released.");
		}
	}
	
	
	bool isInitialized()
	{ 
		return this->_isInitialized;
	}
	
	bool readDeviceConfig();
	bool writeDeviceConfig();
	
signals:
	void lineToLog(const QString &);
	
private:
	
	// Device
	
	hid_device *_handle = NULL;
	ChipSettingsDef _chipDef;
	SPITransferSettingsDef _spiTransferSettingsDef;
	USBKeyParametersDef _usbKeyParameters;
	
	bool _isInitialized = false;
	
	QString _vendorID;
	QString _productID;
	QString _serialNo;
	
	// Bezeichnung
	QString _name;
	
	// Beschreibung
	QString _desc;
	
};




//#endif