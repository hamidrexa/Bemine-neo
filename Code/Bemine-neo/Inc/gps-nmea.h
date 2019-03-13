/******************************************************************************
 * Copyright (C) 2013 Marco Giammarini <http://www.warcomeb.it>
 * 
 * Author(s):
 *  Marco Giammarini <m.giammarini@warcomeb.it>
 *  
 * Project: gps-nmea
 * Package: -
 * Version: 0.0
 * 
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>
 ******************************************************************************/

/**
 * @file gps-nmea.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief GPS NMEA protocol functions definition.
 */

#ifndef __GPS_NMEA_H
#define __GPS_NMEA_H

/* OHIBoard Library (LGPL) */

typedef enum _GpsNmea_Errors
{
	GPSNMEA_ERROR_OK,
	GPSNMEA_ERROR_NO_ACTIVE,
	GPSNMEA_ERROR_JUST_ACTIVE,
	
	GPSNMEA_ERROR_WRONG_CHAR,
	GPSNMEA_ERROR_MSG_TOO_LONG,
	
	GPSNMEA_ERROR_NOT_COMPLIANT,
	GPSNMEA_ERROR_CHECKSUM,
	GPSNMEA_ERROR_MSG_TYPE,
	
	GPSNMEA_ERROR_MSG_RMC_INVALID,
	GPSNMEA_ERROR_MSG_RMC_VALID,
	
	GPSNMEA_ERROR_COORD_CONVERSION,
	GPSNMEA_ERROR_TIME_CONVERSION,
} GpsNmea_Errors;

typedef enum _GpsNmea_RxMessageType
{
    GPSNMEA_RXMSG_ERROR,
    GPSNMEA_RXMSG_GGA,
    GPSNMEA_RXMSG_GLL,
    GPSNMEA_RXMSG_RMC,
    GPSNMEA_RXMSG_GSV,
    GPSNMEA_RXMSG_GSA,
    GPSNMEA_RXMSG_VTG,
    GPSNMEA_RXMSG_ZDA,
    GPSNEMA_RXMSG_PMTK001
} GpsNmea_RxMessageType;

typedef enum _GpsNmea_DeviceType
{
    GPSNMEA_DEVICE_GPS,
    GPSNMEA_DEVICE_GLONASS,
    GPSNMEA_DEVICE_GNSS
} GpsNmea_DeviceType;

typedef float GpsNmea_CoordinateType;

typedef union _GpsNmea_RxDataType
{
    struct
    {
        Time_TimeType          utcTime;
        Time_DateType          utcDate;
        GpsNmea_CoordinateType longitude;
        GpsNmea_CoordinateType latitude;
        float                  speed; /* Speed over ground in km/h */
        float                  heading;
    } rmc;
    
    struct
    {
        Time_TimeType          utcTime;
        Time_DateType          utcDate;        
    } zda;
} GpsNmea_RxDataType;

union GpsNmea_StatusType
{
    uint8_t status;
    
    struct {
        uint8_t commandReady        :1;
        uint8_t parsingMessage      :1;
        uint8_t notUsed             :6;
    } flags;
} extern GpsNmea_status;

GpsNmea_Errors GpsNmea_init (Uart_DeviceHandle device);
GpsNmea_Errors GpsNmea_enable (void);
GpsNmea_Errors GpsNmea_disable (void);

GpsNmea_RxMessageType GpsNmea_getReceiveMessageType (void);

GpsNmea_Errors GpsNmea_addReceiveChar (void);
GpsNmea_Errors GpsNmea_parseMessage (GpsNmea_RxDataType* data, GpsNmea_RxMessageType* msgType);

GpsNmea_Errors GpsNmea_isValidRmcMessage (void);

GpsNmea_Errors GpsNmea_sendSetNmeaOutput (uint8_t gllInterval, uint8_t rmcInterval,
                                          uint8_t vtgInterval, uint8_t ggaInterval,
                                          uint8_t gsaInterval, uint8_t gsvInterval,
                                          uint8_t zdaInterval, uint8_t reset);
#endif /* __GPS_NMEA_H */
