#include <IniParser.h>
#include <fstream>
#include <sstream>
#include <algorithm>

IniParser::IniParser(const char* p_file, const bool p_loadFile)
	: m_filePath{ p_file }
{
	Load(p_file);
}

IniParser::~IniParser()
{
	m_data.clear();
}

void IniParser::RegisterPair(const std::string& p_key,
	const std::string& p_value)
{
	RegisterPair(std::make_pair(p_key, p_value));
}

void IniParser::RegisterPair(const std::pair<std::string, std::string>& p_pair)
{
	m_data.insert(p_pair);
}

bool IniParser::Load()
{
	return Load(m_filePath.c_str());
}

bool IniParser::Load(const char* p_file)
{
	std::ifstream file(p_file);

	if (file.is_open())
	{
		std::string currentLine;

		while (std::getline(file, currentLine))
		{
			if (IsValidLine(currentLine))
			{
				currentLine.erase(
					std::remove_if(
						currentLine.begin(),
						currentLine.end(),
						std::isspace), currentLine.end()
				);
				RegisterPair(ExtractKeyValue(currentLine));
			}
		}

		file.close();
		return true;
	}

	return false;
}

void IniParser::Reload()
{
	m_data.clear();
	Load(m_filePath.c_str());
}

bool IniParser::IsValidLine(const std::string& p_attributeLine)
{
	if (p_attributeLine.empty())
		return false;

	if (p_attributeLine.front() == ';' || p_attributeLine.front() == '[')
		return false;

	if (p_attributeLine.find_first_of('=') == static_cast<size_t>(-1))
		return false;

	return true;
}

bool IniParser::KeyExist(const std::string& p_key)
{
	return m_data.find(p_key) != m_data.end();
}

bool IniParser::Save()
{
	std::ofstream outfile;
	outfile.open(m_filePath, std::ios_base::out | std::ios_base::trunc);

	if (outfile.is_open())
	{
		for (auto& attribute : m_data)
			outfile << attribute.first << "=" << attribute.second << std::endl;

		outfile.close();
		return true;
	}

	return false;
}

bool IniParser::Remove(const std::string& p_key)
{
	if (KeyExist(p_key))
	{
		m_data.erase(p_key);
		return true;
	}

	return false;
}

bool IniParser::StringToBoolean(const std::string& p_value)
{
	return (p_value == "1" || p_value == "T" || p_value == "t" || p_value == "True" || p_value == "true");
}

std::pair<std::string, std::string> IniParser::ExtractKeyValue(
	const std::string& p_line)
{
	const size_t equalPosition = p_line.find('=');

	return std::make_pair(p_line.substr(0, equalPosition), p_line.substr(equalPosition + 1));
}
