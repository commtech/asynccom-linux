/*
	Copyright (C) 2016 Commtech, Inc.

	This file is part of Async Com-linux.

	Async Com-linux is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Async Com-linux is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Async Com-linux.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <linux/kernel.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>
#include <linux/slab.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/version.h>
#include "asynccom.h"
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/device.h>

struct attribute_group port_settings_attr_group;

struct asynccom_private {
	int sample_rate;
	int ACR;
};
		

static const struct usb_device_id fastcom_id_table[] = {	
	{ USB_DEVICE(COMMTECH_VENDOR_ID, 0x0031) },							
	{ }							
};				


#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)


static struct usb_driver asynccom_driver = {
	.name = "Async Com",
	.probe = usb_serial_probe,
	.disconnect = usb_serial_disconnect,
	.id_table = fastcom_id_table,
	};

#endif

static struct usb_serial_driver asynccom_device = {		
	.driver = {						
		.owner =	THIS_MODULE,			
		.name =		"Async_Com",			
	},
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)		
	.usb_driver = &asynccom_driver,
#endif					
	.id_table =		fastcom_id_table,		
	.num_ports =		1,				
	.description =		"Async Com usb asynchronous serial adapter",   
	.write =            asynccom_write, 
	//.probe =            asynccom_probe, 
	.port_probe =       asynccom_port_probe, 
	.port_remove =      asynccom_port_remove,
	.open =             asynccom_open, 
	.set_termios =      asynccom_set_termios, 
	.ioctl       =      asynccom_ioctl,  
	//.process_read_urb = asynccom_process_urb,
}; 




static struct usb_serial_driver * const serial_drivers[] = {
	&asynccom_device,
	NULL
};

int asynccom_open(struct tty_struct *tty, struct usb_serial_port *port)
{
	//asynccom_read(port);
	usb_submit_urb(port->read_urb, GFP_ATOMIC);
	return 0;
}

int asynccom_close(struct tty_struct *tty, struct usb_serial_port *port)
{
	//usb_kill_urb(port->read_urb);
	
	return 0;
}

/*static void write_callback(struct urb *urb)
 {
	// printk("urb complete\n");
	 usb_free_urb(urb);
 }
 */
 
 static void asynccom_read_callback(struct urb *urb)
 {
	
	 struct usb_serial_port *port;
	 size_t payload;
	 int i;
	 char flag;
	  port = urb->context;
	 flag = TTY_NORMAL;
	 payload = port->bulk_in_buffer[0] << 8;
	 payload |= port->bulk_in_buffer[1];
	 
	 for(i = 2; i < (payload + 2); i += 2){ //endianess 
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 26)
	    tty_insert_flip_char(&port->port, port->bulk_in_buffer[i+1], flag);
		if(i <= payload)
		     tty_insert_flip_char(&port->port, port->bulk_in_buffer[i], flag);
	}
	    tty_flip_buffer_push(&port->port);

#else

	   tty_insert_flip_char(port->port.tty, port->bulk_in_buffer[i+1], flag);
	   if(i <= payload)
		     tty_insert_flip_char(port->port.tty, port->bulk_in_buffer[i], flag);
	}
	    tty_flip_buffer_push(port->port.tty);

#endif
	// usb_free_urb(urb);//fix this so allocation doesn't need to reoccur with every read
	 usb_submit_urb(port->read_urb, GFP_ATOMIC);
	// asynccom_read(port);
	 
 }

/*static void asynccom_process_urb(struct urb *urb)
{
	
	struct usb_serial_port *port = urb->context;
	//char *ch = (char *)urb->transfer_buffer;
	int i = 0;
	size_t payload;
	//unsigned char temp = 0;
	 char flag;
	 flag = TTY_NORMAL;
	if (!urb->actual_length)
		return;
	
	  payload = port->bulk_in_buffer[0] << 8;
	  payload |= port->bulk_in_buffer[1];
	 
	 for(i = 2; i < (payload + 2); i += 2){ //endianess 
	    tty_insert_flip_char(&port->port, port->bulk_in_buffer[i+1], flag);
		if(i <= payload)
		     tty_insert_flip_char(&port->port, port->bulk_in_buffer[i], flag);
	}
	
	tty_insert_flip_char(&port->port, 0x31, flag);
	tty_flip_buffer_push(&port->port);
}
*/



