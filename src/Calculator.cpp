#include "Calculator.h"
#include "Math.h"

Statistics Calculator::calculateStats(
		double arrivalRate,
		double serviceRate,
		unsigned int channelCount,
		unsigned int queueMaxLength)
{
	double rho = arrivalRate / serviceRate;

	Statistics stats{};

	double term = 1;
	double noQueueProbability = term;
	for (long unsigned int i = 1; i <= channelCount + queueMaxLength; i++)
	{
		if (i <= channelCount)
		{
			term *= rho / i;
		}
		else
		{
			term *= rho / channelCount;
		}

		noQueueProbability += term;
	}
	noQueueProbability = 1 / noQueueProbability;

	stats.rejectionProbability
			= noQueueProbability
			  * Math::pow(rho, channelCount + queueMaxLength)
			  / (Math::factorial(channelCount) * Math::pow(channelCount, queueMaxLength));

	stats.relativeThroughput = 1 - stats.rejectionProbability;
	stats.absoluteThroughput = arrivalRate * stats.relativeThroughput;

	return stats;
}
