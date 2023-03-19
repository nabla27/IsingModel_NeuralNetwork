#include <QCoreApplication>
//#define MDEBUGMODE
#include "isingmodel.h"
#include "optimization.h"
#include "solve_selfconsistent.h"
#include "montecarlo.h"
#include "isingspinconfig.h"
#include "train-isingmodel.h"

int main()
{
    //optimizeFreeEnergyBySGD();

    //solveSelfConsistentBySgd();

    //compareOptimizer();

    //solveSelfConsistentWithLoss();

    //magnetizationOfSpinConfiguration();

    //magnetizationOfSpinConfigurationHeatBath();

    //magnetizationOfSpinConfigurationHeatBathFixedSeed();

    //createIsingModelDataSet();

    predictMagnetization();

    return 0;
}
