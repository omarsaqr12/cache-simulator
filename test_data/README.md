# Test Data Files

This directory contains test data files for the cache simulator.

## File Format

Each test file contains memory access patterns in the following format:

```
<32-bit binary address><access_type>
```

Where:

- `<32-bit binary address>`: A 32-bit binary representation of the memory address
- `<access_type>`: Either 'I' for instruction access or 'D' for data access

## Test Files

### test1.txt

- **Purpose**: Basic sequential access pattern
- **Description**: Simple sequential memory accesses for both instructions and data
- **Pattern**: Sequential addresses starting from 0

### test2.txt

- **Purpose**: Mixed access pattern with gaps
- **Description**: Non-sequential access pattern with various instruction and data accesses
- **Pattern**: Mixed addresses with different bit patterns

### test3.txt

- **Purpose**: Power-of-2 address pattern
- **Description**: Tests cache behavior with addresses that are powers of 2
- **Pattern**: Addresses follow power-of-2 progression (1, 2, 4, 8, 16, etc.)

## Usage

These test files can be used with the cache simulator to test different cache configurations and observe cache behavior patterns such as:

- Hit/miss ratios
- Cache line replacement
- Instruction vs data cache performance
- Address mapping effectiveness
