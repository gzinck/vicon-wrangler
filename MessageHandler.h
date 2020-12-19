// MessageHandler.h
#include "CoordinateConverter.h"
#include <string>
#include <json/value.h>

namespace messages {
	class Handler {
	public:
		Handler();
		Json::Value handleMessage(Json::Value&);
		virtual ~Handler() {};
	private:
		coordinates::Converter converter;
	};
}
