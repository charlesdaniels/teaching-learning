#ifndef UTIL_H
#define UTIL_H

#define isLetter(_c) ( ( ( ((char) _c) >= 'A' ) &&  ( ((char) _c) <= 'Z') ) || \
	( ( ((char) _c) >= 'a' ) &&  ( ((char) _c) <= 'z') ) || \
	( ((char) _c) == '_') )

#define isWhitespace(_c) ( ( ((char) _c) == ' ' ) || \
	( ((char) _c) == '\t' ) || \
	( ((char) _c) == '\n' ) || \
	( ((char) _c) == '\r' ) )

#define isDigit(_c) ( ( ( ((char) _c) >= '0' ) &&  ( ((char) _c) <= '9') ))


#endif /* UTIL_H */
