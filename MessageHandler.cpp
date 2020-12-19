// MessageHandler.cpp
#include "MessageHandler.h"

using namespace messages;
using Json::Value;

coordinates::double_mat toMat(Json::Value& msg) {
	coordinates::double_mat result;

	if (msg.isArray()) {
		for (Value::ArrayIndex i = 0; i != msg.size(); ++i) {
			std::vector<double> v;
			for (Value::ArrayIndex j = 0; j != msg[i].size(); ++j)
				v.push_back(msg[i][j].asDouble());
			result.push_back(v);
		}
	}

	return result;
}

Json::Value toJson(coordinates::double_mat& mat) {
	Json::Value arr;

	for (int i = 0; i < mat.size(); ++i) {
		Json::Value row;
		for (int j = 0; j < mat.at(i).size(); ++j)
			row.append(mat.at(i).at(j));
		arr.append(row);
	}

	return arr;
}

Handler::Handler() {}

Value Handler::handleMessage(Json::Value& msg) {
	coordinates::double_mat source = toMat(msg["source"]);
	coordinates::double_mat dest = toMat(msg["dest"]);

	try {
		coordinates::double_mat transform = converter.getTransform(source, dest);
		Json::Value ret;
		ret["result"] = toJson(transform);
		ret["type"] = "transformationMatrix";
		return ret;
	} catch (coordinates::ConversionException const &e) {
		Json::Value ret;
		ret["error"] = e.what();
		return ret;
	}
}
