# Security Policy

## Reporting Security Vulnerabilities

If you discover a security vulnerability in this project, please report it responsibly.

### How to Report

**Please do NOT open a public issue for security vulnerabilities.**

Instead, please report security issues by:
1. Opening a private security advisory on GitHub
2. Emailing the project maintainers (if available)
3. Providing detailed information about the vulnerability

### What to Include

When reporting a security vulnerability, please include:
- Type of vulnerability
- Full paths of source files related to the vulnerability
- Location of the affected code
- Step-by-step instructions to reproduce
- Proof-of-concept or exploit code (if possible)
- Impact of the vulnerability
- Suggested fix (if any)

## Security Considerations

### This Project

This gaming overlay software:
- Runs with user-level privileges
- Uses DirectX 11 for rendering
- Accesses screen capture (for detection features)
- Reads/writes configuration files
- May load external models (ONNX files)

### Known Security Considerations

1. **Configuration Files**: Settings are stored in JSON format
   - Do not store sensitive information in settings
   - Validate all loaded configuration data

2. **Model Files**: YOLOv8 ONNX models are loaded from disk
   - Only load models from trusted sources
   - Validate model format before loading

3. **Memory Safety**: Uses C++ with DirectX
   - Uses smart pointers to prevent leaks
   - Validates all buffer operations

4. **Privilege Escalation**: Application runs as regular user
   - Does not request administrator privileges unless needed
   - No system-level modifications

## Best Practices for Users

### Safe Usage

1. **Download from Official Sources**
   - Only download from the official GitHub repository
   - Verify checksums if provided

2. **Model Files**
   - Only use YOLOv8 models from Ultralytics or trusted sources
   - Scan files with antivirus before use

3. **Configuration**
   - Review settings.json before use
   - Don't share configuration files with sensitive paths

4. **Permissions**
   - Run with minimum required privileges
   - Be cautious with screen capture permissions

### Anti-Cheat Considerations

This software may be detected by anti-cheat systems:
- **Risk**: Overlay injection may trigger anti-cheat
- **Recommendation**: Use only in offline/practice modes
- **Warning**: Use at your own risk in online games
- **Responsibility**: Respect game terms of service

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 1.0.x   | :white_check_mark: |

## Security Updates

- Security patches will be released as soon as possible
- Users will be notified via GitHub releases
- Update to the latest version promptly

## Dependencies

### DirectX 11
- Part of Windows SDK
- Updated via Windows Update
- Keep Windows updated for security patches

### Dear ImGui
- Regularly updated upstream
- Review ImGui security advisories
- Update to latest stable version

### ONNX Runtime (Optional)
- Monitor ONNX Runtime security advisories
- Update to patched versions when available

### YOLOv8 Models
- Use official Ultralytics models when possible
- Verify model integrity
- Be cautious with custom-trained models

## Code Security

### Memory Safety
- Smart pointers used throughout
- RAII principles followed
- Buffer overflow protections

### Input Validation
- All user input validated
- Configuration files parsed safely
- Model files validated before loading

### Least Privilege
- Runs with user-level permissions
- No unnecessary system access
- Minimal attack surface

## Vulnerability Disclosure Timeline

1. **Day 0**: Vulnerability reported privately
2. **Day 1-7**: Confirm and assess vulnerability
3. **Day 7-30**: Develop and test fix
4. **Day 30**: Release security patch
5. **Day 30+**: Public disclosure (after patch)

## Attribution

We appreciate security researchers who:
- Report vulnerabilities responsibly
- Allow time for fixes before disclosure
- Help improve project security

Contributors who report valid security issues will be acknowledged (with permission) in:
- Security advisories
- Release notes
- CHANGELOG

## Disclaimer

This software is provided "as is" without warranty. Users are responsible for:
- Compliance with applicable laws
- Respect for game terms of service
- Safe and ethical usage
- Understanding security implications

## Additional Resources

- [OWASP Secure Coding Practices](https://owasp.org/www-project-secure-coding-practices-quick-reference-guide/)
- [CWE Top 25](https://cwe.mitre.org/top25/)
- [Microsoft Security Development Lifecycle](https://www.microsoft.com/en-us/securityengineering/sdl/)

## Questions?

For security-related questions that are not vulnerabilities:
- Open a public issue tagged as "security"
- Start a discussion in the security category

Thank you for helping keep this project secure! 🔒
