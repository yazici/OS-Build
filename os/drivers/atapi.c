#include <stdint.h>

#include "atapi.h"
#include "../cpu/ioports.h"
#include "screen.h"

void resetATA(uint32_t bus)
{
    portByteOut(ATA_REG_DCR_ACR(bus), ATA_ACR_RESET);
    ATA_DRIVE_SWITCH_DELAY(bus);
    portByteOut(ATA_REG_DCR_ACR(bus), 0);
    ATA_DRIVE_SWITCH_DELAY(bus);
}

uint32_t isDrivePacketDevice(uint32_t bus)
{
    resetATA(bus);
    uint8_t sig1 = portByteIn(ATA_REG_SECTOR_COUNT(bus));
    uint8_t sig2 = portByteIn(ATA_REG_ADDR_LOW(bus));
    uint8_t sig3 = portByteIn(ATA_REG_ADDR_MID(bus));
    uint8_t sig4 = portByteIn(ATA_REG_ADDR_HIGH(bus));
    if (sig1 == 0x01 && sig2 == 0x01 && sig3 == 0x00 && sig4 == 0x00) {
        print("Non-packet device detected\n\0");
    } else if (sig1 == 0x01 && sig2 == 0x01 && sig3 == 0x14 && sig4 == 0xeb) {
        print("Packet device detected\n\0");
    } else {
        print("No valid packet data recorded\n\0");
        return 0;
    }
    return 1;
}

uint8_t identifyDrive(uint32_t bus, uint32_t drive)
{
    uint8_t status;
    portByteOut(ATA_REG_DRIVE(bus), drive);
    ATA_DRIVE_SWITCH_DELAY(bus);
    portByteOut(ATA_REG_SECTOR_COUNT(bus), 0);
    portByteOut(ATA_REG_ADDR_LOW(bus), 0);
    portByteOut(ATA_REG_ADDR_MID(bus), 0);
    portByteOut(ATA_REG_ADDR_HIGH(bus), 0);
    portByteOut(ATA_REG_COMMAND_STATUS(bus), ATA_COMMAND_IDENTIFY);
    status = portByteIn(ATA_REG_COMMAND_STATUS(bus));
    if (status == 0) {
        print("Drive does not exist\n\0");
        return 0;
    }
    uint8_t status_mid = portByteIn(ATA_REG_ADDR_MID(bus));
    uint8_t status_high = portByteIn(ATA_REG_ADDR_HIGH(bus));
    if (status_mid == 0 && status_high == 0) {
        print("ATA Drive exists\n\0");
    } else if (status_mid == 0x14 && status_high == 0xeb) {
        print("ATAPI drive exists\n\0");
    } else if (status_mid == 0x3c && status_high == 0xc3) {
        print("SATA Drive exists\n\0");
    } else {
        print("Unknown ATA-like drive exists\n\0");
    }
    printk("LBAmid: %x, LBAhigh: %x\n\0", status_mid, status_high);
    return 1;
}

void initializeATAPI()
{
    __asm__ ("cli");
    print("ATA controller primary:\n\0");
    uint8_t ata_status = portByteIn(ATA_REG_COMMAND_STATUS(ATA_BUS_ADDR_PRIMARY));
    if (ata_status == 0xff) {
        print("Nothing attached\n\0");
    } else {
        isDrivePacketDevice(ATA_BUS_ADDR_PRIMARY);
        identifyDrive(ATA_BUS_ADDR_PRIMARY, ATA_DRIVE_MASTER);
        identifyDrive(ATA_BUS_ADDR_PRIMARY, ATA_DRIVE_SLAVE);
    }
    resetATA(ATA_BUS_ADDR_PRIMARY);

    print("ATA controller secondary:\n\0");
    uint8_t ata_status2 = portByteIn(ATA_REG_COMMAND_STATUS(ATA_BUS_ADDR_SECONDARY));
    if (ata_status2 == 0xff) {
        print("Nothing attached\n\0");
    } else {
        isDrivePacketDevice(ATA_BUS_ADDR_SECONDARY);
        identifyDrive(ATA_BUS_ADDR_SECONDARY, ATA_DRIVE_MASTER);
        identifyDrive(ATA_BUS_ADDR_SECONDARY, ATA_DRIVE_SLAVE);
    }
    resetATA(ATA_BUS_ADDR_SECONDARY);
    __asm__ ("sti");
}

