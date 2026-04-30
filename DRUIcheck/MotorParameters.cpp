#include "MotorParameters.h"

QByteArray MotorParameters::MotorAddressParameter::PositionmodeParameters = QByteArray::fromHex("0110015800060C00000258000002580000000000");
QByteArray MotorParameters::MotorAddressParameter::MotorTargetPosition = QByteArray::fromHex("0110015E0002040000000000");
QByteArray MotorParameters::MotorAddressParameter::MotorEnable = QByteArray::fromHex("0110007C00060C009F000000000000000000000000");
QByteArray MotorParameters::MotorAddressParameter::MotorDisable = QByteArray::fromHex("0110007C00060c009E00000000000000000000");
QByteArray MotorParameters::MotorAddressParameter::MotorStart = QByteArray::fromHex("0110007C00060C006700000000000000000000");
QByteArray MotorParameters::MotorAddressParameter::MotorStop = QByteArray::fromHex("0110007C00060c00E200000000000000000000");
QByteArray MotorParameters::MotorAddressParameter::JogSlaveData = QByteArray::fromHex("0210015200060C000000000000000000000000");
QByteArray MotorParameters::MotorAddressParameter::Alarmreset = QByteArray::fromHex("0110007C00060C00BA00000000000000000000");
QByteArray MotorParameters::MotorAddressParameter::AlarmcodeRead = QByteArray::fromHex("010300000002C40B");
QByteArray MotorParameters::MotorAddressParameter::EncoderPositionReset = QByteArray::fromHex("0110007C00060c009800000000000000000000");
