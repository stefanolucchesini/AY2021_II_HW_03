# Electronic Technologies and Biosensors Laboratory
## Academic Year 2020/2021 - II Semester

## Assignment 03
This repository contains the PSoC Creator workspace to be used as a starting point for the successfull submission of the Assignment 03
of the course "Electronic Technologies and Biosensors Laboratory" of the II Semester of the academic year 2020/2021.

### Objective
In this Project, you have to setup an I2C Slave using the PSoC and sample 2 analog sensors using a Delta-Sigma ADC. For data visualization, you will use the Bridge Control Panel to
plot the two signals.

### Connections

Pin          | Description |
:-----------:|-------------|
2.1          | Built-in LED
2.5          | Analog Channel 0: Temperature sensor
2.6          | Analog Channel 1: LDR (photoresistor) (*)
12.0         | I2C SCL
12.1         | I2C SDA

(*) See the connection scheme below to hook up the photoresistor (<a href="https://components101.com/asset/sites/default/files/component_datasheet/LDR%20Datasheet.pdf">Datasheet</a>)
<a href="https://ibb.co/Vm0pbTk"><img src="https://i.ibb.co/tXwPnD1/Cattura.jpg" alt="Photoresistor schematic" border="0"></a>

Please refer to the following image and to the datasheet of the component (<a href="https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf">TMP36GZ</a>) to hook up the temperature sensor

<a href="https://imgbb.com/"><img src="https://i.ibb.co/BcVt5BN/Untitled-Diagram.jpg" alt="Untitled-Diagram" border="0"></a>

### Requirements
The Project will have to meet the following requirements:
- Sample two analog sensors using a single 16-bit Delta-Sigma ADC (hint: you can use the <a href="https://www.cypress.com/file/127201/download">Analog Mux</a> for this)
- Each sample (of each channel) has to be the average of 5 consecutive samples buffered opportunely
- Setup an I2C Slave (<a href="https://www.cypress.com/file/185396/download">EZI2C</a>) to send the averaged data for the 2 channels to the Bridge Control Panel. 
  Configure the EZI2C component as follows: 100 kbps data rate, 1 address (0x08)
- The required transmission data rate is 50 Hz
- Set up a Timer with an ISR at the appropriate frequency to guarantee the data transmission rate (50 Hz) of the averaged data (5 samples)
- Control the operation of the device (START: 0xFF, STOP: 0x00) writing the value of the Control Register of the I2C slave
- Set up the I2C Slave buffer of the EZI2C according to the following table (set the WHO AM I to 0xBC)

I2C Slave Buffer Structure
Address      | Description   | R/W |
:-----------:|---------------|:---:|
0x00         | Control Reg   | R/W |
0x01         | Who Am I      |  R  |
0x02         | Ch0 Bit 15-8  |  R  |
0x03         | Ch0 Bit 07-0  |  R  |
0x04         | Ch1 Bit 15-8  |  R  |
0x05         | Ch1 Bit 07-0  |  R  |

### Setup and Assignment Delivery
- One student from the team forks this repository 
- Following the forking of the repository, the other team member(s) are added as collaborators via GitHub (as you did with the previous assignments)
- Each team member clones the forked repository by entering the following command in the terminal:

    `git clone https://github.com/[your_username]/AY2021_II_HW_03.git`
- Open up the workspace in PSoC Creator
- Activate the project with your group number
- Work on your project
- In your Group folder, save the Bridge Control Panel configuration files (GROUP_XX.ini and GROUP_XX.iic) inside BRIDGE_CONTROL_PANEL_CONFIG_FILES folder  
- The last commit must be named '*GROUP_XX* Final Delivery', where XX is the group number
- One member of the team proposes a pull request before the deadline for the assignment, which is on **April 23 at 6PM**


### Evaluation
The evaluation of the assignment will take into consideration the following aspects of the project:
- successful build process without any warning/error generated
- successful programming without any error generated
- correct functioning of the device as per the requirements
- code organization (header and source files, useful comments)
- schematic organization and documentation
- successful use of git for version control (use of branches, ...)
- successful collaboration with git across group members
- successful pull request on GitHub
