# Fix8: An 8-bit ISA with Assembler and Emulator

Fix8 is a simple 8-bit Instruction Set Architecture (ISA) implementation, featuring an assembler and emulator. It allows writing, assembling, and emulating programs using the Fix8 ISA.

## Features

- **Custom ISA**: A fixed-length 8-bit ISA with 16 instructions.
- **Assembler**: Converts Fix8 assembly code into machine code.
- **Emulator**: Executes the machine code with debugging output.


## Building the Project

1. Clone the repository:

    ```bash
    git clone https://github.com/DarkPhoenix42/Fix8.git
    ```

2. Navigate to the project directory:

    ```bash
    cd Fix8
    ```

3. Build the project:

    ```bash
    make
    ```

## Running the Assembler and Emulator

1. Place assembly files (`.asm`) in the `asm/` directory.

2. Run the program:

    ```bash
    ./build/Fix8
    ```

3. Follow the menu to assemble and emulate programs.

## Project Structure

- `include/`: Header files.
- `src/`: Source code.
- `asm/`: Assembly files.
- `programs/`: Compiled machine code.
- `docs/`: Documentation.
- `ISA definition/`: Definition of the ISA.