int  asynccom_write(struct tty_struct *tty, struct usb_serial_port *port, const unsigned char *user_buffer, int count)
 {
	 
	struct usb_serial *serial = port->serial;
	//struct urb *urb;
	int i;
	char buf[500];
	int sent;
	memset(buf, 0, 499);
	
	for(i = 0; i < count; i++)
	{
		buf[i * 2] = user_buffer[i];
	}
	/*
	urb = usb_alloc_urb(0, GFP_ATOMIC);
	
	usb_fill_bulk_urb(urb, serial->dev,
					usb_sndbulkpipe(serial->dev,
					 6),
					 &buf, (count * 2), write_callback, port);
	
	usb_submit_urb(urb, GFP_ATOMIC);
	*/
	//for some bulk msgs are the only ones that work 
	 usb_bulk_msg(serial->dev, 
	        usb_sndbulkpipe(serial->dev, 6), &buf, 
		    (count * 2), &sent, 0);
	
	return count;
 }
	
/*static int asynccom_probe(struct usb_interface *interface,
			       const struct usb_device_id *id)
{
	//printk("probe test\n");
	return 0;
}
*/

static int asynccom_port_probe(struct usb_serial_port *port)
{	
	unsigned char clock_bits[20] = BAUD9600;
	
	(sysfs_create_group(&port->dev.kobj, &port_settings_attr_group)); 
		
	set_register(port, 0x0004, 0x01);

    set_register(port, 0x4006, 0x3);
		
	set_register(port, 0x0040, 0x01000000);
	
	set_register(port, 0x0004, 0x01);

    set_register(port, 0x4006, 0x3);
		
	set_register(port, 0x0040, 0x01000000);
	
	asynccom_port_set_clock_bits(port, clock_bits);
	
	//printk("%x\n", port->bulk_in_endpointAddress);
	port->read_urb->complete = asynccom_read_callback; 
	return 0;
}
					

static int asynccom_port_remove(struct usb_serial_port *port)
{	
	usb_kill_urb(port->read_urb);
	return 0;
}


void asynccom_read(struct usb_serial_port *port)
{
	struct usb_serial *serial = port->serial;
	struct urb *urb;
	
	
	urb = usb_alloc_urb(0, GFP_ATOMIC);
	
	usb_fill_bulk_urb(urb, serial->dev,
					usb_rcvbulkpipe(serial->dev,
					 82),
					 port->bulk_in_buffer, 512, asynccom_read_callback, port);
	
	usb_submit_urb(urb, GFP_ATOMIC);
}


static void asynccom_set_termios(struct tty_struct *tty, struct usb_serial_port *port, struct ktermios *old_termios)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 26)
	struct ktermios *termios = &tty->termios;
	unsigned int cflag = termios->c_cflag;
#else
	unsigned int cflag = tty->termios->c_cflag;

#endif

	int urb_value = 0;
	unsigned char orig_lcr;
	
	/*data bits*/
	switch (cflag & CSIZE) {
	case CS5:
		urb_value |= 0x00;
		break;
	case CS6:
		urb_value |= 0x01;
		break;
	case CS7:
		urb_value |= 0x02;
		
		break;
	default:
	case CS8:
		urb_value |= 0x03;
		
		break;
	}
	/*stop bits*/
	if(cflag & CSTOPB)
	   urb_value |= 0x04;
	
	/*Parity bits*/
	if(cflag & PARENB)
	   urb_value |= 0x08;
	   
	if(cflag & PARODD)
	   urb_value |= 0x10;
	   
	if(cflag & CMSPAR)
	   urb_value |= 0x20;
	
	set_register(port, UART_LCR, urb_value);
	
	/* Baud rate*/
	//switch (cflag & CBAUD)
	
	
	/* Flow control */
	if(cflag & CRTSCTS)
	{		
	  orig_lcr = get_register(port, UART_LCR);
	  set_register(port, UART_LCR, 0xbf);
	  set_register(port, UART_EFR, 0xD0);
	  set_register(port, UART_LCR, orig_lcr);	
    }
	
	if(!(cflag & CRTSCTS))
	{		
	  orig_lcr = get_register(port, UART_LCR);
	  set_register(port, UART_LCR, 0xbf);
	  set_register(port, UART_EFR, 0x00);
	  set_register(port, UART_LCR, orig_lcr);	
    }
}
	

void set_register(struct usb_serial_port *port, int addr, int val)
{
   	char msg[7];
	int count;
	struct usb_serial *serial = port->serial;
	
	msg[0] = 0x6A; //write command
	msg[1] = addr >> 8;
	msg[2] = addr;
	msg[3] = val >> 24;
    msg[4] = val >> 16;
	msg[5] = val >> 8;
	msg[6] = val;
	
	usb_bulk_msg(serial->dev, 
	usb_sndbulkpipe(serial->dev, 1), &msg, 
    sizeof(msg), &count, HZ*10);

  return;
}


