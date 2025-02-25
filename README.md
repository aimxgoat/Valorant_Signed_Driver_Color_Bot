In this article, we will analyze and bypass the auth (authentication) system made by the signed driver of a popular valorant color aimbot sold globally. (HVCI, TPM, Secure Boot ON)

1. We unpack the virtualized exe and driver file and open it with IDA Pro to read it better. (I will not explain the unpacking steps)

2. First, we open the driver file with IDA Pro. In order to connect with the driver, we first need to find the DosName. We take a look at DriverEntry and see that the DosName is " JustWokeUp
". We saved this for later use.
