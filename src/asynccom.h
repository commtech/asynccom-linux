/*
	Copyright (C) 2014  Commtech, Inc.

	This file is part of asynccom-linux.

	asynccom-linux is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	asynccom-linux is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with asynccom-linux.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef ASYNCCOM_H
#define ASYNCCOM_H

#define COMMTECH_VENDOR_ID 0x2eb0

#define SYSFS_READ_WRITE_MODE 0660

#define DEFAULT_BITS {0x32, 0x0, 0xb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x82, 0x6a, 0x47, 0x1, 0x84, 0x0, 0xff, 0xff, 0xff}
#define BAUD9600     {0x1, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8a, 0xb2, 0x66, 0x1, 0x84, 0x0, 0xff, 0xff, 0xff}
#define BAUD230400   {0x32, 0x0, 0xb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x82, 0x62, 0x47, 0x1, 0x84, 0x0, 0xff, 0xff, 0xff}

//16C950 register addresses
#define UART_FCR 0x4004
#define UART_ISR 0x4004
#define UART_LCR 0x4006
#define UART_MCR 0x4008
#define UART_LSR 0x400A
#define UART_MSR 0x400C
#define UART_SPR 0x400E

//16C950 extended registers when LCR 0xBF
#define UART_DLL 0x4000
#define UART_DLM 0x4002
#define UART_EFR 0x4004
#define UART_XON 0x4008
#define UART_XOFF 0x400C

//F-CORE register addresses
#define FPGA_FCR 0x0040
#define FPGA_MCR 0x0004

//FPGA MCR commands
#define FIFO_RESET 0x80
#define EP_FLUSH 0x08
#define UART_MODE 0x01

//FCR commands
#define USE_EXT_UART 0x01000000
#define EN485_CTL 0x00040000
#define RX_ECHO_CTL 0x00010000

//UART FCR commands
#define EN_FIFO 0x1
#define FLUSH_RHR 0x2
#define FLUSH_THR 0x4

//UART LCR commands
#define EN_PAR 0x8
#define EVEN_PAR 0x10
#define FORCE_PAR 0x20

//indexed control registers
#define ICR_OFFSET 0x400A

#define ACR_OFFSET 0x00
#define TCR_OFFSET 0x02
#define CKS_OFFSET 0x03
#define RTL_OFFSET 0x05
#define MDM_OFFSET 0x0e
#define EXT_OFFSET 0x16
#define EXTH_OFFSET 0x17
#define FLR_OFFSET 0x20

//extended registers 650
#define EFR_OFFSET 0x2
#define TTL_OFFSET 0x4

/* Normal registers */
#define FCR_OFFSET 0x2
#define LCR_OFFSET 0x3
#define MCR_OFFSET 0x4
#define SPR_OFFSET 0x7

/* FSCC Bar0 register set */
#define VSTR_OFFSET 0x4c

struct ioctl_get_dev_info_struct {
      short vendor;       /* vendor ID */
      short device;       /* device ID */
      unsigned char bus;  /* PCI bus number */
      unsigned char slot; /* slot number */
};

#include <linux/fs.h>

#define SERIALFC_IOCTL_MAGIC 0x19

#define IOCTL_ASYNCCOM_ENABLE_RS485 _IO(SERIALFC_IOCTL_MAGIC, 0)
#define IOCTL_ASYNCCOM_DISABLE_RS485 _IO(SERIALFC_IOCTL_MAGIC, 1)
#define IOCTL_ASYNCCOM_GET_RS485 _IOR(SERIALFC_IOCTL_MAGIC, 2, unsigned *)

#define IOCTL_ASYNCCOM_ENABLE_ECHO_CANCEL _IO(SERIALFC_IOCTL_MAGIC, 3)
#define IOCTL_ASYNCCOM_DISABLE_ECHO_CANCEL _IO(SERIALFC_IOCTL_MAGIC, 4)
#define IOCTL_ASYNCCOM_GET_ECHO_CANCEL _IOR(SERIALFC_IOCTL_MAGIC, 5, unsigned *)

