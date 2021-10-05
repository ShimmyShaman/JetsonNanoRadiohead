// JetsonNanoSPI.cpp
// Original Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2011 Mike McCauley
// Contributed by Joanna Rutkowska
// Modified by Adam Rasburn
// $Id: JetsonNanoSPI.cpp,v 1.29 2020/08/04 09:02:14 mikem Exp $

#include <fcntl.h>
#include <stdio.h>
// #include <linux/ioctl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "JetsonNanoSPI.h"

#if (RH_PLATFORM == RH_PLATFORM_JETSON_NANO)

static void pabort(const char *s)
{
  if (errno != 0) {
    printf("errno:%i\n", errno);
    // fprintf(stderr, "")
    perror(s);
  }
  else
    printf("%s\n", s);

  abort();
}

// Declare a single default instance of the hardware SPI interface class
JetsonNanoSPI jetson_nano_spi;

JetsonNanoSPI::JetsonNanoSPI(Frequency frequency, BitOrder bitOrder, DataMode dataMode)
    : RHGenericSPI(frequency, bitOrder, dataMode)
{
  puts("JetsonNanoSPI()");
  fprintf(stderr, "opening spidev0.0\n");
  usleep(1000);

  int r = 0;
  while (r < 10) {
    _fd = open("/dev/spidev0.0", O_RDWR);
    if (_fd >= 0) {
      fprintf(stderr, "opened spidev0.0\n");
      return;
    }
    ++r;
    usleep(1000000 * r);
  }

  pabort("Could not open SPI DEVICE 0.0");
}

uint8_t JetsonNanoSPI::transfer(uint8_t data)
{
  int ret;
  struct spi_ioc_transfer tr = {
      .tx_buf = (unsigned long)&data,
      .rx_buf = (unsigned long)&data,
      .len = 1,
      .speed_hz = 500000, // TODO
      .delay_usecs = 100,
      .bits_per_word = 8,
  };

  if (data) {
    tr.rx_buf = (unsigned long)0;
    tr.tx_nbits = 8;
  }
  else {
    tr.tx_buf = (unsigned long)0;
    tr.rx_nbits = 8;
  }

  // /*if (mode & SPI_TX_OCTAL)
  // 	tr.tx_nbits = 8;
  // else*/ if (mode & SPI_TX_QUAD)
  // 	tr.tx_nbits = 4;
  // else if (mode & SPI_TX_DUAL)
  // 	tr.tx_nbits = 2;
  // /*if (mode & SPI_RX_OCTAL)
  // 	tr.rx_nbits = 8;
  // else*/ if (mode & SPI_RX_QUAD)
  // 	tr.rx_nbits = 4;
  // else if (mode & SPI_RX_DUAL)
  // 	tr.rx_nbits = 2;
  // if (!(mode & SPI_LOOP)) {
  // 	if (mode & (/*SPI_TX_OCTAL |*/ SPI_TX_QUAD | SPI_TX_DUAL))
  // 		tr.rx_buf = 0;
  // 	else if (mode & (/*SPI_RX_OCTAL |*/ SPI_RX_QUAD | SPI_RX_DUAL))
  // 		tr.tx_buf = 0;
  // }

#ifdef VERBOSE
  uint8_t bef = data;
#endif

  ret = ioctl(this->_fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1) {
    perror("ioctl error. SPI msg not sent");
  }
#ifdef VERBOSE
  printf("SPI sent: 0x%02X", bef);
  if (bef != data)
    printf(" => 0x%02X\n", data);
  else
    puts("");
#endif

  return data;

  // if (verbose)
  // 	hex_dump(tx, len, 32, "TX");

  // if (output_file) {
  // 	out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  // 	if (out_fd < 0)
  // 		pabort("could not open output file");

  // 	ret = write(out_fd, rx, len);
  // 	if (ret != len)
  // 		pabort("not all bytes written to output file");

  // 	close(out_fd);
  // }

  // // return SPI.transfer(data);

  // return 0;
}

void JetsonNanoSPI::attachInterrupt()
{
  // #if (RH_PLATFORM == RH_PLATFORM_ARDUINO || RH_PLATFORM == RH_PLATFORM_NRF52)
  //     SPI.attachInterrupt();
  // #endif
}

void JetsonNanoSPI::detachInterrupt()
{
  // #if (RH_PLATFORM == RH_PLATFORM_ARDUINO || RH_PLATFORM == RH_PLATFORM_NRF52)
  //     SPI.detachInterrupt();
  // #endif
}

/// Call this after configuring the SPI interface and before using it to transfer data.
/// Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high.
void JetsonNanoSPI::begin()
{
  // gpio_export()

  // uint8_t dataMode;
  // if (_dataMode == DataMode0)
  //     dataMode = 0;
  // else if (_dataMode == DataMode1)
  //     dataMode = SPI_CPHA;
  // else if (_dataMode == DataMode2)
  //     dataMode = SPI_CPOL;
  // else if (_dataMode == DataMode3)
  //     dataMode = SPI_CPHA | SPI_CPOL;

  // uint8_t bitOrder;
  // if (_bitOrder == BitOrderLSBFirst)
  //     bitOrder = BCM2835_SPI_BIT_ORDER_LSBFIRST;
  // else
  //     bitOrder = BCM2835_SPI_BIT_ORDER_MSBFIRST;

  // uint32_t divider;
  // switch (_frequency)
  // {
  // case Frequency1MHz:
  // default:
  //     divider = BCM2835_SPI_CLOCK_DIVIDER_256;
  //     break;
  // case Frequency2MHz:
  //     divider = BCM2835_SPI_CLOCK_DIVIDER_128;
  //     break;
  // case Frequency4MHz:
  //     divider = BCM2835_SPI_CLOCK_DIVIDER_64;
  //     break;
  // case Frequency8MHz:
  //     divider = BCM2835_SPI_CLOCK_DIVIDER_32;
  //     break;
  // case Frequency16MHz:
  //     divider = BCM2835_SPI_CLOCK_DIVIDER_16;
  //     break;
  // }
  // SPI.begin(divider, bitOrder, dataMode);*/
}

void JetsonNanoSPI::end()
{
  puts("TODO5552");
  // return SPI.end();
}

void JetsonNanoSPI::beginTransaction()
{
  // #if defined(SPI_HAS_TRANSACTION)
  //     SPI.beginTransaction(_settings);
  // #endif
}

void JetsonNanoSPI::endTransaction()
{
  // #if defined(SPI_HAS_TRANSACTION)
  //     SPI.endTransaction();
  // #endif
}

void JetsonNanoSPI::usingInterrupt(uint8_t interrupt)
{
  // #if defined(SPI_HAS_TRANSACTION) && !defined(RH_MISSING_SPIUSINGINTERRUPT)
  //     SPI.usingInterrupt(interrupt);
  // #endif
  //     (void)interrupt;
}

#endif
