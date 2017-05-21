# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Reflection

### The effect of P, I, and D in the PID Controller

- **P (Proportional)**: The proportional term of the PID controller attempts to account for the cross track error by turning the vehicle toward the desired path by an amount proportional to the cross track. This very quickly steers the vehicle toward the path, but when the vehicle approaches the desired path and the cross track error is small, the vehicle begin to drive straight even though the vehicle is not following the desired path. This process will result in an oscilation when attempting to drive in a straight line and will most likely cause the vehicle to drive off the road while attempting to navigate a corner.

- **I (Integral)**: The integral term of the PID controller attempts to minimise 'accumulated' cross track error. If the vehicle has spent time driving on one side of the desired path, this term of the controller will attempt to drive the vehicle on the other side of the desired path in order to negate the accumulated error. This could also result in an oscillation if it is the only term used in the controller. The Integral term can also be used to counter the effect of a steering bias that would cause the vehicle to follow the desired path, but offset from the path by some amount.

- **D (Derivative)**: The derivative term of the PID controller helps address changes in the cross track error. This term is proportional to the change in cross track error meaning that if the cross track error is increasing, it will steer the vehicle toward the desired path. If the cross track error is decreasing, it will steer the vehicle away from the desired path. This counter steering helps steer the vehicle onto the desired path when the vehicle is approaching it from one side or the other. This term helps address the oscillations that can be introduced by the proportional and integral terms allowing the vehicle to drive more consistently along the desired path.

### The Hyperparameter Search

I started by playing with the `kp`, `ki`, and `kd` parameters to confirm that the controller was behaving as I expected. This also allowed me to get a rough sense of the impact of each parameter. Printing the error value for each term also allowed me to get sense of the rough order of magnitude that each parameter should have. I would have liked to implement the Twiddle algorithm and tune the parameters algorithmically, but i couldn't figure out an easy way to restart the simulator automatically after each run of Twiddle.

Instead, I opted for a manual approach. I knew that this wouldn't result in optimal values for the parameters but felt like the process was good enough for the spirit of this project. Based on what I had already found playing with the parameters I decided the following process would be suitable.

- Start with `kp = 1`, `ki = 0`, and `kd = 0` and observe the behaviour.
- Tune `kp` until the vehicle was able to reach the first corner.
- Add `kd = 1` and observe the behaviour.
- Tune `kd` until the vehicle was able to complete a lap of the track (safely or not).
- Add `ki = 0.1` and observe the behaviour. I knew initialising `ki` to 1 would be a mistake as the integral error term was so much larger than the other terms.
- Tune `ki` until the vehicle was able to complete a lap of the track as safely as possible.

Based on my observations of the three error values, I believed `kp` should be less than 1 but not too much less, `ki` should be quite a bit less than 1, and `kd` should be quite a bit larger than 1. These observations influenced the direction of the search for each value.

The parameter values were chosen first by order of magnitude and then by trying values that were between the orders of magnitude on a log scale. For example, if the vehicle performed ok with a parameter value of 1, but worse with a value of 10 the the next value would be 3.16 (half way between 1 and 10 on the log scale). This process is a little arbitrary, but it felt more reasonable than halving the difference between 1 and 10 as the impact of a value of 10 is more than twice that of 1.

These were my parameter choices and observations of the vehicles behaviour (the bolded parameter is the one modified for the given iteration):

- **kp: 1**, ki: 0, kd: 0: The oscillations grew too great to continue along the initial straight road.
- **kp: 0.1**, ki: 0, kd: 0: Ok along the straight but too slow to react to the first corner.
- kp: 0.1, ki: 0, **kd: 1**: Made it around the track, but not in a way that would be deemed safe.
- kp: 0.1, ki: 0, **kd: 10**: Made it around the track more safely, but steering correction looked a bit jittery.
- kp: 0.1, ki: 0, **kd: 100**: Made it around the track, but the steering seemed off almost introducing a bias that the controller couldn't compensate for.
- kp: 0.1, ki: 0, **kd: 31.6**: Better than the previous 2 iterations, but not good enough.
- **kp: 0.316**, ki: 0, kd: 31.6: Made it around the track safely!
- kp: 0.316, **ki: 0.1**, kd: 31.6: Off the track almost immediately.
- kp: 0.316, **ki: 0.01**, kd: 31.6: Made it around the track safely! Integral error is much smaller and the car appears to remain closer to the center of the track.
- kp: 0.316, **ki: 0.001**, kd: 31.6: A bit less central but a bit less jittery. I would say this is a worse performance than the previous iteration.
- kp: 0.316, **ki: 0.00316**, kd: 31.6: A bit better than the pervious iteration, but a bit too close the edges in some places.

Based on this process, I settled on final parameter values of `kp: 0.316, ki: 0.01, kd: 31.6`.

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets) == 0.13, but the master branch will probably work just fine
  * Follow the instructions in the [uWebSockets README](https://github.com/uWebSockets/uWebSockets/blob/master/README.md) to get setup for your platform. You can download the zip of the appropriate version from the [releases page](https://github.com/uWebSockets/uWebSockets/releases). Here's a link to the [v0.13 zip](https://github.com/uWebSockets/uWebSockets/archive/v0.13.0.zip).
  * If you run OSX and have homebrew installed you can just run the ./install-mac.sh script to install this
* Simulator. You can download these from the [project intro page](https://github.com/udacity/CarND-PID-Control-Project/releases) in the classroom.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`.

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./