#define IOCTL_ASYNCCOM_ENABLE_TERMINATION _IO(SERIALFC_IOCTL_MAGIC, 6)
#define IOCTL_ASYNCCOM_DISABLE_TERMINATION _IO(SERIALFC_IOCTL_MAGIC, 7)
#define IOCTL_ASYNCCOM_GET_TERMINATION _IOR(SERIALFC_IOCTL_MAGIC, 8, unsigned *)

#define IOCTL_ASYNCCOM_SET_SAMPLE_RATE _IOW(SERIALFC_IOCTL_MAGIC, 9, const unsigned)
#define IOCTL_ASYNCCOM_GET_SAMPLE_RATE _IOR(SERIALFC_IOCTL_MAGIC, 10, unsigned *)

#define IOCTL_ASYNCCOM_SET_TX_TRIGGER _IOW(SERIALFC_IOCTL_MAGIC, 11, const unsigned)
#define IOCTL_ASYNCCOM_GET_TX_TRIGGER _IOR(SERIALFC_IOCTL_MAGIC, 12, unsigned *)

#define IOCTL_ASYNCCOM_SET_RX_TRIGGER _IOW(SERIALFC_IOCTL_MAGIC, 13, const unsigned)
#define IOCTL_ASYNCCOM_GET_RX_TRIGGER _IOR(SERIALFC_IOCTL_MAGIC, 14, unsigned *)

#define IOCTL_ASYNCCOM_SET_CLOCK_RATE _IOW(SERIALFC_IOCTL_MAGIC, 15, const unsigned)

#define IOCTL_ASYNCCOM_ENABLE_ISOCHRONOUS _IOW(SERIALFC_IOCTL_MAGIC, 16, const unsigned)
#define IOCTL_ASYNCCOM_DISABLE_ISOCHRONOUS _IO(SERIALFC_IOCTL_MAGIC, 17)
#define IOCTL_ASYNCCOM_GET_ISOCHRONOUS _IOR(SERIALFC_IOCTL_MAGIC, 18, unsigned *)

#define IOCTL_ASYNCCOM_ENABLE_EXTERNAL_TRANSMIT _IOW(SERIALFC_IOCTL_MAGIC, 19, const unsigned)
#define IOCTL_ASYNCCOM_DISABLE_EXTERNAL_TRANSMIT _IO(SERIALFC_IOCTL_MAGIC, 20)
#define IOCTL_ASYNCCOM_GET_EXTERNAL_TRANSMIT _IOR(SERIALFC_IOCTL_MAGIC, 21, unsigned *)

#define IOCTL_ASYNCCOM_SET_FRAME_LENGTH _IOW(SERIALFC_IOCTL_MAGIC, 22, const unsigned)
#define IOCTL_ASYNCCOM_GET_FRAME_LENGTH _IOR(SERIALFC_IOCTL_MAGIC, 23, int *)

#define IOCTL_ASYNCCOM_GET_CARD_TYPE _IOR(SERIALFC_IOCTL_MAGIC, 24, unsigned *)

#define IOCTL_ASYNCCOM_ENABLE_9BIT _IO(SERIALFC_IOCTL_MAGIC, 25)
#define IOCTL_ASYNCCOM_DISABLE_9BIT _IO(SERIALFC_IOCTL_MAGIC, 26)
#define IOCTL_ASYNCCOM_GET_9BIT _IOR(SERIALFC_IOCTL_MAGIC, 27, unsigned *)

#define IOCTL_ASYNCCOM_SET_CLOCK_BITS _IOW(SERIALFC_IOCTL_MAGIC, 28, const unsigned char *)

#define IOCTL_ASYNCCOM_ENABLE_FIXED_BAUD_RATE _IOW(SERIALFC_IOCTL_MAGIC, 29, const unsigned)
#define IOCTL_ASYNCCOM_DISABLE_FIXED_BAUD_RATE _IO(SERIALFC_IOCTL_MAGIC, 30)
#define IOCTL_ASYNCCOM_GET_FIXED_BAUD_RATE _IOR(SERIALFC_IOCTL_MAGIC, 31, unsigned *)

