# Contributing to FOV Overlay Application

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## Code of Conduct

- Be respectful and inclusive
- Provide constructive feedback
- Focus on the issue, not the person
- Help create a welcoming environment

## How to Contribute

### Reporting Bugs

1. **Check existing issues** - search for similar bugs first
2. **Create detailed report** including:
   - Operating system and version
   - Build configuration (Debug/Release)
   - Steps to reproduce
   - Expected vs actual behavior
   - Screenshots if applicable
   - Relevant log output

### Suggesting Features

1. **Check existing feature requests**
2. **Provide clear description**:
   - Use case and motivation
   - Proposed solution
   - Alternative approaches considered
   - Potential impact on existing features

### Pull Requests

#### Before Starting
1. Create or comment on an issue to discuss the change
2. Fork the repository
3. Create a feature branch from `main`

#### Development Process
1. **Setup environment**:
   ```bash
   git clone https://github.com/yourusername/Cc.git
   cd Cc
   git checkout -b feature/your-feature-name
   ```

2. **Make changes**:
   - Follow the coding standards (see DEVELOPMENT.md)
   - Write clear commit messages
   - Add documentation for new features
   - Update CHANGELOG.md

3. **Test thoroughly**:
   - Build in Debug and Release modes
   - Test all affected features
   - Check for memory leaks
   - Verify performance impact

4. **Commit**:
   ```bash
   git add .
   git commit -m "Add feature: brief description"
   ```

5. **Push and create PR**:
   ```bash
   git push origin feature/your-feature-name
   ```
   - Create pull request on GitHub
   - Fill out PR template
   - Link related issues

#### Pull Request Guidelines

**Title Format**: `[Type] Brief description`
- Types: `Feature`, `Fix`, `Docs`, `Refactor`, `Perf`, `Test`
- Example: `[Feature] Add circular FOV rendering`

**Description Should Include**:
- What changed and why
- How to test the changes
- Screenshots/videos for UI changes
- Breaking changes (if any)
- Related issues

**Code Quality**:
- [ ] Follows C++17 standards
- [ ] No compiler warnings
- [ ] Comments for complex logic
- [ ] Documentation updated
- [ ] Build succeeds on Windows

## Development Workflow

### Branch Naming
- `feature/description` - New features
- `fix/description` - Bug fixes
- `docs/description` - Documentation
- `refactor/description` - Code refactoring
- `perf/description` - Performance improvements

### Commit Messages
Use clear, descriptive commit messages:

```
Add sound tracking visualization

- Implement SoundTracker class
- Add directional indicators
- Configure Y-axis sensitivity
- Update documentation
```

### Coding Standards

#### C++ Style
```cpp
// Use snake_case for functions and variables
void calculate_fps();
int frame_count;

// Use PascalCase for classes
class OverlayManager;

// Use UPPER_CASE for constants
const int MAX_DETECTIONS = 100;

// Always use explicit types
auto result = detector.Detect(...);  // Good
auto r = det.Det(...);               // Bad

// Prefer const references
void ProcessDetections(const std::vector<DetectionBox>& detections);
```

#### Documentation
```cpp
/// Brief description
///
/// Detailed explanation of what the function does,
/// parameters, return value, and any important notes.
///
/// @param device DirectX 11 device
/// @param context DirectX 11 device context
/// @return true on success, false on failure
bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
```

#### Error Handling
```cpp
// Check return values
if (!overlay.Initialize(...)) {
    std::cerr << "Failed to initialize overlay" << std::endl;
    return false;
}

// Use RAII for resource management
class ResourceHolder {
public:
    ResourceHolder() : resource_(CreateResource()) {}
    ~ResourceHolder() { ReleaseResource(resource_); }
private:
    Resource* resource_;
};
```

## Testing

### Manual Testing Checklist
- [ ] Application launches successfully
- [ ] Overlay renders correctly
- [ ] FOV circle displays at correct position
- [ ] Settings menu toggles properly
- [ ] Configuration loads from file
- [ ] No crashes during normal operation
- [ ] Clean shutdown with ESC key
- [ ] FPS stays above 60

### Performance Testing
- Monitor FPS for 5 minutes continuous use
- Check CPU usage (should be < 10%)
- Check GPU usage (should be < 20%)
- Verify no memory leaks

### Regression Testing
After making changes, verify:
- [ ] Existing features still work
- [ ] No new compiler warnings
- [ ] Build succeeds on all configurations
- [ ] Documentation is accurate

## Code Review Process

### For Contributors
- Respond to feedback promptly
- Make requested changes in new commits
- Don't force push after review starts
- Mark conversations as resolved when addressed

### For Reviewers
- Review within 48 hours if possible
- Provide specific, actionable feedback
- Explain the reasoning behind suggestions
- Approve when ready or request changes

## Release Process

1. Update version in CMakeLists.txt
2. Update CHANGELOG.md
3. Create release branch
4. Final testing
5. Merge to main
6. Tag release
7. Build release binaries
8. Create GitHub release

## Questions?

- Open an issue for questions
- Tag with `question` label
- Check existing documentation first

## Recognition

Contributors will be acknowledged in:
- CHANGELOG.md
- GitHub contributors page
- Release notes

Thank you for contributing! 🎉
