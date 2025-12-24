# Vector & Volume Calculator
# Author : Dror Elazar Vamos 

A modular C program for performing vector operations and calculating 3D geometric volumes with automated CSV-based testing.

## Features

- **Vector Operations**
  - Scalar (dot) product: `V1 · V2`
  - Cross product: `V1 × V2`
  
- **Volume Calculations**
  - Parallelepiped: `|V1 · (V2 × V3)|`
  - Pyramid: `|V1 · (V2 × V3)| / 6`

- **Interactive UI**
  - Menu-driven interface
  - Single calculation mode
  - Automated test suite with CSV files

- **Robust Testing**
  - CSV test case validation
  - Tolerance-based comparison (0.1%)
  - Comprehensive error handling

## Quick Start

### Compilation

```bash
gcc -o calculator main.c testerFile.c mathUtil.c csvHandler.c -lm
```

### Usage

```bash
./calculator
```

## Project Structure

```
.
├── main.c              # Interactive UI and menu system
├── testerFile.c        # Automated testing framework
├── testerFile.h        # Test function interfaces
├── mathUtil.c          # Vector mathematics core
├── mathUtil.h          # Vector structures and declarations
├── csvHandler.c        # CSV parsing implementation
├── csvHandler.h        # CSV handler interface
└── comprehensive_test_cases.csv  # Test data
```

## CSV Test File Format

Each test case contains 13 comma-separated values:

```csv
V1_X,V1_Y,V1_Z,V1_MAG,V2_X,V2_Y,V2_Z,V2_MAG,V3_X,V3_Y,V3_Z,V3_MAG,EXPECTED_VOLUME
1,0,0,1,0,1,0,1,0,0,1,1,1.000
2,0,0,2,0,3,0,3,0,0,4,4,24.000
```

- **V1, V2, V3**: Three vectors (X, Y, Z components + magnitude)
- **EXPECTED_VOLUME**: Expected parallelepiped volume

## Main Menu Options

1. **Run Automated Test Suite** - Execute CSV test cases
2. **Single Calculation Mode** - Interactive vector calculations
3. **About** - Program information
4. **Exit** - Close program

## Requirements

- GCC compiler
- C99 standard or later
- Math library (`-lm` flag)

## License

Free for everyone to use!