#define IOCTL_ASYNCCOM_GET_DEV_INFO _IOR(SERIALFC_IOCTL_MAGIC, 32, struct ioctl_get_dev_info_struct *)

#define IOCTL_ASYNCCOM_GET_BAUD_RATE _IOR(SERIALFC_IOCTL_MAGIC, 33, unsigned long*)
//#define IOCTL_ASYNCCOM_SET_BAUD_RATE _IOW(SERIALFC_IOCTL_MAGIC, 34, unsigned long)
#define IOCTL_ASYNCCOM_SET_LINE_CNTL _IOW(SERIALFC_IOCTL_MAGIC, 35, unsigned long)

#define IOCTL_ASYNCCOM_SET_DIVISOR _IOW(SERIALFC_IOCTL_MAGIC, 36, unsigned long)


/**************************************************************************************************************************
*                                                                                                                         *
*                                                                                                                         *
*                                                Function prototypes                                                      *
*                                                                                                                         *
*                                                                                                                         *
**************************************************************************************************************************/


int  asynccom_write(struct tty_struct *tty, struct usb_serial_port *port, const unsigned char *user_buffer, int count);

//static int asynccom_probe(struct usb_interface *interface, const struct usb_device_id *id);

static int asynccom_port_probe(struct usb_serial_port *port);
static int asynccom_port_remove(struct usb_serial_port *port);

void asynccom_set_divisor(struct usb_serial_port *port, int divisor);

int asynccom_open(struct tty_struct *tty, struct usb_serial_port *port);

void asynccom_read(struct usb_serial_port *port);

//static void asynccom_process_urb(struct urb *urb);

static void asynccom_set_termios(struct tty_struct *tty, struct usb_serial_port *port, struct ktermios *old_termios);

//void asynccom_set_baud_rate(struct usb_serial_port *port, int baud_rate);

void asynccom_port_set_clock(struct usb_serial_port *port, char *data);

void asynccom_port_set_clock_bits(struct usb_serial_port *port, unsigned char *clock_data);

static int asynccom_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg);

void set_register(struct usb_serial_port *port, int addr, int val);
__u32 get_register(struct usb_serial_port *port, int addr);
								
void asynccom_set_sample_rate(struct usb_serial_port *port, unsigned val);
void asynccom_get_sample_rate(struct usb_serial_port *port, unsigned *val);

void asynccom_set_tx_trigger(struct usb_serial_port *port, unsigned val);
void asynccom_get_tx_trigger(struct usb_serial_port *port, unsigned *val);

void asynccom_set_rx_trigger(struct usb_serial_port *port, unsigned val);
void asynccom_get_rx_trigger(struct usb_serial_port *port, unsigned *val);

void asynccom_set_echo_cancel(struct usb_serial_port *port, int enable);
void asynccom_enable_echo_cancel(struct usb_serial_port *port);
void asynccom_disable_echo_cancel(struct usb_serial_port *port);
void asynccom_get_echo_cancel(struct usb_serial_port *port, int *enabled);

void asynccom_set_rs485(struct usb_serial_port *port, int enable);
void asynccom_get_rs485(struct usb_serial_port *port, int *enabled);

int asynccom_set_clock_bits(struct usb_serial_port *port, void *clock_data);

void asynccom_set_isochronous(struct usb_serial_port *port, int mode);
void asynccom_enable_isochronous(struct usb_serial_port *port, int mode);
void asynccom_disable_isochronous(struct usb_serial_port *port);
void asynccom_get_isochronous(struct usb_serial_port *port, int *mode);

int asynccom_set_external_transmit(struct usb_serial_port *port, unsigned num_chars);
int asynccom_enable_external_transmit(struct usb_serial_port *port, unsigned num_chars);
int asynccom_disable_external_transmit(struct usb_serial_port *port);
void asynccom_get_external_transmit(struct usb_serial_port *port, unsigned *num_chars);

int asynccom_set_frame_length(struct usb_serial_port *port, unsigned num_chars);
int asynccom_get_frame_length(struct usb_serial_port *port, unsigned *num_chars);

/**************************************************************************************/

#endif
