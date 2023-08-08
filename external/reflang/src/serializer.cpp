#include "serializer.hpp"

#include <fstream>
#include <locale>
#include <unordered_set>

#include "serializer.class.hpp"
#include "serializer.enum.hpp"
#include "serializer.function.hpp"

using namespace std;
using namespace reflang;

namespace
{
	string CalcIncludeGuard(const serializer::Options& options)
	{
		string include_guard = options.out_hpp_path;

		for (char& c : include_guard)
		{
			c = toupper(c, std::locale());
			if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
			{
				continue;
			}
			c = '_';
		}

		return include_guard;
	}

	}

	void BeginHeader(
			ostream& o,
			const serializer::Options& options,
			const std::vector<std::unique_ptr<TypeBase>>& types)
	{
		string include_guard = CalcIncludeGuard(options);
		if (!include_guard.empty())
		{
			o << "#pragma once\n";
		}

		o << R"(#include <string>
#include<memory>
#include<nlohmann/json.hpp>
class Object;

)";
		o << options.include_path << "\n";

		// Generate unique list of includes.
		unordered_set<string> headers;
		for (const auto& type : types)
		{
			headers.insert(type->GetFile());
		}
		for (const auto& header : headers)
		{
			o << "#include \"" << header << "\"\n";
		}

		o << R"(
namespace reflang
{

)";
	}

	void EndHeader(
			ostream& o,
			const serializer::Options& options)
	{
		o << "}  // namespace reflang\n";

		string include_guard = CalcIncludeGuard(options);
		if (!include_guard.empty())
		{
			o << "\n#endif //REFLANG_METADATA_" << include_guard << "\n";
		}
	}

	void BeginSources(ostream& o, const serializer::Options& options)
	{
		if (!options.out_cpp_path.empty())
		{
			o << "#include \"" << options.out_hpp_path << "\"\n";
		}
		o << R"(
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <type_traits>

namespace reflang
{

)";
	}

	void EndSources(ostream& o)
	{
		o << "}  // namespace reflang\n";
	}
	void Deserialize(ostream& o,std::vector<Class*>& serializeables)
	{
		o << "	void Deserialize(nlohmann::json data, std::shared_ptr<::Object> object)\n";
		o << "	{\n";
		for (size_t i = 0; i < serializeables.size(); i++)
		{
			o << "		" + serializeables[i]->GetFullName() + "::" + "Deserialize(data,object);\n";
		}
		o << "	}\n";
	}
void serializer::Serialize(
		const std::vector<std::unique_ptr<TypeBase>>& types,
		const Options& options)
{
	std::unique_ptr<ofstream> fout_hpp;
	std::unique_ptr<ofstream> fout_cpp;
	ostream* out_hpp = &cout;
	ostream* out_cpp = &cout;

	if (!options.out_hpp_path.empty())
	{
		fout_hpp = make_unique<ofstream>(options.out_hpp_path.c_str());
		out_hpp = fout_hpp.get();
	}

	out_cpp = out_hpp;
	if (!options.out_cpp_path.empty())
	{
		fout_cpp = make_unique<ofstream>(options.out_cpp_path.c_str());
		out_cpp = fout_cpp.get();
	}
	BeginHeader(*out_hpp, options, types);
	std::vector<Class*> serializeables;
	for (const auto& type : types)
	{
		switch (type->GetType())
		{
		//case TypeBase::Type::Enum:
		//	SerializeEnumHeader(*out_hpp, static_cast<const Enum&>(*type));
		//	break;
		//case TypeBase::Type::Function:
		//	SerializeFunctionHeader(*out_hpp, static_cast<const Function&>(*type));
		//	break;
		case TypeBase::Type::Class:
			serializeables.push_back(static_cast<Class*>(&(*type)));
			//SerializeClassHeader(*out_hpp, static_cast<const Class&>(*type));
			break;
		}
		*out_hpp << "\n\n";
	}
	Deserialize(*out_hpp, serializeables);
	//EndHeader(*out_hpp, options);
	//
	//BeginSources(*out_cpp, options);
	//for (const auto& type : types)
	//{
	//	switch (type->GetType())
	//	{
	//	case TypeBase::Type::Enum:
	//		SerializeEnumSources(*out_cpp, static_cast<const Enum&>(*type));
	//		break;
	//	case TypeBase::Type::Function:
	//		SerializeFunctionSources(*out_cpp, static_cast<const Function&>(*type));
	//		break;
	//	case TypeBase::Type::Class:
	//		SerializeClassSources(*out_cpp, static_cast<const Class&>(*type));
	//		break;
	//	}
	//	*out_cpp << "\n\n";
	//}
	EndSources(*out_cpp);
}
