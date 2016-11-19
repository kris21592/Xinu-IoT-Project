/* ttyhandle_out.c - ttyhandle_out */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandle_out  -  Handle an output on a tty device by sending more
 *		     characters to the device FIFO (interrupts disabled)
 *------------------------------------------------------------------------
 */


void gpiohandle_out(
	 struct	gpiocblk	*gpioptr,		/* Ptr to gpiotab entry		*/
	 struct	gpio_csreg *csrptr	/* Address of GPIO's CSRs	*/
	)
{

	if ((gpioptr->port < 8) || (gpioptr->port > 9))		// if input is not port8 and port 9 , because BBB support P8/P9 Connector
		return;
	if ((gpioptr->pin < 1 ) || (gpioptr->pin > 46))		// if pin over/underflow , range : 1~46
		return;


	csrptr->oe &= ~(PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1]);
 	//printf("csrptr->oe = %x , value = %u" ,&csrptr->oe,csrptr->oe);
	//csrptr->set_data_out= PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];

	//printf("*gpioptr->gpiohead =  %u\n" ,*gpioptr->gpiohead);
	if (*gpioptr->gpiohead) {
		csrptr->set_data_out = PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];
		//kprintf("csrptr->set_data_out = %x , v = %d",&csrptr->set_data_out, csrptr->set_data_out);
	} else {
		csrptr->clr_data_out = PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];
		//kprintf("else\n");
	}
	//printf("csrptr->set_data_out = %x , value = %d",&csrptr->set_data_out, csrptr->set_data_out);
	gpioptr->gpiohead++;
	if (gpioptr->gpiohead>=&gpioptr->gpiobuf[GPIO_BUFLEN]) {
		gpioptr->gpiohead = gpioptr->gpiobuf;
	}
	signal(gpioptr->sem);
	
	return;
}
