#include <stdio.h>
#include <usb.h>
//#include "stdafx.h"
#include "CIUsbLib.h"
#include "BMC_Mappings.h"

#define TIMEOUT   1000
#define BYTESPERLINE 16

#define MAPPING mapMultiDM01

void printbytes(char *buffer, int count)
{
  int i;
  
  // printf("Hex Dump follows... 0x%x %d ", buffer, count);
  
  if(count==0) return;
  
  for(i=0;i<count;i++) {
    if(i != 0 && i%BYTESPERLINE == 0)
      printf("\n%08X: ", i);
    
    printf("%02X ", (int)(0x00FF & buffer[i]));
    
  }
  
  printf("\n");
}

int vcmd(usb_dev_handle *udev, int request, int value, int index, int size, char *bytes)
{
  int err;
  
  // printf("\nSending a mysterious vendor command: 0x%04X\n", request);
  printf("Mysterious UNDOCUMENTED vendor command: %02X %04X %04X %5d: \n", request,value,index,size);
  printbytes(bytes,size);	
  
  err = usb_control_msg(udev,USB_TYPE_VENDOR,request,value,index,bytes,size,TIMEOUT)<0;
  if(err) {
    printf("  (err=%d) >>> FAILED TO COMPLETE MYSTERIOUS VENDOR COMMAND!\n",err);
    return(err);
  } 
  
  // printf("Data returned >>> \n"); 
  //printf("VENDOR_RECV: %04X %04X %04X %04X: ", request,value,index,size);
  //printbytes(bytes,size);	
  //printf("\n");
  return(0);
}

void clearBuffer(u_int16_t buf[], int N) {
  int n;
  for(n=0;n<N;n++) {
    buf[n] = 0;
  }
}

u_int16_t *mapActs(u_int16_t *FROM, u_int16_t *TO, const int actMap[]) {
  int n;
  for(n=0;n<NUM_ACTUATORS;n++)
    TO[n] = FROM[actMap[n]];
  return(TO);
}

