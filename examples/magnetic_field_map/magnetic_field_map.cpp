

// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#include <iostream>

#include <JANA/JApplication.h>
#include <JANA/Calibrations/JCalibrationManager.h>
#include <JANA/Calibrations/JCalibrationGeneratorCCDB.h>
#include "DMagneticFieldMapFineMesh.h"
#include "surrogate.h"
#include "torchscript_model.h"
#include "feedforward_model.h"

using phasm::Model, phasm::Direction;

int main() {


    // auto model = std::make_shared<phasm::TorchscriptModel>("model.pt");
    auto model = std::make_shared<phasm::FeedForwardModel>();
    model->add_var<double>("x", Direction::IN);
    model->add_var<double>("y", Direction::IN);
    model->add_var<double>("z", Direction::IN);
    model->add_var<double>("Bx", Direction::OUT);
    model->add_var<double>("By", Direction::OUT);
    model->add_var<double>("Bz", Direction::OUT);
    model->initialize();

    phasm::Surrogate::set_call_mode(phasm::Surrogate::CallMode::CaptureAndDump);

    japp = new JApplication;
    auto calib_man = std::make_shared<JCalibrationManager>();
    // calib_man->AddCalibrationGenerator(new JCalibrationGeneratorCCDB);
    // japp->SetParameterValue("jana:calib_url", "mysql://ccdb_user@hallddb.jlab.org/ccdb");
    japp->SetParameterValue("jana:calib_url", "file:///cvmfs/oasis.opensciencegrid.org/gluex/group/halld/www/halldweb/html/resources");
    japp->SetParameterValue("jana:resource_dir", "/cvmfs/oasis.opensciencegrid.org/gluex/group/halld/www/halldweb/html/resources");
    japp->ProvideService(calib_man);
    // DMagneticFieldMapFineMesh mfmfm(japp, 1, "Magnets/Solenoid/solenoid_1350A_poisson_20160222");
    DMagneticFieldMapFineMesh mfmfm(japp, "/cvmfs/oasis.opensciencegrid.org/gluex/group/halld/www/halldweb/html/resources/Magnets/Solenoid/finemeshes/solenoid_1350A_poisson_20160222.evio");

    double x, y, z, bx, by, bz;

    phasm::Surrogate surrogate([&](){ mfmfm.GetField(x,y,z,bx,by,bz); }, model);
    surrogate.bind("x", &x);
    surrogate.bind("y", &y);
    surrogate.bind("z", &z);
    surrogate.bind("Bx", &bx);
    surrogate.bind("By", &by);
    surrogate.bind("Bz", &bz);

    for (x = 0.0; x < 3.0; x+=.5) {
        for (y = 0.0; y < 3.0; y+=0.5) {
            for (z = 0.0; z < 3.0; z+=0.5) {
                surrogate.call();
            }
        }
    }
}

