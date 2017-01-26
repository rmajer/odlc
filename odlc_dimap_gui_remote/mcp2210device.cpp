#include "mcp2210device.h"

bool MCP2210Device::readDeviceConfig()
{
	
	emit lineToLog(_name + ": Reading device config...");
	
	if (NULL == _handle)
	{
		emit lineToLog(_name + ": Error: no handle!");
		return false;
	}
	
	emit lineToLog(_name + ": Reading USB key parameters...");
	_usbKeyParameters = GetUSBKeyParameters(_handle);
	
	if (_usbKeyParameters.ErrorCode == 0)
	{
		this->_vendorID = QString("%1").arg(_usbKeyParameters.VID, 2, 16, QChar('0'));
		this->_productID = QString("%1").arg(_usbKeyParameters.PID, 2, 16, QChar('0'));
		
		QString tmp = QString("%1").arg(2*_usbKeyParameters.RequestedCurrentAmountFromHost, 4, 10, QChar('0'));
		QString val = _name + ": VendorID " + _vendorID + " | ProductID: " + _productID + " | Req. Current: " + tmp + " mA";
		
		emit lineToLog(val);
		
	}
	else if (_usbKeyParameters.ErrorCode == 0xFB)
	{
		emit lineToLog(_name + ": Wrong password!");
	}
	else
	{
		emit lineToLog(_name + ": USB reading error " + QString("%1").arg(_usbKeyParameters.ErrorCode, 2, 16, QChar('0')));
	}
	
	_chipDef = GetChipSettings(_handle);
	_spiTransferSettingsDef = GetSPITransferSettings(_handle);
	
	return true;
}