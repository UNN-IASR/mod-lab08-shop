#pragma once

struct Statistics
{
public:
	double rejectionProbability;
	double relativeThroughput;
	double absoluteThroughput;
};

class Calculator
{
public:
	static Statistics calculateStats(
			double arrivalRate,
			double serviceRate,
			unsigned int channelCount,
			unsigned int queueMaxLength);
};