__u32 get_register(struct usb_serial_port *port, int addr)
{
	struct usb_serial *serial = port->serial;
   	char msg[3];
	int count;
	__u32 val;
	
	
	msg[0] = 0x6B; //read command
	msg[1] = addr >> 8 & 0xFF;
	msg[2] = addr & 0xFF;
	
	usb_bulk_msg(serial->dev, 
	usb_sndbulkpipe(serial->dev, 1), &msg, 
    sizeof(msg), &count, HZ*10);
	
	usb_bulk_msg(serial->dev, 
	usb_rcvbulkpipe(serial->dev, 81), &val, 
    sizeof(val), &count, HZ*10);

   val = ((val>>24)&0xff) | ((val<<8)&0xff0000) | ((val>>8)&0xff00) | ((val<<24)&0xff000000);
  return val;
}


/********************************************************************************
Functions that handle the clock and baud rate setting.  The driver will recieve 
a desired baud rate from termios, then set the clock rate to sample * desired baud
rate using calculate_clock_bits.
********************************************************************************/

#define STRB_BASE 0x00000008
#define DTA_BASE 0x00000001
#define CLK_BASE 0x00000002
#define DEVICE_NAME "asynccom"





void asynccom_port_set_clock_bits(struct usb_serial_port *port,
							  unsigned char *clock_data)
{
	
	__u32 orig_fcr_value;
	__u32 new_fcr_value;
	int j = 0; // Must be signed because we are going backwards through the array
	int i = 0; // Must be signed because we are going backwards through the array
	unsigned strb_value = STRB_BASE;
	unsigned dta_value = DTA_BASE;
	unsigned clk_value = CLK_BASE;
	//unsigned long flags;
    char buf_data[4];
	__u32 *data = 0;
	unsigned data_index = 0;


	orig_fcr_value = 0;
	new_fcr_value = 0;

     clock_data[15] |= 0x04;
   

   
	data = kmalloc(sizeof(__u32) * 323, GFP_KERNEL);

	if (data == NULL) {
		printk(KERN_ERR DEVICE_NAME "kmalloc failed\n");
		return;
	}

	

	//spin_lock_irqsave(&port->board_settings_spinlock, flags);

	orig_fcr_value = get_register(port, 0x0040);

	data[data_index++] = new_fcr_value = orig_fcr_value & 0xfffff0f0;

	for (i = 19; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			int bit = ((clock_data[i] >> j) & 1);

            /* This is required for 4-port cards. I'm not sure why at the
               moment */
			//data[data_index++] = new_fcr_value;

			if (bit)
				new_fcr_value |= dta_value; /* Set data bit */
			else
				new_fcr_value &= ~dta_value; /* Clear data bit */

			data[data_index++] = new_fcr_value |= clk_value; /* Set clock bit */
			data[data_index++] = new_fcr_value &= ~clk_value; /* Clear clock bit */

			new_fcr_value = orig_fcr_value & 0xfffff0f0;
		}
	}

	new_fcr_value = orig_fcr_value & 0xfffff0f0;

	new_fcr_value |= strb_value; /* Set strobe bit */
	new_fcr_value &= ~clk_value; /* Clear clock bit	*/

	data[data_index++] = new_fcr_value;
	data[data_index++] = orig_fcr_value;

for(i = 0; i < 323; i++)
{
        buf_data[0] = data[i] >> 24; 
		buf_data[1] = data[i] >> 16; 
		buf_data[2] = data[i] >> 8;
		buf_data[3] = data[i] >> 0;
		
	asynccom_port_set_clock(port, &buf_data[0]);
}
	//spin_unlock_irqrestore(&port->board_settings_spinlock, flags);

	kfree(data);
}

void asynccom_port_set_clock(struct usb_serial_port *port, char *data)
{
	struct usb_serial *serial = port->serial;
    int count;
    char msg[7];
	
	msg[0] = 0x6a;
	msg[1] = 0x00;
	msg[2] = 0x40;
	msg[3] = data[0];
	msg[4] = data[1];
	msg[5] = data[2];
	msg[6] = data[3];

	
	usb_bulk_msg(serial->dev, 
	usb_sndbulkpipe(serial->dev, 1), &msg, 
    sizeof(msg), &count, HZ*10);
}






/****************************************************************************
*                         IOCTL                                             *
****************************************************************************/
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 26)

