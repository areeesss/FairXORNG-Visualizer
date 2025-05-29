# FairXORNG Visualizer

A visualization tool for the FairXORNG algorithm, featuring a React frontend and Node.js backend with native C++ bindings.

## Features

- Real-time visualization of the FairXORNG algorithm
- Interactive frontend built with React and TypeScript
- Native C++ implementation for high-performance random number generation
- WebSocket-based real-time communication
- Modern UI with Tailwind CSS

## Prerequisites

Before installing, ensure you have the following installed on your system:

- **Node.js** (v16 or later recommended)
- **npm** (comes with Node.js)
- **Python** (required for node-gyp)
- **C++ Build Tools**:
  - Windows: Visual Studio Build Tools with C++ workload
  - Linux: `build-essential` package (`sudo apt-get install build-essential`)
  - macOS: Xcode Command Line Tools (`xcode-select --install`)

## Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/FairXORNG-Mk2.git
   cd FairXORNG-Mk2
   ```

2. **Install all dependencies**
   ```bash
   npm run install:all
   ```
   This command will:
   - Install backend dependencies
   - Install frontend dependencies
   - Build the native C++ module

3. **Build the native module** (if needed)
   If the automatic build during installation fails, you can manually rebuild:
   ```bash
   npm run rebuild
   ```

## Running the Application

### Development Mode

To run the application in development mode (both frontend and backend):

```bash
npm start
```

This will start:
- Backend server (default port: likely 3000 or 5000)
- Frontend development server (default port: 3000)

### Production Build

To create a production build:

1. Build the frontend:
   ```bash
   npm run build
   ```

2. The built files will be in the `frontend/build` directory

## Project Structure

```
FairXORNG-Mk2/
├── backend/           # Node.js backend server
├── frontend/          # React frontend application
├── FairXORNG.cpp      # C++ implementation
├── FairXORNG.h        # C++ header
├── binding.cpp        # Node.js native bindings
├── binding.gyp        # Node-gyp build configuration
└── package.json       # Project dependencies and scripts
```

## Troubleshooting

### Common Issues

1. **Node-gyp Build Fails**
   - Ensure you have the correct C++ build tools installed
   - On Windows, run as administrator
   - Try clearing the build cache: `npm run rebuild`

2. **Frontend Dependencies Issues**
   - Clear npm cache: `npm cache clean --force`
   - Delete `node_modules` folder and `package-lock.json` in the frontend directory
   - Run `npm install` again in the frontend directory

3. **Backend Server Issues**
   - Check if the port is already in use
   - Ensure all environment variables are set correctly
   - Check the backend logs for specific errors

### Getting Help

If you encounter any issues not covered here:
1. Check the existing GitHub issues
2. Create a new issue with:
   - Your operating system
   - Node.js version
   - Steps to reproduce the problem
   - Any error messages

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

[Add your license information here]

## Authors

[Add author information here] 