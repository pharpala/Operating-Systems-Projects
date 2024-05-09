# Parallel Alphabet Histogram Generator

## Main technologies used

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
## Introduction

This project is part of the Operating Systems course and received a grade of 100 / 100. It involves writing a parallel program that analyzes a set of text files to produce alphabet histograms for each file. These histograms count the occurrences of each letter of the English alphabet within the files. The purpose of this project is to demonstrate the ability to manage multiple processes and utilize inter-process communication through pipes and signals.

## Project Description

The program accepts a list of file names as input and outputs a new set of files, each containing the English alphabet histogram corresponding to each input file. For instance, if an input file contains the text "hello world", the histogram would record the counts of each letter from 'a' to 'z', such as 'e' appearing once, 'h' once, 'l' three times, and so forth.

## Structure

The application architecture involves a parent process and multiple child processes. The parent process is responsible for reading the list of file names and distributing these files among the children processes. Each child process then calculates the histogram for its assigned files and communicates the results back to the parent process using pipes. The parent process collects all results and generates output files.

## Key Concepts Demonstrated

- **Process Management:** Utilizes multiple processes to handle separate tasks simultaneously.
- **Inter-process Communication:** Implements pipes and signals for communication between parent and child processes.
- **File Handling:** Reads from and writes to files based on the histogram analysis of file contents.
- **Signal Handling:** Uses signals for process control and management.

## Getting Started

### Prerequisites

- Ensure you have a Unix-like environment as the program uses Unix-specific features such as forks, pipes, and signals.
- C compiler (e.g., GCC) installed.
- CIS 3110 University of Guelph Docker Image.

### Installation

1. Clone the repo:
   ```bash
   git clone https://github.com/pharpala/Operating-Systems-Projects/tree/main/Histogram

2. Navigate to repository
   ```bash
   cd Histogram

3. Run the program
   ```bash
   ./main

4. Follow the instructions from the GUI
   **

## Reminder

![Made with Love](http://ForTheBadge.com/images/badges/built-with-love.svg)
