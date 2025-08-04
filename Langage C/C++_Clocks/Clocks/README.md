# README

# 🌟 Introduction

Welcome to **ADHD** — *Analog to Digital ~~Time~~ Hour Display* — a project that means a lot to us.

We're proud to be the second generation of students to take on this challenge, building upon the amazing work started by our fellow engineers from the **MAIN** (*Mathematics and Computer Science*) specialty. They laid the groundwork using Python (you should absolutely check out their awesome work [here](https://github.com/Halexeli/analog_to_digital) 🔥).

Our mission? To reimagine and reimplement the same vision — but in **C++**, pushing the boundaries even further.

At its core, this project is about transforming a set of **analog clocks** into a **digital display** by precisely controlling their hands. It’s a beautiful fusion of hardware, software, and creativity — and we couldn’t be more excited to share it with you.

# 🧠 Project Details

If you're not here for the nitty-gritty, feel free to skip ahead.

But if you're curious and ready to dive in — fair warning — it's about to get delightfully geeky.

## 🎯 The Vision

So, the ultimate goal for this project is as we said to create an actual **PHYSICAL** system that displays time digitally using analog clocks. More specifically here is our vision:

Our end goal is to build a **real, physical system** that displays time in a digital format... using **analog clocks**. It’s a project that sits at the intersection of mechanics, electronics, software, and design — and we’ve envisioned every detail.

Here’s what the full system should look like:

- **🧱 3D-printed rectangular pieces**
    
    These act as the clock hands. Their size may vary based on the torque needed to move them smoothly (more on that in a bit).
    
- **🔁 Dual-shaft motors**
    
    These rotate the two hands (inner and outer) of each clock — one motor per clock.
    
- **🧠 Microcontroller boards**
    
    Each controls one or more motors.
    
- **🕹️ A central development board**
    
    This one acts as the master brain, communicating with the microcontrollers.
    
- **🧩 Custom PCBs**
    
    To integrate all electronics and connections neatly.
    
- **🪵 A handcrafted wooden board**
    
    Serves as the elegant foundation of the display — carved at the back to embed electronics and engraved at the front to frame each clock. And yes, it will be painted and varnished to make it as aesthetic as it is functional 😌.
    

For instance, here is what we were aiming to use:

- **Rectangular 3D printed pieces**:
    - Number: 48
    - Color: White (#FFFFFF)
    - Material:  ****Polylactic Acid (PLA)
    - Dimensions: 4 x 1 x .1 cm (1.58 x .4 x .04 in for our freedom language speakers)
- **Dual shaft motors**:
    - Number: 24
    - Reference: VID28
    - Datasheet: https://cdck-file-uploads-europe1.s3.dualstack.eu-west-1.amazonaws.com/arduino/original/3X/b/0/b0aa1434329fd55ba59f10d853612d71be1a5b07.pdf
- **Microcontroller boards**:
    - Number: 10
    - Reference: Raspberry Pi Pico
    - Datasheet: https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html
- **Microcontroller Development Board**:
    - Number: 1
    - Reference: Raspberry Pi 0 W
    - Datasheet: https://datasheets.raspberrypi.com/rpizero2/raspberry-pi-zero-2-w-product-brief.pdf
- **Wooden board**:
    - Number: 1
    - Color: Wood (#A1662F)
    - Material: Wood
    - Dimensions: 90 x 40 x 7 cm (35.4 x 15.7 x 2.75)

## 🧪 From Idea to Reality

As you might have noticed this list of material is designed for a 8 x 3 clocks display (which is actually the minimum allowed by logic) but it is obviously scalable (to double the size for example 16 x 6 clock display).

To make this dream system come true, we split the implementation into two main parts:

### 🖥️ 1. Simulation

Before wiring and soldering anything, we decided to simulate everything — and this step became the heart of our project.

Here’s how we organized it:

🎯 **Supervisor**

Acts as the "brain" of the system. It sends the target positions and timing data for each clock to the simulator using **TCP sockets**.

🛠️ **Receptor**

This simulates the **actual hardware setup**:

- Displays a grid of 24 analog clocks (using the **SFML** graphics library).
- Mimics the behavior of the motors.
- Translates incoming data into realistic motion using proper stepping logic and transitions.

With this design, we could test everything: timing, transitions, and the system’s responsiveness.

And so, using TCP sockets, SFML graphic library, and many more interesting subtilities, we succeeded to make this simulation. Follow along to know how it works.

### 🔧 2. Physical System

Once we validated our simulation and had confidence in its accuracy, the second phase began: preparing the **real hardware**.

Here’s the twist:

Instead of using **TCP sockets**, we’ll switch to **I²C communication** to relay movement commands from the central board to the individual microcontroller boards — each commanding one or more motors.

Now that you have a small idea about what our project is about, tag along to see what we were able to make so far.

## 💻Coding!

Ready to get your hands dirty with the code? Let’s dive in! 🔧✨

### 📦 Step 1: Clone the Repository

Start by cloning this repository to your local machine using:

```bash
git clone https://github.com/Midosamaa/Clocks
```

This command will download a copy of the project into a folder named clocks.

### 🧭 Step 2: Navigate into the Project

Once cloned, head into the project directory:

```bash
cd Clocks
```

### 🌲Step 3: Check the Project Structure

To make sure everything is in place, run:

```bash

tree
```

You should see something like this:

```bash
.
├── elec
│   ├── led_control.ino
│   ├── serial.cpp
│   └── simple
│       └── simple.ino
└── simulation
    ├── receptor
    │   ├── communication.cpp
    │   ├── communication.h
    │   ├── main.cpp
    │   └── makefile
    └── supervisor_with_window
        ├── features
        │   ├── Clock.cpp
        │   ├── Clock.h
        │   ├── Hand.cpp
        │   └── Hand.h
        ├── main.cpp
        ├── makefile
        ├── transitions
        │   ├── DigitConfiguration.cpp
        │   ├── DigitConfiguration.h
        │   ├── Transition.cpp
        │   └── Transition.h
        └── type_transitions
            ├── type_transitions.cpp
            └── type_transitions.h
```

If you have more files do not worry you are going to know soon why. But the essential is to have the files/folders above.

**🗂️ Project Structure**

If that is the case allow me to enlighten you on what does every folder you see contain.

- elec: This folder contains everything related to the **physical system**, including code for microcontrollers, wiring, and motor control.
    - led_control.ino: This is an Arduino sketch uploaded to the **Raspberry Pi Pico** using the Arduino IDE. It was used to test **serial communication** with a PC and control the onboard LED.
    - serial.cpp: A **PC-side C++ program** used to test and handle **serial communication** with a microcontroller or embedded board.
    - simple
        - simple.ino: A **basic motor control test** using an **Arduino Uno**. It was the first example we used to make the motor rotate.
- simulation: This folder contains all the code for the **simulated version of the system**. It’s organized into two parts: the simulator (receiver) and the supervisor interface.
    - receptor: This folder handles **receiving data** from the supervisor via TCP and simulating the clocks accordingly.
        - communication.cpp / communication.h: These files manage **TCP socket communication**, allowing the receptor to receive angle and timing data from the supervisor.
        - main.cpp: The **entry point** of the receptor. It launches the TCP server and listens for incoming data to simulate clock behavior accordingly.
        - Makefile: Used to compile the receptor code and manage dependencies.
    - supervisor_with_window: This folder represents the **simulator with graphical output**, built using the **SFML library**. It is responsible for rendering the clocks and animating the transitions.
        - features: Contains the core building blocks (classes and components) of the clock system.
            - Clock.cpp / Clock.h: The **structure and logic** for a full analog clock unit, including its drawing and positioning.
            - Hand.cpp / Hand.h: Similar to clock, these files manage the **individual clock hands** (hour/minute/second) and their behavior.
        - main.cpp: The **main simulation loop**, which launches the simulation window, loads all the clocks, and processes updates based on received or predefined inputs.
        - makefile: Build configuration for compiling the simulator with SFML.
        - transitions: Manages the **logic of moving clock hands smoothly** from one position to another, simulating real mechanical constraints.
            - DigitConfiguration.cpp / DigitConfiguration.h: Define **visual configurations** for characters (letters, numbers, symbols) using a matrix of analog clocks in a 3x8 layout. Each character is defined by the angles of hands.
            - Transition.cpp / Transition.h: Contain the **general mechanics** for computing how to **animate transitions** from one position to another.
        - type_transitions: Implements **various kinds of transitions**, each with its own visual and temporal style.
            - type_transitions.cpp / type_transitions.h:
                - Stars 🌟
                - Waves 🌊
                - Words sliding 📝
                - Pac-Man style 🟡
        

Now that everything is set. We can get to the fun part! Using our simulator.

### 🏞️Step 4: Prepare evironment

But before you get too excited get back to your seat and make sure you have all the necessary libraries on your machine.

Run the command 

```bash
sudo apt-get install libsfml-dev
```

This will download the SFML library that we used for this project. And if you are more interested in this library take a peek at their website at https://www.sfml-dev.org/tutorials/3.0/getting-started/linux/.

Well now that your machine is ready, let us get started.

### 🏃🏻Step 5: Running the simulation

First, go to the directory in which you cloned the repository, and access the supervisor_with_window folder by running the command

```bash
cd simulation/supervisor_with_window
```

Then just in case one of our ✨ brilliant ✨ members pushed before cleaning. Run the command

```bash
make clean
```

And then build the executable by running the command

```bash
make
```

After waiting for a couple of seconds for the compilation to finish you can finally run the simulation.

To do so there is a couple of things you need to know. 

If you know anything about programming, to run the executable you need exactly three arguments besides the latter.

1. Direction: you have to enter the number coresponding to one of these directions
    
    1 : Right to Left
    2 : Left to Right
    3 : Bottom to Top
    4 : Top to Bottom
    
2. Transition type: you have to enter the name of a transition. These transitions are the ones that we implemented in the type_transitions files (keep an ear out, there might be updates in the future).
    - 'pacman': Pac-Man animation eating the old time
    - 'wave'
    - 'stars'
    - 'words'
3. Transition details: for this field you need to fill the details depending on the transition you chose in the second fields.
    - 'pacman'  : transitionDetail = NULL) and Direction is either vertical (2) or horizontal (1)
    - 'wave'    : Wave animation (transitionDetail = number of waves)
    - 'stars'   : Star animation (transitionDetail = number of stars)
    - 'words'   : Temporary word display before showing time (transitionDetail = word to display)

Concretely, here is what your command should look like

```bash
./clock_display 2 pacman NULL
```

or

```bash
./clock_display 4 words "IT IS TIME"
```

or

```bash
./clocks_display 3 stars 4
```

or 

```bash
./clocks_display 1 wave 2
```

And if in doubt execute it however you feel and it will tell you where you got it wrong.

And well, that is basically everything we have to present.

A program that shows you the current time, and then when the time changes, you will see the transition you chose before showing the updated time.

# 👋 A Few Words to Wrap Things Up

As we reach the end of what we wanted to share with you about this project, let us take a moment to introduce ourselves.

We are three fourth-year **Electronics and Computer Science** students from **Polytech Sorbonne**, and this project has been a true labor of love. From the very beginning, we poured our energy, creativity, and curiosity into bringing this analog-digital vision to life. What started as an academic assignment quickly turned into something much more meaningful to us — something we genuinely cared about and enjoyed building.

While one of us is planning to continue the adventure, we hope that the story doesn’t end here. If you stumble upon this project and feel even a fraction of the excitement we did, we wholeheartedly invite you to pick up where we left off. You’ll find our contact information below — please don’t hesitate to reach out. Whether it’s a question, a collaboration, or just to say hi, we’d love to hear from you.

Remember: this is *not* the final chapter of the project. We're committed to making it as open, accessible, and easy to understand as possible. So if you run into any issues, or spot something that could be improved, let us know. Your feedback could help shape the next steps of this journey.

We also want to express our deepest gratitude to our supervisors, **Thibault Hilaire** and **Hugo Dorfsman**, for their support, guidance, and insight throughout this experience. Their encouragement helped transform our ideas into something real.

And finally — thank you.

Thank you for reading, for exploring, and for being curious.

Thank you for following this journey.

We hope it inspires you as much as it inspired us. 🌟

With all our appreciation,

**The ADHD Team** ❤️

- Ahmed FILALI  -[Ahmed.Filali_Darai@etu.sorbonne-universite.fr](mailto:Ahmed.Filali_Darai@etu.sorbonne-universite.fr)
- Victor LAM  -[Victor.Lam@etu.sorbonne-universite.fr](mailto:Victor.Lam@etu.sorbonne-universite.fr)
- Yedam KIM  -[Yedam.Kim@etu.sorbonne-universite.fr](mailto:Yedam.Kim@etu.sorbonne-universite.fr)