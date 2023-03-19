#ifndef SOLVE_SELFCONSISTENT_H
#define SOLVE_SELFCONSISTENT_H

#include "isingmodel.h"
#include "mathutil.h"
#include <fstream>
#include <string>


/* 温度を固定したとき，イジングモデルの自由エネルギーが3つの勾配法
 * によって最適化される様子を見比べる．
 */
void compareOptimizer()
{
    IsingModel ising;

    ising.param.T = 0.8 * ising.Tc();

    const double lr = 0.01;
    const double alpha = 0.9;
    double v = 0.0;
    double h = 0.0;

    double m_1 = 1e-2;
    double m_2 = m_1;
    double m_3 = m_1;

    std::ofstream fout;
    fout.open("compare_optimizer.csv");

    for(int i = 0; i < 5000; ++i)
    {
        const double grad_1 = GradientDescent::gradient<IsingModel, &IsingModel::freeEnergy>(&ising, m_1);
        const double grad_2 = GradientDescent::gradient<IsingModel, &IsingModel::freeEnergy>(&ising, m_2);
        const double grad_3 = GradientDescent::gradient<IsingModel, &IsingModel::freeEnergy>(&ising, m_3);

        /* Sgd */
        m_1 -= lr * grad_1;
        /* Momentum */
        v = alpha * v - lr * grad_2;
        m_2 += v;
        /* AdaGrad */
        h += grad_3 * grad_3;
        m_3 -= lr * grad_3 / (sqrt(h) + 1e-7);

        fout << i << ','
             << m_1 << ','
             << m_2 << ','
             << m_3 << '\n';
    }
}


/* T = 0.8 * Tc の場合のSGDを用いた自由エネルギーの勾配降下法による最適化．
 * いくつかの学習率で自由エネルギーを最適化し，ステップ数と磁化(m_1,m_2,m_3)の様子を数値計算する．
 */
void optimizeFreeEnergyBySGD()
{
    IsingModel ising;

    std::ofstream fout;
    fout.open("sgd-optimization.csv");

    static constexpr int count = 2000;

    ising.param.T = 0.8 * ising.Tc();
    double m_1 = 1.0 - 1e-3;
    double m_2 = m_1;
    double m_3 = m_1;

    for(int i = 0; i < count; ++i)
    {
        m_1 -= 0.01 * GradientDescent::gradient<IsingModel, &IsingModel::freeEnergy>(&ising, m_1);
        m_2 -= 0.1 * GradientDescent::gradient<IsingModel, &IsingModel::freeEnergy>(&ising, m_2);
        m_3 -= 0.15 * GradientDescent::gradient<IsingModel, &IsingModel::freeEnergy>(&ising, m_3);

        fout << i << ','
             << m_1 << ','
             << m_2 << ','
             << m_3 << ','
             << '\n';
    }

    fout.close();
}


/* 磁化の温度依存性を3つの勾配法によって求める．
 * 目的関数を自由エネルギーとする．
 */
void solveSelfConsistentBySgd()
{
    IsingModel ising;

    const double initM = 1e-2; //磁化の初期値
    const double Tc = ising.Tc();    //転移点Tc
    static constexpr int maxCount = 3000;

    std::ofstream fout;
    fout.open("solve_selfconsistent_sgd" + std::to_string(maxCount) + ".csv");

    double T = 0.00; //温度T
    while(T < 3 * Tc)
    {
        ising.param.T = T;

        fout << T / Tc << ','
             << GradientDescent::sgd<IsingModel, &IsingModel::freeEnergy, maxCount>(&ising, initM) << ','
             << GradientDescent::momentum<IsingModel, &IsingModel::freeEnergy, maxCount>(&ising, initM) << ','
             << GradientDescent::adagrad<IsingModel, &IsingModel::freeEnergy, maxCount>(&ising, initM) << std::endl;

        T += 0.002;
    }

    fout.close();
}


/* 磁化の温度依存性を3つの勾配法によって求める．
 * 目的関数を自己無撞着方程式を移項した式とする．
 */
void solveSelfConsistentWithLoss()
{
    struct IsingSelfConsistent
    {
        IsingSelfConsistent(IsingModel* ising)
            : ising(ising) {}

        double loss(const double& m) const
        {
            const double diff = m - tanh(ising->Tc() / ising->param.T * m);
            return diff * diff;
        }

    private:
        IsingModel *ising; //this has no ownership
    };

    IsingModel ising;
    IsingSelfConsistent selfConsistent(&ising);

    const double Tc = ising.Tc();
    const double maxT = 3 * Tc;
    static constexpr double initM = 0.5 + 1e-2;
    static constexpr int maxCount = 10000;

    std::ofstream fout;
    fout.open("solve_selfconsistent_loss" + std::to_string(maxCount) + ".csv");

    double T = 0.0;
    while(T < maxT)
    {
        ising.param.T = T;

        fout << T / Tc << ','
             << GradientDescent::sgd<IsingSelfConsistent, &IsingSelfConsistent::loss, maxCount>(&selfConsistent, initM) << ','
             << GradientDescent::momentum<IsingSelfConsistent, &IsingSelfConsistent::loss, maxCount>(&selfConsistent, initM) << ','
             << GradientDescent::adagrad<IsingSelfConsistent, &IsingSelfConsistent::loss, maxCount>(&selfConsistent, initM) << '\n';

        T += 0.002;
    }

    fout.close();
}

#endif // SOLVE_SELFCONSISTENT_H
