In this article, we will analyze and bypass the auth (authentication) system made by the signed driver of a popular valorant color aimbot sold globally. (HVCI, TPM, Secure Boot ON)

1. We unpack the virtualized exe and driver file and open it with IDA Pro to read it better. (I will not explain the unpacking steps)

2. First, we open the driver file with IDA Pro. In order to connect with the driver, we first need to find the DosName. We take a look at DriverEntry and see that the DosName is " JustWokeUp
". We saved this for later use.
![Wpns9hE](https://github.com/user-attachments/assets/3fcaf14c-1262-43b1-8f0f-66f1917dedc1)

3. Another thing we need is IOCTL calls. Similarly, we go from DriverEntry to the ioctl_handler function.
This IOCTL driver signed by Microsoft Windows Hardware Compatibility Publisher has 4 IOCTL calls in its handler function.
![64phfC5](https://github.com/user-attachments/assets/e1ccf218-369d-4e49-955b-d28f9c9eeb9b)