static int asynccom_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg)
#else
static int asynccom_ioctl(struct tty_struct *tty, struct file *file, unsigned int cmd, unsigned long arg)
#endif
{
	struct usb_serial_port *port = tty->driver_data;
	int error_code = 0;
	
	switch (cmd){
		
	case IOCTL_ASYNCCOM_ENABLE_RS485:
			 asynccom_set_rs485(port, 1);
		 break;
		
	case IOCTL_ASYNCCOM_DISABLE_RS485:
		     asynccom_set_rs485(port, 0);
	     break;
			 
	case IOCTL_ASYNCCOM_GET_RS485:
		asynccom_get_rs485(port, (unsigned *)arg);
		break;

    case IOCTL_ASYNCCOM_ENABLE_ECHO_CANCEL:
		asynccom_enable_echo_cancel(port);
		break;

    case IOCTL_ASYNCCOM_DISABLE_ECHO_CANCEL:
		asynccom_disable_echo_cancel(port);
		break;

    case IOCTL_ASYNCCOM_GET_ECHO_CANCEL:
		asynccom_get_echo_cancel(port, (unsigned *)arg);
		break;

    case IOCTL_ASYNCCOM_SET_SAMPLE_RATE:
		asynccom_set_sample_rate(port, (unsigned)arg);
		break;

    case IOCTL_ASYNCCOM_GET_SAMPLE_RATE:
		asynccom_get_sample_rate(port, (unsigned *)arg);
		break;

    case IOCTL_ASYNCCOM_SET_TX_TRIGGER:
		asynccom_set_tx_trigger(port, (unsigned)arg);
		break;

    case IOCTL_ASYNCCOM_GET_TX_TRIGGER:
		asynccom_get_tx_trigger(port, (unsigned *)arg);
		break;

    case IOCTL_ASYNCCOM_SET_RX_TRIGGER:
		asynccom_set_rx_trigger(port, (unsigned)arg);
		break;

    case IOCTL_ASYNCCOM_GET_RX_TRIGGER:
		asynccom_get_rx_trigger(port, (unsigned *)arg);
		break;

    case IOCTL_ASYNCCOM_SET_CLOCK_RATE:
		//asynccom_set_clock_rate(port, (unsigned)arg);
		error_code = -EPROTONOSUPPORT;
		break;

    case IOCTL_ASYNCCOM_SET_CLOCK_BITS:
		asynccom_port_set_clock_bits(port, (void *)arg);
		break;

    case IOCTL_ASYNCCOM_ENABLE_ISOCHRONOUS:
		asynccom_enable_isochronous(port, (unsigned)arg);
		break;

    case IOCTL_ASYNCCOM_DISABLE_ISOCHRONOUS:
		asynccom_disable_isochronous(port);
		break;

    case IOCTL_ASYNCCOM_GET_ISOCHRONOUS:
		asynccom_get_isochronous(port, (int *)arg);
		break;

    case IOCTL_ASYNCCOM_ENABLE_EXTERNAL_TRANSMIT:
		asynccom_set_external_transmit(port, (unsigned)arg);
		break;

    case IOCTL_ASYNCCOM_DISABLE_EXTERNAL_TRANSMIT:
		asynccom_set_external_transmit(port, 0);
		break;

    case IOCTL_ASYNCCOM_GET_EXTERNAL_TRANSMIT:
		asynccom_get_external_transmit(port, (unsigned *)arg);
		break;

    case IOCTL_ASYNCCOM_SET_FRAME_LENGTH:
		asynccom_set_frame_length(port, (unsigned)arg);
		break;

    case IOCTL_ASYNCCOM_GET_FRAME_LENGTH:
		asynccom_get_frame_length(port, (unsigned *)arg);
		break;

    case IOCTL_ASYNCCOM_ENABLE_9BIT:
		//asynccom_enable_9bit(port);
		error_code = -EPROTONOSUPPORT;
		break;

    case IOCTL_ASYNCCOM_DISABLE_9BIT:
		//asynccom_disable_9bit(port);
		error_code = -EPROTONOSUPPORT;
		break;

    case IOCTL_ASYNCCOM_GET_9BIT:
		//asynccom_get_9bit(port, (unsigned *)arg);
		error_code = -EPROTONOSUPPORT;
		break;

    case IOCTL_ASYNCCOM_SET_LINE_CNTL:
	     set_register(port, UART_LCR, (unsigned)arg);
		 break;
	
	case IOCTL_ASYNCCOM_SET_DIVISOR:
	     asynccom_set_divisor(port,(unsigned)arg);
		 break;

	case IOCTL_ASYNCCOM_ENABLE_FIXED_BAUD_RATE:
	case IOCTL_ASYNCCOM_DISABLE_FIXED_BAUD_RATE:
	case IOCTL_ASYNCCOM_GET_FIXED_BAUD_RATE:
        error_code = -EPROTONOSUPPORT;
        break;
	
	default:
		//dev_dbg(port->udev, "unknown ioctl 0x%x\n", cmd);
		return -ENOIOCTLCMD;
	}

	return error_code;
	
}


/****************************************************************************
*                               IOCTL Functions                             *
****************************************************************************/


void asynccom_set_divisor(struct usb_serial_port *port, int divisor)
{
	 unsigned char orig_lcr;
	 
	 orig_lcr = get_register(port, UART_LCR);
	 set_register(port, UART_LCR, (orig_lcr | 0x80)); //set divisor latch allowing access to DLL and DLM registers
	 set_register(port, UART_DLL, divisor);
	 set_register(port, UART_LCR, orig_lcr);
}

