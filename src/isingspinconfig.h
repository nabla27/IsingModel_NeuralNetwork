#ifndef ISINGSPINCONFIG_H
#define ISINGSPINCONFIG_H

#include "isingmodel.h"
#include "mathutil.h"
#include <fstream>
#include <iostream>



/* メトロポリス法によってイジングモデルのスピン配位をシミュレートする．
 * シミュレートされたスピン配位の平均磁化を求め，磁化の温度依存性を求める．
 */
void magnetizationOfSpinConfiguration()
{
    using StateType = State<20, 20, bool>;
    StateType state;

    IsingModel ising;
    MetropolisMethod<IsingModel, StateType,
                     &IsingModel::energy,
                     &IsingModel::randomAction,
                     &IsingModel::randomAccept> metropolis(&ising);

    double T = 0.0;
    const double Tc = ising.Tc();
    constexpr size_t updateCount = 1e5;

    std::ofstream fout;
    fout.open("isingspinconfig_" + std::to_string(updateCount) + ".csv");

    while(T < 4.0 * Tc)
    {
        state.initRand();
        ising.param.T = T;
        metropolis.optimize<updateCount>(state);

        const double t = T / Tc;
        fout << t << ',' << IsingModel::averageSpin(state) << '\n';
        std::cout << t << std::endl;

        T += 0.005;
    }

    fout.close();
}






/* 熱浴法法によってイジングモデルのスピン配位をシミュレートする．
 * シミュレートされたスピン配位の平均磁化を求め，磁化の温度依存性を求める．
 */
void magnetizationOfSpinConfigurationHeatBath()
{
    using StateType = State<20, 20, bool>;
    StateType state;

    IsingModel ising;
    IsingHeatBathMethod hbMethod(&ising);

    double T = 0.0;
    const double Tc = ising.Tc();
    constexpr size_t updateCount = 1e6;

    std::ofstream fout;
    fout.open("isingspinconfig_heat_" + std::to_string(updateCount) + ".csv");

    while(T < 4.0 * Tc)
    {
        state.initRand();
        ising.param.T = T;
        hbMethod.optimize<updateCount>(state);

        const double t = T / Tc;
        fout << t << ',' << IsingModel::averageSpin(state) << '\n';
        std::cout << t << std::endl;

        T += 0.005;
    }

    fout.close();
}



/* 熱浴法によってイジングモデルのスピン配位をシミュレートする．
 * シミュレートされたスピン配位の平均磁化を求め，磁化の温度依存性を求める．
 * スピン配位の初期状態は，up,downそれぞれに揃った状態から始める．
 * 各メソッドのシード値は固定する．つまり，各温度で初期状態や反転させるスピンの
 * 選び方などの更新方法を揃える．
 */
void magnetizationOfSpinConfigurationHeatBathFixedSeed()
{
    using StateType = State<20, 20, bool>;
    StateType state;
    IsingModel ising;
    IsingHeatBathMethod hbMethod(&ising);

    std::ofstream fout;
    fout.open("isingspinconfig_hb_fs.csv");

    double T = 0.0;
    const double Tc = ising.Tc();
    static constexpr size_t updateCount = 1e6;
    static constexpr unsigned int seed = 0;

    while(T < 4.0 * Tc)
    {
        ising.param.T = T;
        const double t = T / Tc;

        state.setSeed(seed);
        ising.setSeed(seed);
        hbMethod.setSeed(seed);

        state.init(true);
        hbMethod.optimize<updateCount>(state);

        fout << t << ',' << IsingModel::averageSpin(state);

        state.setSeed(seed);
        ising.setSeed(seed);
        hbMethod.setSeed(seed);

        state.init(false);
        hbMethod.optimize<updateCount>(state);

        fout << ',' << IsingModel::averageSpin(state) << '\n';
        std::cout << t << std::endl;

        T += 0.005;
    }
}

#endif // ISINGSPINCONFIG_H
