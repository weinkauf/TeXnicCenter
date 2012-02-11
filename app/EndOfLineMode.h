#pragma once

/// Represents the style of line endings.
enum EndOfLineMode
{
	AutomaticEOLMode = -1,
	DOSStyleEOLMode = 0,
	UnixStyleEOLMode = 1,
	MacStyleEOLMode = 2
};