void asynccom_set_rs485(struct usb_serial_port *port, int enable)
{
	
	 unsigned char orig_lcr;
    __u32 orig_fcr;
    int ACR;

    orig_lcr = get_register(port, UART_LCR);
    orig_fcr = get_register(port, FPGA_FCR);

    set_register(port, UART_LCR, 0); /* Ensure last LCR value is not 0xbf */
    set_register(port, UART_SPR, ACR_OFFSET); /* To allow access to ACR */
    
	ACR = get_register(port, ACR_OFFSET);
	
    if (enable) {
        ACR |= 0x10;
        set_register(port, FPGA_FCR, (orig_fcr | EN485_CTL));
    }
    else {
        ACR &= ~0x10;
        set_register(port, FPGA_FCR, (orig_fcr & ~EN485_CTL));
    }

    set_register(port, ICR_OFFSET, ACR);
	
    set_register(port, UART_LCR, orig_lcr);
	
}

void asynccom_get_rs485(struct usb_serial_port *port, int *enabled)
{
	__u32 fcr_value = get_register(port, FPGA_FCR);
	int dtr_enable_active, transmitter_485_active;
	int ACR;
	
	ACR = get_register(port, ACR_OFFSET);
	transmitter_485_active = (fcr_value & EN485_CTL) ? 1 : 0;
	dtr_enable_active = (ACR & 0x10) ? 1 : 0;

    *enabled = (dtr_enable_active && transmitter_485_active) ? 1 : 0;
}


void asynccom_enable_echo_cancel(struct usb_serial_port *port)
{
	__u32 orig_fcr_value = get_register(port, FPGA_FCR);
	set_register(port, FPGA_FCR, (orig_fcr_value | RX_ECHO_CTL));
}

void asynccom_disable_echo_cancel(struct usb_serial_port *port)
{
	__u32 orig_fcr_value = get_register(port, FPGA_FCR);
	set_register(port, FPGA_FCR, (orig_fcr_value & ~RX_ECHO_CTL));
}

void asynccom_get_echo_cancel(struct usb_serial_port *port, int *enabled)
{
	__u32 fcr_value = get_register(port, FPGA_FCR);
	*enabled = (fcr_value & RX_ECHO_CTL) ? 1 : 0;
}

void asynccom_set_sample_rate(struct usb_serial_port *port, unsigned val)
{
	struct asynccom_private *priv;
	unsigned char orig_lcr;
	
	priv = usb_get_serial_port_data(port);
	
	orig_lcr = get_register(port, UART_LCR);
	set_register(port, UART_LCR, 0);
	set_register(port, UART_SPR, TCR_OFFSET);
	set_register(port, ICR_OFFSET, val);
	set_register(port, UART_LCR, orig_lcr);
	
	//priv->sample_rate = val;
}

void asynccom_get_sample_rate(struct usb_serial_port *port, unsigned *val)
{
	struct asynccom_private *priv;
	priv = usb_get_serial_port_data(port);
	//*val = priv->sample_rate;
}

void asynccom_set_tx_trigger(struct usb_serial_port *port, unsigned val)
{
	
	unsigned char orig_lcr;
	struct asynccom_private *priv;
	
	priv = usb_get_serial_port_data(port);
	
	orig_lcr = get_register(port, UART_LCR);
	set_register(port, UART_LCR, 0);
	set_register(port, UART_SPR, TTL_OFFSET);
	set_register(port, ICR_OFFSET, val);
	set_register(port, UART_LCR, orig_lcr);
	
	
}

void asynccom_get_tx_trigger(struct usb_serial_port *port, unsigned *val)
{
	
	unsigned char orig_lcr;
	unsigned char ttl;
	int ACR;
	
	ACR = get_register(port, ACR_OFFSET);
	orig_lcr = get_register(port, UART_LCR);
	set_register(port, UART_LCR, 0);
	set_register(port, UART_SPR, ACR_OFFSET);
	set_register(port, ICR_OFFSET, ACR | 0x40);
	set_register(port, UART_SPR, TTL_OFFSET);
	
	ttl = get_register(port, ICR_OFFSET);
	*val = ttl & 0x7F;
	set_register(port, UART_SPR, ACR_OFFSET);
	set_register(port, ICR_OFFSET, ACR);
	set_register(port, UART_LCR, orig_lcr);
}

void asynccom_set_rx_trigger(struct usb_serial_port *port, unsigned val)
{
	
	unsigned char orig_lcr;
	struct asynccom_private *priv;
	
	priv = usb_get_serial_port_data(port);
	
	orig_lcr = get_register(port, UART_LCR);
	set_register(port, UART_LCR, 0);
	set_register(port, UART_SPR, RTL_OFFSET);
	set_register(port, ICR_OFFSET, val);
	set_register(port, UART_LCR, orig_lcr);
	
	
}

