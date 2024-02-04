#include <kernel/sys/io.h>
#include <kernel/cmos.h>
#include <stdint.h>
#include <common.h>

const char * cmos_weekdays[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char * cmos_months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

uint8_t cmos_get_register(uint8_t reg)
{
  IoPortByteWrite(CMOS_ACCESS, reg);
  return IoPortByteRead(CMOS_DATA);
}

uint8_t cmos_get_update_flag(void)
{
  IoPortByteWrite(CMOS_ACCESS, CMOS_STATUS_REG_A);
  return (IoPortByteRead(CMOS_DATA) & 0x80);
}