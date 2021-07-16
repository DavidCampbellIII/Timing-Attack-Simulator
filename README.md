# Timing Attack Simulator

[![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://github.com/DavidCampbellIII/Timing-Attack-Simulator/blob/master/LICENSE.md)

![password cracked](https://user-images.githubusercontent.com/18089889/125908090-dc90b8dd-7bf2-4f2f-912f-4a1ce7886401.PNG)

## An Interactive Experiment in Cracking Passwords Using a Timing Attack

I built this as a fun little experiment in cracking passwords using nothing but the infamous timing attack.

---

## How Does It Work?

Comparing strings takes time.  By measuring the amount of time it takes for the system to respond (measuring in nanoseconds!), we can determine if a guess is partially correct.  The longer it takes for the system to respond, the more letters that we have guessed correctly.

Do this a few thousand times, and we can crack nearly any password!

Learn more [here](https://en.wikipedia.org/wiki/Timing_attack "Timing Attack Wikipedia")

---

## How To Run It

1. Download all source files above
2. Configure the `const` variables in `int main()` as desired
3. Compile and run Source.cpp
4. Add users with their passwords
5. Given only their username, have the system crack their password!

---

Inspired by [mCoding's video](https://www.youtube.com/watch?v=XThL0LP3RjY&t=231s "mCoding Timing Attack Video") on the subject.  Thought it'd be fun to see how fast I could get it working in C++ 😉