void asynccom_get_rx_trigger(struct usb_serial_port *port, unsigned *val)
{
	
	unsigned char orig_lcr;
	unsigned char rtl;
	int ACR;
	
	ACR = get_register(port, ACR_OFFSET);
	orig_lcr = get_register(port, UART_LCR);
	set_register(port, UART_LCR, 0);
	set_register(port, UART_SPR, ACR_OFFSET);
	set_register(port, ICR_OFFSET, ACR | 0x40);
	set_register(port, UART_SPR, RTL_OFFSET);
	
	rtl = get_register(port, ICR_OFFSET);
	*val = rtl & 0x7F;
	set_register(port, UART_SPR, ACR_OFFSET);
	set_register(port, ICR_OFFSET, ACR);
	set_register(port, UART_LCR, orig_lcr);
}

void asynccom_disable_isochronous(struct usb_serial_port *port)
{
	asynccom_enable_isochronous(port, -1);
}


void asynccom_enable_isochronous(struct usb_serial_port *port, int mode)
{
	unsigned char orig_lcr;
	 unsigned char new_cks = 0;
    unsigned char new_mdm = 0;

  
    orig_lcr = get_register(port, UART_LCR);

    set_register(port, UART_LCR, 0);

    switch (mode) {
	
	/* turn isochronous off */	
	case -1:
	    new_cks |= 0x00;
        new_mdm |= 0x00;
		break;
		
    /* Enable receive using external DSR# */
    case 2:
    case 3:
    case 4:
    case 10:
        new_cks |= 0x09;
        new_mdm |= 0x02;
        break;

    /* Enable receive using internal BRG */
    case 5:
    case 6:
    case 7:
        new_cks |= 0x0A;
        break;

    /* Enable receive using transmit clock */
    case 8:
        new_cks |= 0x0B;
        break;
    }

    switch (mode) {
    /* Enable transmit using external RI# */
    case 0:
    case 3:
    case 6:
    case 8:
        new_cks |= 0xD0;
        new_mdm |= 0x04;
        break;

    /* Transmit using internal BRG */
    case 1:
    case 4:
    case 7:
        new_cks |= 0x90;
        break;

    case 9:
    case 10:
        new_cks |= 0x10;
        break;
    }
	
	
	set_register(port, UART_SPR, MDM_OFFSET);
	set_register(port, ICR_OFFSET, new_mdm);
	set_register(port, UART_SPR, CKS_OFFSET);
	set_register(port, ICR_OFFSET, new_cks);
	set_register(port, UART_LCR, orig_lcr);
	
}

void asynccom_get_isochronous(struct usb_serial_port *port, int *mode)
{
    unsigned char orig_lcr;
    unsigned char cks;
    int ACR;
	
	ACR = get_register(port, ACR_OFFSET);
    orig_lcr = get_register(port, UART_LCR);

    set_register(port, UART_LCR, 0);
	set_register(port, SPR_OFFSET, ACR_OFFSET);
    set_register(port, ICR_OFFSET, ACR | 0x40);
	set_register(port, SPR_OFFSET, CKS_OFFSET);
    
    cks = get_register(port, ICR_OFFSET);
   

    switch (cks) {
    case 0x00:
        *mode = -1;
        break;

    case 0xD0:
        *mode = 0;
        break;

    case 0x90:
        *mode = 1;
        break;

    case 0x09:
        *mode = 2;
        break;

    case 0xD9:
        *mode = 3;
        break;

    case 0x99:
        *mode = 4;
        break;

    case 0x0A:
        *mode = 5;
        break;

    case 0xDA:
        *mode = 6;
        break;

    case 0x9A:
        *mode = 7;
        break;

    case 0xDB:
        *mode = 8;
        break;
        break;

    case 0x10:
        *mode = 9;
        break;
        break;

    case 0x19:
        *mode = 10;
        break;
    }

    set_register(port, SPR_OFFSET, ACR_OFFSET);
    set_register(port, ICR_OFFSET, ACR_OFFSET);
    set_register(port, UART_LCR, orig_lcr);
    
}

int asynccom_set_external_transmit(struct usb_serial_port *port, unsigned num_chars)
{
    unsigned char orig_lcr;

    if (num_chars > 8191)
        return -EINVAL;

    orig_lcr = get_register(port, UART_LCR);
	set_register(port, UART_LCR, 0); /* Ensure last LCR value is not 0xbf */
   
   
    if (num_chars != 0) {
		set_register(port, UART_SPR, EXTH_OFFSET);
        set_register(port, ICR_OFFSET, 0x80 | (num_chars >> 8));
        set_register(port, UART_SPR, EXT_OFFSET);
        set_register(port, ICR_OFFSET, (char)num_chars);
        
    }
    else {
		set_register(port, UART_SPR, EXTH_OFFSET);
        set_register(port, ICR_OFFSET, 0x00);
        set_register(port, UART_SPR, EXT_OFFSET);
        set_register(port, ICR_OFFSET, 0x00);
    }

    set_register(port, UART_LCR, orig_lcr);

    return 0;
}

