// JetsonNanoSPI.h
// Original Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2011 Mike McCauley
// Contributed by Joanna Rutkowska
// Modified by Adam Rasburn
// $Id: JetsonNanoSPI.h,v 1.12 2020/01/05 07:02:23 mikem Exp $

#ifndef JetsonNanoSPI_h
#define JetsonNanoSPI_h

#include <linux/spi/spidev.h>

#include "JetsonNano_gpio.h"
#include "RHGenericSPI.h"

/////////////////////////////////////////////////////////////////////
/// \class JetsonNanoSPI JetsonNanoSPI.h <JetsonNanoSPI.h>
/// \brief Encapsulate a hardware SPI bus interface
///
/// This concrete subclass of GenericSPIClass encapsulates the standard Arduino hardware and other
/// hardware SPI interfaces.
///
/// SPI transactions are supported in development environments that support it with SPI_HAS_TRANSACTION.
class JetsonNanoSPI : public RHGenericSPI
{
#ifdef RH_HAVE_HARDWARE_SPI
public:
    /// Constructor
    /// Creates an instance of a hardware SPI interface, using whatever SPI hardware is available on
    /// your processor platform. On Arduino and Uno32, uses SPI. On Maple, uses HardwareSPI.
    /// \param[in] frequency One of RHGenericSPI::Frequency to select the SPI bus frequency. The frequency
    /// is mapped to the closest available bus frequency on the platform.
    /// \param[in] bitOrder Select the SPI bus bit order, one of RHGenericSPI::BitOrderMSBFirst or 
    /// RHGenericSPI::BitOrderLSBFirst.
    /// \param[in] dataMode Selects the SPI bus data mode. One of RHGenericSPI::DataMode
    JetsonNanoSPI(Frequency frequency = Frequency1MHz, BitOrder bitOrder = BitOrderMSBFirst, DataMode dataMode = DataMode0);

    /// Transfer a single octet to and from the SPI interface
    /// \param[in] data The octet to send
    /// \return The octet read from SPI while the data octet was sent
    uint8_t transfer(uint8_t data);

    // SPI Configuration methods
    /// Enable SPI interrupts
    /// This can be used in an SPI slave to indicate when an SPI message has been received
    /// It will cause the SPI_STC_vect interrupt vectr to be executed
    void attachInterrupt();

    /// Disable SPI interrupts
    /// This can be used to diable the SPI interrupt in slaves where that is supported.
    void detachInterrupt();
    
    /// Initialise the SPI library
    /// Call this after configuring the SPI interface and before using it to transfer data.
    /// Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high. 
    void begin();

    /// Disables the SPI bus (leaving pin modes unchanged). 
    /// Call this after you have finished using the SPI interface.
    void end();
#else
    // not supported on ATTiny etc
    uint8_t transfer(uint8_t /*data*/) {return 0;}
    void begin(){}
    void end(){}

#endif

    /// Signal the start of an SPI transaction that must not be interrupted by other SPI actions
    /// In subclasses that support transactions this will ensure that other SPI transactions
    /// are blocked until this one is completed by endTransaction().
    /// Uses the underlying SPI transaction support if available as specified by SPI_HAS_TRANSACTION.
    virtual void beginTransaction();

    /// Signal the end of an SPI transaction
    /// Uses the underlying SPI transaction support if available as specified by SPI_HAS_TRANSACTION.
    virtual void endTransaction();

    /// Specify the interrupt number of the interrupt that will use SPI transactions
    /// Tells the SPI support software that SPI transactions will occur with the interrupt
    /// handler assocated with interruptNumber
    /// Uses the underlying SPI transaction support if available as specified by SPI_HAS_TRANSACTION.
    /// \param[in] interruptNumber The number of the interrupt
    virtual void usingInterrupt(uint8_t interruptNumber);

protected:

#if defined(SPI_HAS_TRANSACTION)
    // Storage for SPI settings used in SPI transactions
    SPISettings  _settings;
#endif
    int _fd;
};

// Built in default instance
extern JetsonNanoSPI jetson_nano_spi;

#endif
