In this article, we will analyze and bypass the auth (authentication) system made by the signed driver of a popular valorant color aimbot sold globally. (HVCI, TPM, Secure Boot ON)

1. We unpack the virtualized exe and driver file and open it with IDA Pro to read it better. (I will not explain the unpacking steps)

2. First, we open the driver file with IDA Pro. In order to connect with the driver, we first need to find the DosName. We take a look at DriverEntry and see that the DosName is " JustWokeUp
". We saved this for later use.
![Wpns9hE](https://github.com/user-attachments/assets/3fcaf14c-1262-43b1-8f0f-66f1917dedc1)

3. Another thing we need is IOCTL calls. Similarly, we go from DriverEntry to the ioctl_handler function.
This IOCTL driver signed by Microsoft Windows Hardware Compatibility Publisher has 4 IOCTL calls in its handler function.
![64phfC5](https://github.com/user-attachments/assets/e1ccf218-369d-4e49-955b-d28f9c9eeb9b)

After a short check, we see that the first of these, the 0x8F67D7B0 call, performs the auth operation and, if successful, sets the byte_140005140 variable to true, allowing mouse move and other packages. We see
that the package that performs the mouse move operation is the 0x617F862C call.
![aTUMcnp](https://github.com/user-attachments/assets/aa3724ba-8d0c-4ddb-bfb2-2968c40b2b96)

Let's take a look at what we have.
DosName: " JustWokeUp "
IOCTL address that checks auth: 0x8F67D7B0
IOCTL address that performs mouse move if auth is successful: 0x617F862C

4. We load the swarm for testing with the following commands.
sc create salesto type=kernel binpath="C:\Windows\System32\drivers\salesto.sys"
sc start salesto
![KyRfBdj](https://github.com/user-attachments/assets/6cf17c5a-8e76-4e3e-9089-44250ffe9453)