void asynccom_get_external_transmit(struct usb_serial_port *port, unsigned *num_chars)
{
    unsigned char orig_lcr;
    unsigned char ext, exth;
    int ACR;
	
	ACR = get_register(port, ACR_OFFSET);
    orig_lcr = get_register(port, UART_LCR);

    set_register(port, UART_LCR, 0); /* Ensure last LCR value is not 0xbf */
	set_register(port, UART_SPR, ACR_OFFSET);
	set_register(port, ICR_OFFSET, ACR | 0x40);
    set_register(port, UART_SPR, EXT_OFFSET);
    ext = get_register(port, ICR_OFFSET);
    set_register(port, UART_SPR, EXTH_OFFSET);
    exth = get_register(port, ICR_OFFSET);
    
    *num_chars = ((exth & 0x1F) << 8) + ext;

    set_register(port, UART_SPR, ACR_OFFSET);
    set_register(port, ICR_OFFSET, ACR);
    set_register(port, UART_LCR, orig_lcr);
    
}

int asynccom_set_frame_length(struct usb_serial_port *port, unsigned num_chars)
{
    unsigned char orig_lcr;
    
    if (num_chars == 0 || num_chars > 256)
        return -EINVAL;
    
    orig_lcr = get_register(port, UART_LCR);
    set_register(port, UART_LCR, 0); /* Ensure last LCR value is not 0xbf */

    set_register(port, UART_SPR, FLR_OFFSET);
    set_register(port, ICR_OFFSET, num_chars - 1);
    
    set_register(port, UART_LCR, orig_lcr);

    return 0;
}

int asynccom_get_frame_length(struct usb_serial_port *port, unsigned *num_chars)
{
    unsigned char orig_lcr;
    unsigned char flr;
    int ACR;
	
	ACR = get_register(port, ACR_OFFSET);

    orig_lcr = get_register(port, UART_LCR);

    set_register(port, UART_LCR, 0); /* Ensure last LCR value is not 0xbf */
	set_register(port, UART_SPR, ACR_OFFSET);
    set_register(port, ICR_OFFSET, ACR | 0x40);
    
	set_register(port, UART_SPR, FLR_OFFSET);
    flr = get_register(port, ICR_OFFSET);
    
    *num_chars = flr + 1;

    set_register(port, UART_SPR, ACR_OFFSET);
    set_register(port, ICR_OFFSET, ACR);
    set_register(port, UART_LCR, orig_lcr);
    

    return 0;
}



/****************************************************************************
*                                   sysfs                                   *
****************************************************************************/

static ssize_t echo_cancel_store(struct kobject *kobj,
							struct kobj_attribute *attr, const char *buf,
							size_t count)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;
	char *end = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

	value = (unsigned)simple_strtoul(buf, &end, 16);
    
	if(value == 1)
	   asynccom_enable_echo_cancel(port);
	
	if(value == 0)
	   asynccom_disable_echo_cancel(port);
	   
	return count;
}

static ssize_t echo_cancel_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
	struct usb_serial_port *port = 0;
	unsigned enabled = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

    asynccom_get_echo_cancel(port, &enabled);

	return sprintf(buf, "%i\n", enabled);
}




static ssize_t external_transmit_store(struct kobject *kobj,
							struct kobj_attribute *attr, const char *buf,
							size_t count)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;
	char *end = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

	value = (unsigned)simple_strtoul(buf, &end, 16);

	asynccom_set_external_transmit(port, value);

	return count;
}

static ssize_t external_transmit_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

   asynccom_get_external_transmit(port, &value);

	return sprintf(buf, "%i\n", value);
}



static ssize_t frame_length_store(struct kobject *kobj,
							struct kobj_attribute *attr, const char *buf,
							size_t count)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;
	char *end = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

	value = (unsigned)simple_strtoul(buf, &end, 16);

	asynccom_set_frame_length(port, value);

	return count;
}

static ssize_t frame_length_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

    asynccom_get_frame_length(port, &value);

	return sprintf(buf, "%i\n", value);
}



static ssize_t isochronous_store(struct kobject *kobj,
							struct kobj_attribute *attr, const char *buf,
							size_t count)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;
	char *end = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

	value = (unsigned)simple_strtoul(buf, &end, 16);

	asynccom_enable_isochronous(port, value);

	return count;
}

static ssize_t isochronous_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

    asynccom_get_isochronous(port, &value);

	return sprintf(buf, "%i\n", value);
}

