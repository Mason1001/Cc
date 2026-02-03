# Contributing to Gaming Overlay

Thank you for your interest in contributing to the Gaming Overlay project!

## How to Contribute

### Reporting Bugs

If you find a bug, please open an issue with:
- Clear description of the problem
- Steps to reproduce
- Expected vs actual behavior
- System information (OS, GPU, etc.)
- Screenshots if applicable

### Suggesting Features

Feature requests are welcome! Please include:
- Clear description of the feature
- Use case and benefits
- Any implementation ideas

### Code Contributions

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/your-feature`
3. **Make your changes**
4. **Test thoroughly**
5. **Commit with clear messages**: `git commit -m "Add feature: description"`
6. **Push to your fork**: `git push origin feature/your-feature`
7. **Create a Pull Request**

## Code Standards

### C++ Style
- Use C++17 features
- Follow existing code style and naming conventions
- Use smart pointers for memory management
- Comment complex logic
- Keep functions focused and small

### Naming Conventions
- Classes: `PascalCase`
- Functions: `PascalCase`
- Variables: `camelCase`
- Member variables: `m_camelCase`
- Constants: `UPPER_SNAKE_CASE`

### Example
```cpp
class MyNewFeature
{
public:
    void DoSomething();
    
private:
    int m_value;
    std::unique_ptr<Resource> m_resource;
};
```

## Building and Testing

Before submitting:
1. Build in both Debug and Release configurations
2. Test all modified features
3. Ensure no new warnings
4. Verify performance impact is minimal

```bash
# Build and test
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
# Test the executable
```

## Documentation

- Update README.md if adding major features
- Update API.md for new public APIs
- Add inline comments for complex code
- Update CHANGELOG in README.md

## Areas for Contribution

### High Priority
- Full ONNX Runtime integration for real YOLOv8 detection
- Performance optimizations
- Bug fixes

### Medium Priority
- Additional UI themes
- Hotkey system
- Multiple profile support
- Advanced tracking algorithms

### Low Priority
- Additional rendering shapes
- More configuration options
- Plugin system

## Testing

- Test on Windows 10 and Windows 11
- Test with different GPU vendors (NVIDIA, AMD, Intel)
- Test at different resolutions
- Verify multi-monitor support
- Check memory leaks
- Profile performance

## Pull Request Process

1. Ensure CI passes (when implemented)
2. Update documentation
3. Add tests if applicable
4. Request review from maintainers
5. Address review feedback
6. Squash commits if requested

## Code Review

All submissions require review. We look for:
- Code quality and style
- Functionality and correctness
- Performance impact
- Documentation completeness
- Test coverage

## Community Guidelines

- Be respectful and constructive
- Help others learn and improve
- Focus on the code, not the person
- Welcome newcomers
- Share knowledge

## Questions?

Feel free to:
- Open an issue for questions
- Start a discussion
- Reach out to maintainers

Thank you for contributing! 🎮
