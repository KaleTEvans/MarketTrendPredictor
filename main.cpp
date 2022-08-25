#include <iostream>

#include <FinancialData/FundamentalData.h>
#include <FinancialData/TechnicalData.h>
#include <FinancialData/Logger.h>
//#include <ql/math/distributions/studenttdistribution.hpp>

int main() {
    managers::LogManager fd;
    fd.Initialize();

    Fundamentals::CompanyProfile temp = Fundamentals::getCompanyProfile("PYPL");
    std::cout << temp.symbol << std::endl;
    std::cout << temp.beta << std::endl;
    std::cout << std::setprecision(15) << temp.marketCap << std::endl;

    vector<double> sr = TechnicalData::getSupportAndResistance("PYPL", "D");
    for (auto i : sr) {
        cout << i << endl;
    }

    CPPFINANCIALDATA_ERROR("This is your COCK speaking");

    fd.Shutdown();

    return 0;
}