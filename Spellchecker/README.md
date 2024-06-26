# Multithreaded Spellchecker

## Main technologies used

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

## Introduction

Welcome to the Multithreaded Spellchecker project, developed as part of the CIS 3110 Operating Systems course and received a grade of 100/100. This project demonstrates the practical use of threads and thread synchronization in a real-world application scenario. The core functionality revolves around a spell-checking service that performs computationally intensive tasks, leveraging multithreading to maintain responsiveness and efficiency. The program uses detached threads and is free of deadlocks / livelocks and race conditions known.

## Overview

The program features a text-based user interface (UI) that runs on the main thread. Through this UI, users can initiate a spell-checking task which is computationally intensive and potentially long-running. These tasks are managed by spawning separate worker threads, allowing the main menu interface to remain responsive and interactive during task execution.

## Key Features

- **Multithreading:** Utilizes multiple threads to handle intensive computational tasks separately from the main UI thread.
- **Thread Synchronization:** Implements synchronization mechanisms to ensure that results from each worker task are safely written to a file without data corruption or loss.
- **Responsive UI:** Maintains a responsive text-based menu that allows the user to start new tasks or perform other interactions while tasks are running in the background.

## Objective

This project aims to provide hands-on experience with common concurrency patterns used in many software applications today. It allows for a deeper understanding of how independent worker tasks can be managed and synchronized effectively in a concurrent programming environment.

## Getting Started

### Prerequisites

- CIS 3110 Docker Image, as a University of Guelph student, you can obtain it from here .

### Installation

1. Clone the repo:
   ```bash
   git clone https://github.com/pharpala/Operating-Systems-Projects/tree/main/Spellchecker

2. Navigate to repository
   ```bash
   cd Spellchecker

3. Run the program
   ```bash
   ./A2checker

4. Follow the instructions from the GUI
   **

## Reminder

![Made with Love](http://ForTheBadge.com/images/badges/built-with-love.svg)