int main(int argc, char **argv)
{
  struct usb_bus *bus;
  struct usb_device *dev;
  int readlen = 0;
  char string[256];
  int err;
  
  printf("argc = %d\n", argc);
  
  if (argc > 1) {
    readlen = atoi(argv[1]);
    printf("argv: %s  --> %d\n",argv[1],readlen);
  }
  
  usb_init();
  
  usb_find_busses();
  usb_find_devices();
  
  printf("Scanning USB busses...\n");
  
  for (bus = usb_busses; bus; bus = bus->next) {
    for (dev = bus->devices; dev; dev = dev->next) {
      int ret;
      //char data[256];
      usb_dev_handle *udev;
      
      if (dev->descriptor.idVendor == BMC_USB_VENDOR) {
	printf("Located a Boston MicroMachines device on USB %s.%s\n",bus->dirname, dev->filename);
      } else {
	continue;
      }
      
      if (dev->descriptor.idProduct == BMC_USB_MULTIDRIVER ) {
	printf("It checks out as a BMC Multi-Driver.\n");
      } else {
	printf("I don't know what the heck this thing is.\n");
	printf("bus=%s device=%s     Vendor=0x%04X  Product=0x%04X\n",
	       bus->dirname, dev->filename,
	       dev->descriptor.idVendor, dev->descriptor.idProduct);
	continue;
      }
      
      udev = usb_open(dev);
      if (udev) {
	if (dev->descriptor.iManufacturer) {
	  ret = usb_get_string_simple(udev, dev->descriptor.iManufacturer, string, sizeof(string));
	  
	  if (ret > 0)
	    printf("- Manufacturer : %s\n", string);
	  else
	    printf("- Unable to fetch manufacturer string\n");
	}
	
	if (dev->descriptor.iProduct) {
	  ret = usb_get_string_simple(udev, dev->descriptor.iProduct, string, sizeof(string));
	  if (ret > 0)
	    printf("- Product      : %s\n", string);
	  else
	    printf("- Unable to fetch product string\n");
	}
	
	// Start doing something interesting...
	printf("\n ... This is where I start to play with the DM...\n\n");
	
	usb_set_debug(5);
	
	if (usb_claim_interface(udev,0)<0) {
	  printf("Couldn't claim interface.\n");
	  continue;
	}
	printf("I have CLAIMED the interface.\n");
	
	// bmReqType	Recipient	Type	Dir	bReq	wValue	wIndex	wLength	[bmReqType]
	// 0xC0		0		10	1	0xF0	0	0	0x16	USB_TYPE_VENDOR dev->host
	// 										
	// 0x40		0		10	0	0xF5	0	0x0002	0	USB_TYPE_VENDOR host->dev
	// 0x40		0		10	0	0xF5	0	0x0082	0	USB_TYPE_VENDOR host->dev
	// 0x40		0		10	0	0xF5	0	0x0088	0	USB_TYPE_VENDOR host->dev
	// 										
	// Data block transfer...							
	// 										
	// 0x40		0		10	0	0xF5	0	0x0008	0	USB_TYPE_VENDOR host->dev
	
	//int vcmd(usb_dev_handle *udev, int request, int value, int index, int size, char *bytes);
	
	// This appears to just be an ID read command.  It can be done using normal commands above.

	string[0] = 0;
	err = vcmd(udev, 0xF0, 0, 0, 0x16, string); // Returns: "DM Driver USB Interface"
	//err = vcmd(udev, 0xC0, 0, 0, 0x16, string); // Returns: "DM Driver USB Interface"
	if(err) {
	  printf("First vendor read command failed.  err=%d\n",err);
	  continue;
	} 
	printf("vendor command returned <%s>\n",string);
	  
	  
	err = vcmd(udev, 0xF5, 0, 0x0002, 0, string); 
	if(err) {
	  printf("Mysterious undoc command 0x0002 failed.  err=%d\n",err);
	  continue;
	} 
	  
	err = vcmd(udev, 0xF5, 0, 0x0082, 0, string); 
	if(err) {
	  printf("Mysterious undoc command 0x0082 failed.  err=%d\n",err);
	  continue;
	} 
	  
	  err = vcmd(udev, 0xF5, 0, 0x0088, 0, string); 
	  if(err) {
	    printf("Mysterious undoc command 0x0088 failed.  err=%d\n",err);
	    continue;
	  } 
	 
	  // send the initialization commands...
	  { 
	    
	    u_int16_t	sActData	[NUM_ACTUATORS]  = {0x0000};	// unmapped actuator data for sending to the DM
	    u_int16_t	sMapData	[NUM_ACTUATORS]  = {0x0000};	// mapped actuator data for sending to the DM
	    int counter, val, n, nloop,nloop2;
	    int retval;
	    
	    for(nloop=0; nloop<10; nloop++) {
	    
	    // PISTON
	    printf("Testing PISTON...\n");
	    
	    counter = 0;
	    for(nloop2=0; nloop2<10; nloop2++) {
	      for(val=0; val<(1<<14); val+=16 ) {
		for(n=0; n<NUM_ACTUATORS ;n++) // n.b. this loops over 160 "actuators"
		      sActData[n] = val;
		
		counter++;
		// endpoint = 2 is the only value.
		retval = usb_bulk_write(udev, 2, (const char *) sActData, NUM_ACTUATORS*sizeof(u_int16_t), TIMEOUT);
		if(retval!=USB_BYTES_PER_FRAME_MULTI) {
		  printf("\nFAILED: Actuator bulk write failed for val=%d.  bytes written=%d <%s>\n\n",val,retval,usb_strerror());
		  break;
		} 
	      }
	    }
	    printf("Made %d block actuator writes.\n",counter);
	    
	    // POKES
	      printf("Testing Mapped POKES\n");
	      for(n=0;n<NUM_ACTUATORS;n++) {
		clearBuffer(sActData,NUM_ACTUATORS);
		for(val=0; val<(1<<14); val+=50 ) {
		  sActData[n]=val;
		  retval = usb_bulk_write(udev, 2, (const char *) mapActs(sActData,sMapData,MAPPING), NUM_ACTUATORS*sizeof(u_int16_t), TIMEOUT);
		  if(retval!=USB_BYTES_PER_FRAME_MULTI) {
		    printf("\nFAILED: Actuator bulk write failed for val=%d.  bytes written=%d <%s>\n\n",val,retval,usb_strerror());
		    break;
		  } 
		}
	      }
	      
	      clearBuffer(sActData,NUM_ACTUATORS);
	      retval = usb_bulk_write(udev, 2, (const char *) mapActs(sActData,sMapData,MAPPING), NUM_ACTUATORS*sizeof(u_int16_t), TIMEOUT);
	      if(retval!=USB_BYTES_PER_FRAME_MULTI) {
		printf("\nFAILED: Actuator bulk write failed for val=%d.  bytes written=%d <%s>\n\n",val,retval,usb_strerror());
		break;
	      } 
	    }
	  } // end of DM actuator test block.
	  
	  printf("Shutdown command sequence(?)\n");
	err = vcmd(udev, 0xF5, 0, 0x0008, 0, string); 
	if(err) {
	  printf("Mysterious undoc command 0x0008 failed.  err=%d\n",err);
	  continue;
	} 
	
	printf("Releasing the DM USB interface...\n");
	
	if (usb_release_interface(udev,0)<0) {
	  printf("Couldn't release interface.\n");
	  continue;
	}
	
	// Close out the DM...
	usb_close (udev);
      }
      
      printf(" \n\n [usb_close has been called.]\n\n");
      
    } // devices
  } // busses
  return 0;
}
