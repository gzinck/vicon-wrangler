// CoordinateConverter.h
#include <vector>
#include <string>
#include <exception>

namespace coordinates {
	// Generic matrix type does not depend on external libs
	typedef std::vector<std::vector<double>> double_mat;
	
	class Converter {
	public:
		Converter();
		virtual ~Converter() {};
		double_mat getTransform(double_mat&, double_mat&);
	private:
	};

	class ConversionException : public std::exception {
	public:
		ConversionException(const std::string& message) : msg(message) {};
		~ConversionException() {};
		const char* what() const throw() { return msg.c_str(); }
	private:
		std::string msg;
	};
}
