#ifndef ISINGMODEL_H
#define ISINGMODEL_H

#include "mathutil.h"
#include <iostream>
#include <math.h>
#include <random>


/* イジングモデル */
class IsingModel
{
public:
    struct Parameter
    {
        int N = 1;
        int z = 1;
        int J = 1;
        double T = 0.1;
        double kb = 1.0;
    } param;

    explicit IsingModel(const Parameter& param = Parameter())
        : param(param) {}

    double Tc() const noexcept { return param.z * param.J / param.kb; }
    double kbT() const noexcept { return param.kb * param.T; }

    /* 自由エネルギーを計算 */
    double freeEnergy(const double& m) const noexcept
    {
        MDEBUG(
            if(m > 1 || m < -1)
                    std::cout << "m is an invalid range: " << m << std::endl;
        )

        return - 0.5 * param.N * param.z * param.J * m * m
               - 0.5 * param.N * param.kb * param.T *
               (- (1 + m) * log(1 + m) - (1 - m) * log(1- m));
    }

    static double isingSpin(const bool& b) noexcept { return (b) ? 1 : -1; }

    /* スピン配位からエネルギーの計算 */
    template<size_t N, size_t M>
    double energy(const State<N, M, bool>& state) const noexcept
    {
        double energy = 0.0;

#if 0
        for(size_t r = 0; r < state.rows() - 1; ++r)
            for(size_t c = 0; c < state.cols() - 1; ++c)
                energy += - param.J * (static_cast<double>(state.at(r, c) == state.at(r, c + 1)) +
                                      static_cast<double>(state.at(r, c) == state.at(r + 1, c)));
#else
        for(size_t r = 0; r < state.rows() - 1; ++r)
            for(size_t c = 0; c < state.cols() - 1; ++c)
                energy += - param.J * (IsingModel::isingSpin(state.at(r, c)) *
                                       (IsingModel::isingSpin(state.at(r, c + 1)) + IsingModel::isingSpin(state.at(r + 1, c))));
#endif

        return energy;
    }

    /* スピン配位でランダムに1自由度を反転させる */
    template<size_t N, size_t M>
    void randomAction(State<N, M, bool>& state) const noexcept
    {
        static const std::uniform_int_distribution<> rowRand(0, static_cast<int>(state.rows() - 1));
        static const std::uniform_int_distribution<> colRand(0, static_cast<int>(state.cols() - 1));

        const size_t row = rowRand(mt);
        const size_t col = colRand(mt);

        const bool value = !state[row][col];

        state[row][col] = value;

        if      (row == 0) state[state.rows() - 1][col] = value;
        else if (row == state.rows() - 1) state[0][col] = value;
        if      (col == 0) state[row][state.cols() - 1] = value;
        else if (col == state.cols() - 1) state[row][0] = value;
    }

    /* 遷移後のエネルギーと遷移前のエネルギー差から遷移する確率を与える */
    bool randomAccept(const double& prevEnergy, const double& nextEnergy) const noexcept
    {
        static const std::uniform_real_distribution<> rand01(0.0, 1.0);

        return (
                    rand01(mt) <
                    exp(- (nextEnergy - prevEnergy) / kbT())
               );
    }

    /* スピン配位の磁化を計算 */
    template<size_t N, size_t M>
    static double magnetization(const State<N, M, bool>& state) noexcept
    {
        double Mag = 0;
        for(size_t n = 0; n < N; ++n)
            for(size_t m = 0; m < M; ++m)
                Mag += isingSpin(state.at(n, m));

        return Mag;
    }

    /* スピン配位の平均磁化を計算 */
    template<size_t N, size_t M>
    static double averageSpin(const State<N, M, bool>& state) noexcept
    {
        static constexpr size_t spinCount = N * M;

        static_assert(spinCount != 0, "division by zero");

        return magnetization(state) / spinCount;
    }

    static void setSeed(const unsigned int& seed) { mt.seed(seed); }

private:
    inline static std::random_device rnd = std::random_device();
    inline static std::mt19937 mt = std::mt19937(rnd());
};

#endif // ISINGMODEL_H
