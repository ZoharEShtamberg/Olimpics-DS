#include <stdexcept>
		
class KeyAlreadyExistsException : public std::logic_error {
public:
	KeyAlreadyExistsException() : std::logic_error("Key already exists") {}
};

class KeyDoesNotExistException : public std::logic_error {
public:
	KeyDoesNotExistException() : std::logic_error("Key does not exist") {}
};

class EmptyTreeException : public std::logic_error {
public:
	EmptyTreeException() : std::logic_error("Tree is empty") {}
};

