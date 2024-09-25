/*
   Author: Roy Bebru (RoyBebru@Gmail.Com)
*/

#ifndef _DATAMAKER_HPP_
# define _DATAMAKER_HPP_

# include <vector>

# include "../my/etc.hpp"

struct DataMaker
{
    std::vector<double> data;
    DataMaker(uint amount, double range_y);
};
#endif // _DATAMAKER_HPP_
