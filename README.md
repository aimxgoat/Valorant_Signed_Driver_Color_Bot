### probably detected

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

5. We quickly created a test application in C++ and sent the 0x617F862C mouse move package to the driver by imitating the classes .
![pe5mKvZ](https://github.com/user-attachments/assets/356b2365-e2f4-4308-a082-4c1ea407aac1)

Since it has an auth control, we cannot access this driver directly by sending the mouse move packet. If we do this, we get a warning like the one below and our mouse does not move.
![oRAiJoe](https://github.com/user-attachments/assets/619d17e0-c6e5-4751-9bf7-97c8eeb49e09)

In this case, we need to check the function that performs the auth check. ( sub_140001DA4 )
When checking this package in the driver, we see that it is heavily virtualized, it is unnecessary to try to analyze it at this stage.

6. We open the exe file with IDA Pro and analyze the data sent with the IOCTL auth call.
![BjAhN7f](https://github.com/user-attachments/assets/2b5ec532-9a8e-403c-b7dd-debc76bb3e55)

We see that the data uses several arguments, one of which is to get the computer name with the " ComputerNameW " api, and the other is to get the computer's system time with " GetSystemTimeAsFileTime
". This data is then sent to the driver by BCrypt with a lot of hashing and encryption, each time unique.
![BjAhN7f](https://github.com/user-attachments/assets/f899197d-54fa-4ce7-b4e2-10122100b427)
![pe5mKvZ](https://github.com/user-attachments/assets/c8f55e7b-96c1-45e9-a8e9-20b5edc5d94f)
![lXFvl3u](https://github.com/user-attachments/assets/8ddf1426-c418-4d4a-9012-f4bbea25759e)

Let's take a look at what we have.
The arguments received for the Auth Package.
1. Computer Name. (Fixed)
2. System Time. (Variable) (This prevents spoofing by sending a unique packet each time)
3. Windows Version. (Fixed) (21H2,22H2,23H2 etc.)

7. We open the Color aimbot and examine the data in the IOCTL packet sent for auth and see that it sends different data each time.
![2gSPRo8](https://github.com/user-attachments/assets/6594661f-50d3-4407-bbf2-dd8b83104d6a)

My computer name is " DESKTOP-5L3J5FV " and my Windows version is " Windows 10 22H2 Pro ". We set
our system time to 01.01.2024 12:00 and resend the packet several times.
Since we fixed the system time and date to 01.01.2024 12:00, the auth packet does not change anymore. We see that it sends the same packet every time.
Computer Name is " DESKTOP-5L3J5FV ", My Windows Version is " Windows 10 22H2 Pro ", System Time is " 01.01.2024 12:00 " and the packet sent is always " c8 21 d3 2f ".

8. We add the Auth call and the necessary classes to our C++ code and before sending the packet, we set our system time to 01.01.2024 12:00. We reverse the sent packet ( c8 21 d3 2f ) and send it to the driver with the IOCTL call ( 0x2fd321c8 ).
![weyx6Sq](https://github.com/user-attachments/assets/7e0a7e13-de26-48fe-baf9-6286ffeb7662)

Success. The driver's auth system accepted the data we sent. Now we can move the mouse.
![Z3SRHvD](https://github.com/user-attachments/assets/2fc59414-babe-48b5-bfe5-d569299ccbb4)

esult
If you change your Computer Name to " DESKTOP-5L3J5FV ", System time and date to " 01.01.2024 Time 12:00 " and have a " Windows 10 22H2 Pro " operating system, you can bypass the license verification by sending this " 0x2fd321c8 " fake package to auth and use this signed color aimbot driver.

What to do
1-Change the Computer name to " DESKTOP-5L3J5FV " and restart the computer. (The project below does this for you)
2-Change the System time and date to " 01.01.2024 Time 12:00 ". (The project below does this for you)
3.Drop the driver to C:\Windows\System32\drivers\ and run it. (salesto.sys)

Now you can install and use this driver. (It will work even if HVCI, TPM, Secure Boot are on)
sc create salesto type=kernel binpath="C:\Windows\System32\drivers\salesto.sys"
sc start salesto

The shared project does almost all the work for you (Computer name, System time), all you have to do is start the driver. Don't forget to send the package that is compatible with your windows version in the project.
driver.Fake_Auth_Data(0x2fd321c8); // Windows 10 Pro 22H2
driver.Fake_Auth_Data(0x484c1ace); // Windows 11 Pro 22H2
driver.Fake_Auth_Data(0x1e1a7376); // Windows 11 Pro or Home 23H2 (Not tested)

Note 1: If you send the wrong auth package to the driver once, it will not accept the correct packages you send later. You need to restart the driver. Or restart the computer.
Note 2: The information in this article is for educational purposes.



