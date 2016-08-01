# unhardlink

Tests if file is a hardlink and in that case replace it with a copy of the same content.

### Example

To ensure there are no hardlinks in a directory tree:

	find . -type f -links +1 -print0 | xargs -0 unhardlink