static ssize_t rs485_store(struct kobject *kobj,
							struct kobj_attribute *attr, const char *buf,
							size_t count)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;
	char *end = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

	value = (unsigned)simple_strtoul(buf, &end, 16);

	asynccom_set_rs485(port, value);

	return count;
}

static ssize_t rs485_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
	struct usb_serial_port *port = 0;
	unsigned enabled = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

    asynccom_get_rs485(port, &enabled);

	return sprintf(buf, "%i\n", enabled);
}

static ssize_t rx_trigger_store(struct kobject *kobj,
							struct kobj_attribute *attr, const char *buf,
							size_t count)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;
	char *end = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

	value = (unsigned)simple_strtoul(buf, &end, 16);

	asynccom_set_rx_trigger(port, value);

	return count;
}

static ssize_t rx_trigger_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

    asynccom_get_rx_trigger(port, &value);

	return sprintf(buf, "%i\n", value);
}

static ssize_t sample_rate_store(struct kobject *kobj,
							struct kobj_attribute *attr, const char *buf,
							size_t count)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;
	char *end = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

	value = (unsigned)simple_strtoul(buf, &end, 16);

	asynccom_set_sample_rate(port, value);

	return count;
}

static ssize_t sample_rate_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;
    
	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

    asynccom_get_sample_rate(port, &value);

	return sprintf(buf, "%i\n", value);
}

static ssize_t tx_trigger_store(struct kobject *kobj,
							struct kobj_attribute *attr, const char *buf,
							size_t count)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;
	char *end = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

	value = (unsigned)simple_strtoul(buf, &end, 16);

	asynccom_set_tx_trigger(port, value);

	return count;
}


static ssize_t tx_trigger_show(struct kobject *kobj,
							struct kobj_attribute *attr, char *buf)
{
	struct usb_serial_port *port = 0;
	unsigned value = 0;

	port = (struct usb_serial_port *)dev_get_drvdata((struct device *)kobj);

    asynccom_get_tx_trigger(port, &value);

	return sprintf(buf, "%i\n", value);
}



static struct kobj_attribute echo_cancel_attribute =
	__ATTR(echo_cancel, 0660, echo_cancel_show, echo_cancel_store);

static struct kobj_attribute external_transmit_attribute =
	__ATTR(external_transmit, SYSFS_READ_WRITE_MODE, external_transmit_show, external_transmit_store);

static struct kobj_attribute frame_length_attribute =
	__ATTR(frame_length, SYSFS_READ_WRITE_MODE, frame_length_show, frame_length_store);

static struct kobj_attribute isochronous_attribute =
	__ATTR(isochronous, SYSFS_READ_WRITE_MODE, isochronous_show, isochronous_store);

static struct kobj_attribute rs485_attribute =
	__ATTR(rs485, SYSFS_READ_WRITE_MODE, rs485_show, rs485_store);

static struct kobj_attribute rx_trigger_attribute =
	__ATTR(rx_trigger, SYSFS_READ_WRITE_MODE, rx_trigger_show, rx_trigger_store);

static struct kobj_attribute sample_rate_attribute =
	__ATTR(sample_rate, SYSFS_READ_WRITE_MODE, sample_rate_show, sample_rate_store);

static struct kobj_attribute tx_trigger_attribute =
	__ATTR(tx_trigger, SYSFS_READ_WRITE_MODE, tx_trigger_show, tx_trigger_store);


static struct attribute *settings_attrs[] = {
	&echo_cancel_attribute.attr,
	&external_transmit_attribute.attr,
	&frame_length_attribute.attr,
	&isochronous_attribute.attr,
	&rs485_attribute.attr,
	&rx_trigger_attribute.attr,
	&sample_rate_attribute.attr,
	&tx_trigger_attribute.attr,
	NULL,
};


struct attribute_group port_settings_attr_group = {
	.name = "settings",
	.attrs = settings_attrs,
};

extern struct attribute_group port_settings_attr_group;

MODULE_DEVICE_TABLE(usb, fastcom_id_table);


#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
module_usb_serial_driver(serial_drivers, fastcom_id_table);

#else
static int __init asynccom_init(void)
	{
		return usb_serial_register_drivers(serial_drivers, KBUILD_MODNAME, fastcom_id_table);
	}

static int __exit asynccom_exit(void)
	{
		usb_serial_deregister_drivers(serial_drivers);
	}

module_init(asynccom_init);
module_exit(asynccom_exit);

#endif


MODULE_LICENSE("GPL");
MODULE_VERSION("1.1.0");
MODULE_AUTHOR("Landon Unruh <landon@fastcomproducts.com>");
MODULE_DESCRIPTION("Driver for the Async Com series of cards from Commtech, Inc.");