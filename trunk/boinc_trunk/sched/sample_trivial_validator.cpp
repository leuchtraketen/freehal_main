// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2008 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

// A sample validator that grants credit to any result whose CPU time is above
// a certain minimum

#include <cstdlib>
#include "config.h"
#include "validate_util.h"
#include <string>

using std::vector;
#include <iostream>
using namespace std;

static const double MIN_CPU_TIME = 0;

int init_result(RESULT& result, void*& /*data*/) {
    return 0;
}

int compare_results(
    RESULT & r1, void* /*data1*/,
    RESULT const& r2, void* /*data2*/,
    bool& match
) {

    return 0;
}

int cleanup_result(RESULT const&, void*) {
    return 0;
}

double compute_granted_credit(WORKUNIT& wu, vector<RESULT>& results) {
    double credits = 0;
    for (int i = 0; i < results.size(); ++i ) {
    if ( (results[i].received_time - results[i].sent_time) < 60*70 ) {
        cout << (results[i].received_time - results[i].sent_time) << endl;
        credits = 1;
    }
    }

    if (credits == 0)
        credits = 15;

    return credits;
}

const char *BOINC_RCSID_f3a7a34795 = "$Id: sample_trivial_validator.cpp 16069M 2012-01-02 16:24:51Z (lokal) $";
