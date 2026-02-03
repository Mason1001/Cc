# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Initial project structure with CMake build system
- Real-time FOV overlay rendering using DirectX 11
- YOLOv8 object detection integration framework
- Friend/enemy object classification system
- Sound tracking with Y-axis sensitivity configuration
- JSON-based configuration management (settings.json)
- Dear ImGui integration for runtime UI
- Transparent overlay window with topmost positioning
- 120 FPS target rendering optimization
- Comprehensive documentation (README, DEVELOPMENT, API)
- Build scripts for Windows and Linux
- Example configuration files

### Core Features
- **OverlayManager**: Window creation and event handling
- **FOVRenderer**: DirectX 11 rendering for circles, lines, and boxes
- **YOLODetector**: YOLOv8 model wrapper for object detection
- **ObjectTracker**: Multi-object tracking with IOU matching
- **SoundTracker**: Audio direction visualization
- **ConfigManager**: JSON configuration loading/saving
- **ImGuiManager**: In-overlay settings interface

### Documentation
- Build instructions for Visual Studio and CMake
- API documentation with usage examples
- Development guide with coding standards
- External dependency setup instructions
- YOLOv8 model download and configuration guide

### Configuration Options
- FOV customization (radius, color, thickness)
- Detection thresholds (confidence, IoU)
- Sound tracking sensitivity and ranges
- Overlay window settings
- UI preferences

## [1.0.0] - 2026-02-03

### Added
- Initial release
- Complete project architecture
- Framework for real-time overlay application
