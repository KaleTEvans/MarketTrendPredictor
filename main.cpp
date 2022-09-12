#include <iostream>

// Include boost shared pointer first to avoid cpprest macro U()
#include <boost/make_shared.hpp>
#include <FinancialData/FundamentalData.h>
#include <FinancialData/TechnicalData.h>
#include <FinancialData/Logger.h>
#include <ql/math/distributions/studenttdistribution.hpp>

#include <armadillo>

using namespace arma;

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

    uword d = 10;
    uword N = 10000;

    mat data(d, N, fill::zeros);

    vec mean1 = linspace<vec>(1,d,d);
    vec mean2 = mean1 + 2;

    uword i = 0;

    while (i < N) {
        if(i < N) { data.col(i) = mean1 + randn<vec>(d); ++i; }
        if(i < N) { data.col(i) = mean1 + randn<vec>(d); ++i; }
        if(i < N) { data.col(i) = mean2 + randn<vec>(d); ++i; }
    }

    // model the data as a diagonal GMM with 2 Gaussians
    gmm_diag model;

    bool status = model.learn(data, 2, maha_dist, random_subset, 10, 5, 1e-10, true);

    if(status == false) { cout << "learning failed" << endl; }

    model.means.print("means");

    double overall_likelihood = model.avg_log_p(data);

    rowvec set_likelihood = model.log_p( data.cols(0,9) );
    double scalar_likelihood = model.log_p( data.col(0) );

    uword gaus_id = model.assign( data.col(0), eucl_dist );
    urowvec gaus_ids = model.assign( data.cols(0,9), prob_dist );

    urowvec histogram1 = model.raw_hist (data, prob_dist);
    rowvec histogram2 = model.norm_hist(data, eucl_dist);

    model.save("my_model.gmm");

    mat modified_dcovs = 2 * model.dcovs;

    model.set_dcovs(modified_dcovs);


    fd.Shutdown();

    return 0;
}