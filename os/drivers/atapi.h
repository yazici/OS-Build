/* OS-Build. Copyright (C) 2017, 2018. Ben Watts-Jones.
 *
 * This program is distributed under the GNU General Public License Version 3 
 * (GPLv3) as published by the Free Software Foundation. You should have 
 * recieved a copy of the GPLv3 licence along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MECHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GPLv3 license for more details.
 */

#ifndef DRIVERS_ATAPI_H
#define DRIVERS_ATAPI_H

#include "../cpu/ioports.h"

// term used to describe bits all being set due to non-existant drive
#define FLOATING_BUS 0xff

// The starting addresses of the memory mapped IO for the primary and
// secondary ATA controllers. Valid values for "bus" parameter.
#define ATA_BUS_ADDR_PRIMARY 0x1f0
#define ATA_BUS_ADDR_SECONDARY 0x170

// Valid values of the drive register and "drive" parameter for master and
// slave drives on each bus.
#define ATA_DRIVE_MASTER 0xa0
#define ATA_DRIVE_SLAVE 0xb0

// Memory mapped IO port definitions, indexed relative to "bus" parameter.
// If different names for write/read, the write name is summerised first.
#define ATA_REG_DATA(x)           (x)
#define ATA_REG_FEATURES_ERROR(x) (x+1)
#define ATA_REG_SECTOR_COUNT(x)   (x+2)
#define ATA_REG_ADDR_LOW(x)       (x+3)
#define ATA_REG_ADDR_MID(x)       (x+4)
#define ATA_REG_ADDR_HIGH(x)      (x+5)
#define ATA_REG_DRIVE(x)          (x+6)
#define ATA_REG_COMMAND_STATUS(x) (x+7)
#define ATA_REG_DCR_ACR(x)        (x+0x206)

// 400ns delay required after switching drives. Can be achieved by reading
// the alternate status register (ATA_REG_DCR_STATUS) four times and ignoring 
// the result. (Done six times just in case.)
#define ATA_DRIVE_SWITCH_DELAY(bus) {portByteIn(ATA_REG_DCR_ACR(bus)); \
                                     portByteIn(ATA_REG_DCR_ACR(bus)); \
                                     portByteIn(ATA_REG_DCR_ACR(bus)); \
                                     portByteIn(ATA_REG_DCR_ACR(bus)); \
                                     portByteIn(ATA_REG_DCR_ACR(bus)); \
                                     portByteIn(ATA_REG_DCR_ACR(bus));}

#define ATAPI_COMMAND_IDENTIFY 0xa1 
#define ATA_COMMAND_IDENTIFY 0xec

// Definitions of the bits in the command/status port when read
#define ATA_STATUS_BUSY 0x80
#define ATA_STATUS_READY 0x40
#define ATA_STATUS_WRITE_FAULT 0x20
#define ATA_STATUS_SEEK_COMPLETE 0x10
#define ATA_STATUS_REQUEST_READY 0x8
#define ATA_STATUS_CORRECTED_DATA 0x4
#define ATA_STATUS_INDEX 0x2
#define ATA_STATUS_ERROR 0x1

// Definitions of the bits in the features/error port when read
#define ATA_ERROR_BAD_SECTOR 0x80
#define ATA_ERROR_UNCORRECTABLE_DATA 0x40
#define ATA_ERROR_NO_MEDIA 0x20
#define ATA_ERROR_NO_ID 0x10
#define ATA_ERROR_NO_MEDIA2 0x08
#define ATA_ERROR_ABORTED 0x04
#define ATA_ERROR_TRACK0_NOT_FOUND 0x02
#define ATA_ERROR_ERROR 0x01

// Definitions of the bits in the DCR (device control) / ACR (alternate status)
#define ATA_DCR_NEIN 0x01 // set nEIN to stop sending interrupts
#define ATA_DCR_SOFTWARE_RESET 0x04
#define ATA_DCR_HIGH_ORDER_BYTE 0x80

enum AtaReturnCodes
{
    ERROR,
    NO_DRIVE,
    DRIVE_EXISTS,
    UNKNOWN_DRIVE,
    SATA_DRIVE,
    ATA_DRIVE,
    ATAPI_DRIVE,
    PRIMARY_BUS,
    SECONDARY_BUS,
    MASTER_DEVICE,
    SLAVE_DEVICE,
    PRIMARY_MASTER = 0x01f000a0,
    PRIMARY_SLAVE = 0x01f000b0,
    SECONDARY_MASTER = 0x017000a0,
    SECONDARY_SLAVE = 0x017000b0,
};

void initializeATAPI();

#define ATA_PACKET_COMMAND 0xa0
#define ATA_COMMAND_READ_CAPACITY 0x25

#endif

