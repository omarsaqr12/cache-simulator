# Cache Simulator

A comprehensive cache memory simulator implemented in C++ with a Python GUI interface. This project simulates the behavior of instruction and data caches, providing detailed statistics and performance metrics for different cache configurations.

## 🚀 Features

- **Dual Cache Simulation**: Separate instruction and data cache simulation
- **Configurable Parameters**: Customizable cache size, block size, and access latency
- **Direct Mapped Cache**: Implements direct-mapped cache organization
- **Performance Metrics**: Detailed hit/miss ratios, AMAT (Average Memory Access Time) calculations
- **Multiple Interfaces**: Command-line interface and modern GUI
- **Test Data Support**: Includes various test patterns for comprehensive testing
- **Real-time Statistics**: Live cache status updates during simulation

## 📁 Project Structure

```
cache-simulator/
├── src/                    # Source code files
│   ├── main.cpp           # Main command-line interface
│   ├── cache_logic.cpp    # Alternative cache logic implementation
│   └── gui.py             # Python GUI interface
├── include/               # Header files
│   └── cache.h           # Cache class definition and implementation
├── test_data/            # Test input files
│   ├── test1.txt         # Basic sequential access pattern
│   ├── test2.txt         # Mixed access pattern
│   ├── test3.txt         # Power-of-2 address pattern
│   └── README.md         # Test data documentation
├── docs/                 # Documentation
│   └── project_report.pdf # Detailed project report
├── Makefile              # Build configuration
├── .gitignore           # Git ignore rules
└── README.md            # This file
```

## 🛠️ Requirements

### For C++ Components

- **Compiler**: GCC 7.0+ or equivalent with C++17 support
- **Operating System**: Windows, Linux, or macOS
- **Memory**: Minimal requirements

### For Python GUI

- **Python**: 3.7 or higher
- **Dependencies**:
  - `ttkbootstrap` (for modern GUI components)
  - `tkinter` (usually included with Python)

## 📦 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/cache-simulator.git
cd cache-simulator
```

### 2. Build the C++ Components

```bash
# Build both executables
make all

# Or build individually
make cache_simulator    # Main simulator
make cache_logic       # Alternative implementation
```

### 3. Install Python Dependencies (for GUI)

```bash
make install-deps
# or manually:
pip install ttkbootstrap
```

## 🎮 Usage

### Command Line Interface

#### Basic Usage

```bash
./cache_simulator
```

The program will prompt you for:

- Instruction cache size (bytes)
- Data cache size (bytes)
- Cache block size (bytes)
- Cache access latency (cycles, 1-10)
- Access sequence filename

#### Example Session

```
Enter instruction cache size (in bytes): 4096
Enter data cache size (in bytes): 4096
Enter cache block size (in bytes): 64
Enter cache access latency (in cycles): 2
Enter access sequence file name: test_data/test1.txt
```

### GUI Interface

Launch the modern GUI interface:

```bash
make gui
# or directly:
python src/gui.py
```

The GUI provides:

- Input forms for all cache parameters
- File browser for test data selection
- Real-time output display
- Modern, responsive interface

### Automated Testing

Run tests with all provided test data:

```bash
make test
```

## 📊 Input Format

Test data files should contain memory access patterns in the following format:

```
<32-bit binary address><access_type>
```

Where:

- `<32-bit binary address>`: Binary representation of memory address (e.g., `00000000000000000000000000000001`)
- `<access_type>`: Either `I` (instruction) or `D` (data)

### Example:

```
00000000000000000000000000000000I
00000000000000000000000000000001I
00000000000000000000000000000100D
00000000000000000000000000000101D
```

## 📈 Output Metrics

The simulator provides comprehensive statistics:

### Cache Status

- Active cache blocks and their tags
- Cache configuration details
- Memory access patterns

### Performance Metrics

- **Total Requests**: Number of memory access requests
- **Successful Hits**: Number of cache hits
- **Misses**: Number of cache misses
- **Hit Rate**: Percentage of successful cache hits
- **Miss Rate**: Percentage of cache misses
- **AMAT**: Average Memory Access Time in cycles

### Example Output

```
Cache Details:
Cache Size: 4096 bytes
Block Size: 64 bytes
Total Blocks: 64
Access Time per Block: 2 cycles
Memory Latency: 100 cycles
Total Requests: 10
Successful Hits: 6
Misses: 4
Hit Rate: 60.00
Miss Rate: 40.00
Average Memory Access Time: 42.00 cycles
```

## 🏗️ Architecture

### Cache Implementation

- **Type**: Direct-mapped cache
- **Replacement Policy**: Not applicable (direct-mapped)
- **Write Policy**: Write-through (implied)
- **Address Mapping**: Uses index and tag bits for address resolution

### Key Components

1. **Cache Class**: Core cache simulation logic
2. **Address Parser**: Extracts tag, index, and offset from addresses
3. **Statistics Engine**: Tracks and calculates performance metrics
4. **File Parser**: Handles test data input processing

## 🧪 Testing

The project includes three test scenarios:

1. **test1.txt**: Sequential access pattern for basic functionality testing
2. **test2.txt**: Mixed access pattern for realistic workload simulation
3. **test3.txt**: Power-of-2 addresses for cache conflict analysis

Run individual tests:

```bash
echo -e "4096\n4096\n64\n2\ntest_data/test1.txt" | ./cache_simulator
```

## 🔧 Development

### Building from Source

```bash
# Debug build
g++ -std=c++17 -Wall -Wextra -g -Iinclude -o cache_simulator src/main.cpp

# Release build
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -o cache_simulator src/main.cpp
```

### Code Structure

- **Modular Design**: Separate header and source files
- **Object-Oriented**: Cache implemented as a C++ class
- **Cross-Platform**: Compatible with major operating systems
- **Modern C++**: Uses C++17 features for improved performance

## 📄 License

This project is part of an academic assignment for Computer Architecture/Assembly Language coursework. Please respect academic integrity policies when using this code.

## 🤝 Contributing

This is an educational project. For suggestions or improvements:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## 📞 Support

For questions or issues:

- Check the documentation in the `docs/` folder
- Review the test data examples
- Examine the source code comments

## 🏆 Acknowledgments

- Developed as part of Computer Architecture coursework
- Implements fundamental cache memory concepts
- GUI built with modern Python frameworks

---
