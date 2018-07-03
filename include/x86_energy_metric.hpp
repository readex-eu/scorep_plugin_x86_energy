/*
 * x86_energy_metric.hpp
 *
 *  Created on: 29.06.2018
 *      Author: gocht
 */

#ifndef INCLUDE_X86_ENERGY_METRIC_HPP_
#define INCLUDE_X86_ENERGY_METRIC_HPP_

#include <chrono>
#include <string>

#include <x86_energy.hpp>

/** Our x86_energy metric handle
 *
 **/
class x86_energy_metric
{
public:
    x86_energy_metric(const std::string& full_name_, const std::string& name_,
                      std::vector<x86_energy::SourceCounter>&& sensor_,
                      const std::string& quantity_, bool im_blade_, double offset_)
    : mfull_name(full_name_), mname(name_), msensor(std::move(sensor_)), mquantity(quantity_),
      im_blade(im_blade_), offset(offset_)
    {
        init_energy = read();
        first_measurement = std::chrono::steady_clock::now();
    }

    // delete copy constructor to avoid ... copies,
    // needs move and default constructor though!
    x86_energy_metric(x86_energy_metric&&) = default;
    x86_energy_metric(const x86_energy_metric&) = delete;
    /* copy-assign */
    x86_energy_metric& operator=(const x86_energy_metric&) = delete;
    /* move assignment */
    x86_energy_metric& operator=(x86_energy_metric&&) = default;

    double first = -1;
    double acct = 0;
    double diff = 0;
    double last = 0;

    /* getter functions */
    std::string name() const
    {
        return mname;
    }

    std::string full_name() const
    {
        return mfull_name;
    }

    double read()
    {
        double tmp = 0;
        for (auto& sens : msensor)
        {
            tmp += sens.read();
        }
        tmp = tmp - init_energy;
        if (im_blade)
        {
            tmp = tmp +
                  offset *
                      std::chrono::duration_cast<std::chrono::seconds>(
                          std::chrono::steady_clock::now() - first_measurement)
                          .count();
        }
        return tmp;
    }

    std::string quantity() const
    {
        return mquantity;
    }

private:
    std::string mfull_name;
    std::string mname;                              /*<sensor is the ident number of x86_energy */
    std::vector<x86_energy::SourceCounter> msensor; /*< on wich cpu is the sensor */
    std::string mquantity;
    double offset; /*< offset for network cardes and so on in the blade */
    double init_energy = 0;
    bool im_blade = false;

    std::chrono::time_point<std::chrono::steady_clock> first_measurement;
};

/** operator to print the metric handle
 *
 *
 **/
std::ostream& operator<<(std::ostream& s, const x86_energy_metric& metric)
{
    s << "(" << metric.full_name() << ")";
    return s;
}

#endif /* INCLUDE_X86_ENERGY_METRIC_HPP_ */
