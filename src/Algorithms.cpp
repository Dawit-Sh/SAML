/*
 * SAML 0.0.1
 * Author - Kidus W. (hello@kidusw.com)
 * License - GNU GPL v.3
 */

#include <algorithm>
#include <cmath>

#include "Algorithms.h"

double SAML::find_total(std::vector < double >& values)
{
    double total = 0;

    if (values.size() > 0)
    {
        for (double i = 0; i  < values.size(); i++)
        {
            total = total  + values[i];

        }
    }

    else return -1;

    return total;
}

double SAML::deviate_list(std::vector < double >& values)
{
    double variance = SAML::find_variance(values);
    return (double)(sqrt(variance));
}

double SAML::find_mean(std::vector < double >& values)
{
    double total = SAML::find_total(values);
    return total / (double)(values.size() - 1);
}

double SAML::find_median(std::vector < double >& values)
{
    unsigned _size = values.size() - 2;
    unsigned index = 0;

    if (!(values.size() % 2 == 0)) //Even number of values
    {
        index = (_size + 1) / 2;
    }

    else if (values.size() % 2 != 0) {
        index = ((_size / 2) + ((_size / 2) + 1) / 2);
    }

    return values[index];
}

double SAML::find_mode(std::vector < double >& values)
{
    std::cout << "Excuse: Algorithm not implemented yet.\n\n";
    return 0;
}

double SAML::find_range(std::vector < double >& values)
{
    std::vector < double > sorted = values;
    std::sort(sorted.begin(), sorted.end());

    return sorted[sorted.size() - 1] - sorted[1]; //Last element is 0, remember
}

double SAML::find_variance(std::vector < double >& values)
{
    double mean = SAML::find_mean(values);
    double sigma = 0;

    for (unsigned i = 0; i < values.size(); i++)
    {
        sigma = sigma + pow((values[i] - mean), 2);
    }

    return sigma / ((double)((values.size() - 1)));
}
