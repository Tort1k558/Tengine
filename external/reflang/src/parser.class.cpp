#include "parser.class.hpp"

#include "parser.util.hpp"

#include<iostream>

using namespace reflang;
using namespace std;

namespace
{
	Function GetMethodFromCursor(CXCursor cursor)
	{
		auto type = clang_getCursorType(cursor);

		Function f(
				parser::GetFile(cursor), parser::GetFullName(cursor));
		f.Name = parser::Convert(clang_getCursorSpelling(cursor));
		int num_args = clang_Cursor_getNumArguments(cursor);
		for (int i = 0; i < num_args; ++i)
		{
			auto arg_cursor = clang_Cursor_getArgument(cursor, i);
			NamedObject arg;
			arg.Name = parser::Convert(
					clang_getCursorSpelling(arg_cursor));
			if (arg.Name.empty())
			{
				arg.Name = "nameless";
			}
			auto arg_type = clang_getArgType(type, i);
			arg.Type = parser::GetName(arg_type);
			f.Arguments.push_back(arg);
		}

		f.ReturnType = parser::GetName(clang_getResultType(type));
		return f;
	}

	NamedObject GetFieldFromCursor(CXCursor cursor)
	{
		NamedObject field;
		field.Name = parser::Convert(clang_getCursorSpelling(cursor));
		//field.Type = parser::GetName(clang_getCursorType(cursor));
		switch (clang_getCXXAccessSpecifier(cursor))
		{
		case CX_CXXPrivate:
			field.Access = AccessModifier::PRIVATE;
			break;
		case CX_CXXProtected:
			field.Access = AccessModifier::PROTECTED;
			break;
		case CX_CXXPublic:
			field.Access = AccessModifier::PUBLIC;
			break;
		default:
			break;
		}
		CXToken* tokens;
		unsigned int numTokens;
		clang_tokenize(clang_Cursor_getTranslationUnit(cursor), clang_getCursorExtent(cursor), &tokens, &numTokens);
		string type;
		for (unsigned int i = 0; i < numTokens; ++i) {
			CXString tokenCXString = clang_getTokenSpelling(clang_Cursor_getTranslationUnit(cursor), tokens[i]);
			string tokenStr = clang_getCString(tokenCXString);
			if (tokenStr == field.Name || tokenStr == "static" || tokenStr == "const" || tokenStr == "volatile")
			{
				continue;
			}
			type += tokenStr;
		}
		if (type.find("=") != type.npos)
		{
			type.erase(type.begin() + type.find("="), type.end());
		}
		field.Type = type;
		std::cout << field.Type<< std::endl;
		return field;
	}

	CXChildVisitResult VisitClass(
			CXCursor cursor, CXCursor parent, CXClientData client_data)
	{
		auto* c = reinterpret_cast<Class*>(client_data);
		switch (clang_getCursorKind(cursor))
		{
		//case CXCursor_CXXMethod:
		//	if (clang_CXXMethod_isStatic(cursor) != 0)
		//	{
		//		c->StaticMethods.push_back(GetMethodFromCursor(cursor));
		//	}
		//	else
		//	{
		//		c->Methods.push_back(GetMethodFromCursor(cursor));
		//	}
		//	break;
		case CXCursor_FieldDecl:
			c->Fields.push_back(GetFieldFromCursor(cursor));
			break;
		case CXCursor_VarDecl:
			c->StaticFields.push_back(GetFieldFromCursor(cursor));
			break;
		default:
			break;
		}
		return CXChildVisit_Continue;
	}
}

Class parser::GetClass(CXCursor cursor)
{
	Class c(GetFile(cursor), GetFullName(cursor));

	CXToken* tokens;
	unsigned int numTokens;
	clang_tokenize(clang_Cursor_getTranslationUnit(cursor), clang_getCursorExtent(cursor), &tokens, &numTokens);
	string str;
	for (unsigned int i = 0; i < numTokens; ++i) {
		CXString tokenString = clang_getTokenSpelling(clang_Cursor_getTranslationUnit(cursor), tokens[i]);
		str += clang_getCString(tokenString);
	}
	if (str.find("TRACEABLE") != str.npos)
	{
		c.Traceable = true;
	}

	clang_visitChildren(cursor, VisitClass, &c);
	return c;
}
