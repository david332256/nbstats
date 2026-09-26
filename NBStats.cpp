#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <iomanip>

void showHelp() {
	std::cout << "NBStats - Newcomb-Benford Statistical Analysis" << std::endl;
	std::cout << "Usage: nbstats.exe [filename] [--skipbad] [--help]" << std::endl;
	std::cout << std::endl;
	std::cout << "  filename    - Optional file to read numbers from" << std::endl;
	std::cout << "  --skipbad   - Skip non-numeric input instead of terminating" << std::endl;
	std::cout << "  --help      - Display this help message" << std::endl;
	std::cout << std::endl;
	std::cout << "Press Ctrl+Z to end input when reading from keyboard." << std::endl;
}

bool isNumber(const std::string& s) {
	if (s.empty()) return false;

	size_t i = 0;
	if (s[0] == '-' || s[0] == '+') i++;

	bool hasDecimal = false;
	for (; i < s.length(); i++) {
		if (s[i] == '.') {
			if (hasDecimal) return false;
			hasDecimal = true;
		}
		else if (!std::isdigit(s[i])) {
			return false;
		}
	}
	return true;
}

int main(int argc, char* argv[]) {
	std::vector<double> numbers;
	std::string filename;
	bool skipBad = false;
	bool showHelpFlag = false;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];

		if (arg == "--help") {
			showHelpFlag = true;
		}
		else if (arg == "--skipbad") {
			skipBad = true;
		}
		else if (arg[0] != '-') {
			filename = arg;
		}
		else {
			std::cout << "Unknown option: " << arg << std::endl;
			showHelp();
			return 1;
		}
	}

	if (showHelpFlag) {
		showHelp();
		return 0;
	}

	std::ifstream fileStream;
	std::istream* inputStream = &std::cin;

	if (!filename.empty()) {
		fileStream.open(filename);
		if (!fileStream.is_open()) {
			std::cout << "Error: Could not open file: " << filename << std::endl;
			return 1;
		}
		inputStream = &fileStream;
		std::cout << "Reading from file: " << filename << std::endl;
	}
	else {
		std::cout << "Enter numbers (Ctrl+Z to end):" << std::endl;
	}

	std::string inputString;
	bool hadError = false;

	while (*inputStream >> inputString) {
		if (isNumber(inputString)) {
			double value = std::stod(inputString);

			if (value > 0) {
				numbers.push_back(value);
			}
			else {
				std::cout << "Skipped (not positive): " << value << std::endl;
			}
		}
		else {
			if (skipBad) {
				std::cout << "Warning: Skipping non-numeric input: " << inputString << std::endl;
			}
			else {
				std::cout << "Error: Non-numeric input detected: " << inputString << std::endl;
				hadError = true;
				break;
			}
		}
	}

	if (hadError) {
		return 1;
	}

	std::cout << "Loaded " << numbers.size() << " positive numbers." << std::endl;

	if (!numbers.empty()) {
		std::sort(numbers.begin(), numbers.end());

		double minVal = numbers.front();
		double maxVal = numbers.back();
		std::cout << "\nRange: [" << minVal << ", " << maxVal << "]" << std::endl;

		double sum = 0;
		for (double d : numbers) {
			sum += d;
		}
		double mean = sum / numbers.size();
		std::cout << "Mean: " << mean << std::endl;

		double median;
		size_t n = numbers.size();
		if (n % 2 == 1) {
			median = numbers[n / 2];
		}
		else {
			median = (numbers[n / 2 - 1] + numbers[n / 2]) / 2.0;
		}
		std::cout << "Median: " << median << std::endl;

		double variance = 0;
		for (double d : numbers) {
			double diff = d - mean;
			variance += diff * diff;
		}
		variance /= n;
		std::cout << "Variance: " << variance << std::endl;

		double stdDev = std::sqrt(variance);
		std::cout << "Standard Deviation: " << stdDev << std::endl;

		std::vector<double> modes;
		int maxFrequency = 0;
		int currentCount = 1;

		for (size_t i = 1; i < n; ++i) {
			if (numbers[i] == numbers[i - 1]) {
				currentCount++;
			}
			else {
				if (currentCount > maxFrequency) {
					maxFrequency = currentCount;
					modes.clear();
					modes.push_back(numbers[i - 1]);
				}
				else if (currentCount == maxFrequency) {
					modes.push_back(numbers[i - 1]);
				}
				currentCount = 1;
			}
		}

		if (currentCount > maxFrequency) {
			maxFrequency = currentCount;
			modes.clear();
			modes.push_back(numbers[n - 1]);
		}
		else if (currentCount == maxFrequency) {
			modes.push_back(numbers[n - 1]);
		}

		if (maxFrequency == 1) {
			std::cout << "Mode: No mode (all values appear once)" << std::endl;
		}
		else if (modes.size() == 1) {
			std::cout << "Mode: " << modes[0] << " (appears " << maxFrequency << " times)" << std::endl;
		}
		else {
			std::cout << "Mode: ";
			for (size_t i = 0; i < modes.size(); ++i) {
				if (i > 0) std::cout << ", ";
				std::cout << modes[i];
			}
			std::cout << " (each appears " << maxFrequency << " times)" << std::endl;
		}

		std::vector<int> digitCounts(10, 0);

		for (double d : numbers) {
			double absVal = std::abs(d);
			if (absVal >= 1.0) {
				double logVal = std::log10(absVal);
				int exponent = static_cast<int>(std::floor(logVal));
				double firstDigitVal = absVal / std::pow(10.0, exponent);
				int firstDigit = static_cast<int>(std::floor(firstDigitVal));
				if (firstDigit >= 1 && firstDigit <= 9) {
					digitCounts[firstDigit]++;
				}
			}
			else if (absVal > 0.0) {
				double logVal = std::log10(absVal);
				int exponent = static_cast<int>(std::floor(logVal));
				double firstDigitVal = absVal / std::pow(10.0, exponent);
				int firstDigit = static_cast<int>(std::floor(firstDigitVal));
				if (firstDigit >= 1 && firstDigit <= 9) {
					digitCounts[firstDigit]++;
				}
			}
		}

		std::cout << "\nBenford's Law Analysis:" << std::endl;
		std::cout << "Digit  Expected%  Actual%   Bar" << std::endl;
		std::cout << "-----  ---------  --------  ----------" << std::endl;

		double maxActualPercent = 0;
		std::vector<double> expectedPercent(10);
		std::vector<double> actualPercent(10);

		for (int d = 1; d <= 9; ++d) {
			expectedPercent[d] = std::log10(1.0 + 1.0 / d) * 100.0;
			actualPercent[d] = (static_cast<double>(digitCounts[d]) / numbers.size()) * 100.0;
			if (actualPercent[d] > maxActualPercent) {
				maxActualPercent = actualPercent[d];
			}
		}

		int maxBarLength = 50;
		double maxPercent = std::max(maxActualPercent, 50.0);
		if (maxActualPercent < 50.0) {
			maxPercent = 50.0;
		}

		for (int d = 1; d <= 9; ++d) {
			int barLength = static_cast<int>((actualPercent[d] / maxPercent) * maxBarLength);

			std::cout << std::fixed << std::setprecision(2);
			std::cout << "  " << d << "    "
				<< std::setw(8) << expectedPercent[d] << "%  "
				<< std::setw(8) << actualPercent[d] << "%  ";

			for (int i = 0; i < barLength; ++i) {
				std::cout << "*";
			}
			std::cout << std::endl;
		}

		double nbVariance = 0;
		for (int d = 1; d <= 9; ++d) {
			double expected = std::log10(1.0 + 1.0 / d);
			double actual = static_cast<double>(digitCounts[d]) / numbers.size();
			if (expected > 0) {
				double ratio = actual / expected;
				nbVariance += (ratio - 1.0) * (ratio - 1.0);
			}
		}
		nbVariance /= 9.0;
		double nbDeviation = std::sqrt(nbVariance);

		std::cout << "\nNB Variance: " << std::fixed << std::setprecision(6) << nbVariance << std::endl;
		std::cout << "NB Deviation: " << nbDeviation << std::endl;

		std::string strength;
		if (nbDeviation < 0.1) {
			strength = "very strong";
		}
		else if (nbDeviation < 0.2) {
			strength = "strong";
		}
		else if (nbDeviation < 0.35) {
			strength = "moderate";
		}
		else if (nbDeviation < 0.5) {
			strength = "weak";
		}
		else {
			strength = "none";
		}
		std::cout << "NB Relationship Strength: " << strength << std::endl;
	}

	return 0;
}