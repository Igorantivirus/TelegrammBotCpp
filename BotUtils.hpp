#pragma once

#include <locale>
#include <codecvt>
#include <complex>
#include <fstream>

void existOrCreateFile(const char* fName)
{
	std::ofstream file(fName, std::ios::app);
	file.close();
}

std::string to_utf8(const std::wstring& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(str);
}

std::string toFixedString(const long double x)
{
	std::string s = std::to_string(x);
	if (s.find('.') == std::string::npos)
		return s;
	while (s.back() == '0')
		s.pop_back();
	if (s.back() == '.')
		s.pop_back();
	return s;
}
std::string complexToString(const std::complex<long double>& z)
{
	if (!z.imag())
		return toFixedString(z.real());
	else if (!z.real())
		return toFixedString(z.imag()) + 'i';
	else
		return toFixedString(z.real()) + (z.imag() > 0 ? '+' : '-') + toFixedString(std::abs(z.imag())) + 'i';
}