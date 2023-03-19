#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <iostream>
#include <random>
#include <cmath>

#ifdef MDEBUGMODE
#define MDEBUG(statement) do { statement } while(false);
#else
#define MDEBUG(statement) ;
#endif //ifdef MDEBUGMODE

/* スピン配位などを表すクラス */
template<size_t N, size_t M, typename T = double>
class State
{
public:

    static_assert(N != 0 && M != 0, "invalid size");

    State() {}
    State(const T& init)
    {
        for(size_t n = 0; n < N; ++n)
            for(size_t m = 0; m < M; ++m)
                _elements[n][m] = init;
    }
    State(const State& other)
    {
        for(size_t n = 0; n < N; ++n)
            for(size_t m = 0; m < M; ++m)
                _elements[n][m] = other._elements[n][m];
    }

    State& operator=(const State& other) noexcept
    {
        for(size_t n = 0; n < N; ++n)
            for(size_t m = 0; m < M; ++m)
                _elements[n][m] = other._elements[n][m];

        return *this;
    }

    T *const operator[](const size_t& index) noexcept
    {
        return _elements[index];
    }

    T at(const size_t& row, const size_t& col) const noexcept
    {
        return _elements[row][col];
    }

    template<size_t row, size_t col>
    T at() const noexcept
    {
        static_assert(row < N && col < M, "out of range.");

        return _elements[row][col];
    }

    void print() const
    {
        std::cout << "----- " << N << "," << M << "-----" << std::endl;
        for(size_t r = 0; r < N; ++r)
        {
            for(size_t c = 0; c < M; ++c)
            {
                std::cout << _elements[r][c] << " ";
            }
            std::cout << std::endl;
        }
    }

    void initRand(const T& min = T(), const T& max = T()) noexcept
    {
        for(size_t n = 0; n < N; ++n)
            for(size_t m = 0; m < M; ++m)
            {
                if constexpr (std::is_same_v<T, bool>)
                {
                    static const std::uniform_int_distribution<> bitRand(0, 1);
                    _elements[n][m] = bitRand(mt) % 2 == 0;
                }
                else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
                {
                    static const std::uniform_real_distribution<> random(min, max);
                    _elements[n][m] = random(mt);
                }
                else
                {
                    _elements[n][m] = T::random(min, max);
                }
            }
    }

    void init(const T& value = T()) noexcept
    {
        for(size_t n = 0; n < N; ++n)
            for(size_t m = 0; m < M; ++m)
            {
                _elements[n][m] = value;
            }
    }

    template<typename U = T>
    U sum() const noexcept
    {
        U sum = U();
        for(size_t n = 0; n < N; ++n)
            for(size_t m = 0; m < N; ++m)
            {
                sum += _elements[n][m];
            }

        return sum;
    }

    static constexpr size_t rows() noexcept { return N; }
    static constexpr size_t cols() noexcept { return M; }

    static void setSeed(const unsigned int& seed) { mt.seed(seed); }

    template<typename U>
    void createVector1d(std::vector<U>& vec)
    {
        vec.resize(N * M);

        for(size_t i = 0; i < N; ++i)
            for(size_t j = 0; j < M; ++j)
                vec[i * M + j] = static_cast<U>(_elements[i][j]);
    }

private:
    T _elements[N][M];
    inline static std::random_device rnd = std::random_device();
    inline static std::mt19937 mt = std::mt19937(rnd());
};

template<size_t N, size_t M, typename T>
using StateType =  State<N, M, T>;







namespace GradientDescent
{


/* 中心差分によって関数の微分を求める */
template <typename T, double(T::*func)(const double&)const>
double gradient(const T *const& instance, const double& x)
{
    static constexpr double h = 1e-7;
    static constexpr double c = 1.0 / (2.0 * h);

    return ((instance->*func)(x + h) - (instance->*func)(x - h)) * c;
}

/* 勾配法 SGD */
template <typename T, double(T::*func)(const double&)const, int N = 200>
double sgd(const T*const& instance, double x, const double& lr = 0.01)
{
    int count = 0;
    while(count < N)
    {
        const double grad = gradient<T, func>(instance, x);

        static constexpr double eps = 1e-7;
        if(fabs(grad) < eps) break;

        x -= lr * grad;
        count++;
    }

    MDEBUG(
        std::cout << "sgd updated count: " << count << std::endl;
    )

    return x;
}

/* 勾配法 Momentum */
template <typename T, double(T::*func)(const double&)const, int N = 200>
double momentum(const T*const& instance,
                double x,
                const double& alpha = 0.9,
                const double& lr = 0.01)
{
    double v = 0.0;

    int count = 0;
    while(count < N)
    {
        const double grad = gradient<T, func>(instance, x);

        static constexpr double eps = 1e-7;
        if(fabs(grad) < eps) break;

        v = alpha * v - lr * grad;
        x += v;
        count++;
    }

    MDEBUG(
        std::cout << "momentum updated count :" << count << std::endl;
    )

    return x;
}

/* 勾配法 AdaGrad */
template <typename T, double(T::*func)(const double&)const, int N = 200>
double adagrad(const T *const& instance,
               double x,
               const double& lr = 0.01)
{
    double h = 0.0;

    int count = 0;
    while(count < N)
    {
        const double grad = gradient<T, func>(instance, x);

        static constexpr double eps = 1e-7;
        if(fabs(grad) < eps) break;

        h += grad * grad;

        x -= lr *grad / (sqrt(h) + 1e-7);
        count++;
    }

    MDEBUG(
        std::cout << "AdaGrad updated count:" << count << std::endl;
    )

    return x;
}

}













/* メトロポリス法 */
template<typename ObjType, typename StateType,
         double(ObjType::*fEnergy)(const StateType&)const,
         void(ObjType::*fAction)(StateType&)const,
         bool(ObjType::*fRandAccept)(const double& prev, const double& next)const>
class MetropolisMethod
{
public:
    MetropolisMethod(ObjType* obj) : obj(obj) {}

public:
    void update(StateType& state)
    {
        const auto prevEnergy = (obj->*fEnergy)(state);

        StateType nextState = state;
        (obj->*fAction)(nextState);

        const auto nextEnergy = (obj->*fEnergy)(nextState);

        if(nextEnergy < prevEnergy)
        {
            //accept
            state = nextState;
        }
        else
        {
            if((obj->*fRandAccept)(prevEnergy, nextEnergy))
            {
                //accept
                state = nextState;
            }
            else
            {
                //reject
            }
        }
    }

    template<size_t stepCount = 50000>
    void optimize(StateType& state)
    {
        for(size_t i = 0; i < stepCount; ++i)
            update(state);
    }

private:
    ObjType *obj;
};












#include "mathutil.h"
#include "isingmodel.h"
#include <iostream>


enum class LatticeType { Square, Triangle, Rhombus, Hexagonal };


/* 熱浴法 */

template<LatticeType = LatticeType::Square>
class IsingHeatBathMethod
{
public:
    IsingHeatBathMethod(IsingModel *ising) : ising(ising) {}

    template<size_t N, size_t M>
    void update(State<N, M, bool>& state) noexcept
    {
        static const std::uniform_real_distribution<> rand01(0, 1);
        static const std::uniform_int_distribution<> randRow(0, N - 1);
        static const std::uniform_int_distribution<> randCol(0, M - 1);

        //ランダムに1サイトを選択
        const int row = randRow(mt);
        const int col = randCol(mt);

        //最近接のイジングスピンの和
        const double spin = neighborSpin(state, row, col);

        //選択したサイトの遷移状態
        const bool value = (rand01(mt) < 0.5 * (std::tanh(ising->param.J / ising->kbT() * spin) + 1.0));

        //遷移させる
        state[row][col] = value;

        //周期境界条件
             if(row == 0) state[N - 1][col] = value;
        else if(row == N - 1) state[0][col] = value;
             if(col == 0) state[row][M - 1] = value;
        else if(col == M - 1) state[row][0] = value;
    }

    template<size_t stepCount, size_t N, size_t M>
    void optimize(State<N, M, bool>& state) noexcept
    {
        for(size_t i = 0; i < stepCount; ++i) update(state);
    }

    template<size_t N, size_t M>
    double neighborSpin(const State<N, M, bool>& state, const size_t& row, const size_t& col);

    static void setSeed(const unsigned int& seed) { mt.seed(seed); }

private:
    IsingModel *ising; //this has no ownership

    inline static std::random_device rnd = std::random_device();
    inline static std::mt19937 mt = std::mt19937(rnd());
};


template<>
template<size_t N, size_t M>
double IsingHeatBathMethod<LatticeType::Square>::neighborSpin(const State<N, M, bool>& state,
                                                                 const size_t& row,
                                                                 const size_t& col)
{
    double spin = 0.0;
    if constexpr(N > 2 && M > 2)
    {
        const int up = (row == 0) ? N - 2 : row - 1;
        const int down = (row == N - 1) ? 1 : row + 1;
        const int left = (col == 0) ? M - 2 : col - 1;
        const int right = (col == M - 1) ? 1 : col + 1;

        spin = IsingModel::isingSpin(state.at(up, col)) +
               IsingModel::isingSpin(state.at(down, col)) +
               IsingModel::isingSpin(state.at(row, left)) +
               IsingModel::isingSpin(state.at(row, right));
    }
    else
    {
        if constexpr(N > 2)
        {
            const int up = (row == 0) ? N - 2: row - 1;
            const int down = (row == N - 1) ? 1 : row + 1;

            spin += IsingModel::isingSpin(state.at(up, col)) +
                    IsingModel::isingSpin(state.at(down, col));
        }
        if constexpr(M > 2)
        {
            const int left = (col == 0) ? M - 2 : col - 1;
            const int right = (col == M - 1) ? 1 : col + 1;

            spin += IsingModel::isingSpin(state.at(row, left)) +
                    IsingModel::isingSpin(state.at(row, right));
        }
    }

    return spin;
}

template<>
template<size_t N, size_t M>
double IsingHeatBathMethod<LatticeType::Triangle>::neighborSpin(const State<N, M, bool>& state,
                                                                const size_t& row,
                                                                const size_t& col)
{
    double spin = 0.0;
    if constexpr(N > 2 && M > 2)
    {
        const int up = (row == 0) ? N - 2 : row - 1;
        const int down = (row == N - 1) ? 1 : row + 1;
        const int left = (col == 0) ? M - 2 : col - 1;
        const int right = (col == M - 1) ? 1 : col + 1;

        if(row % 2 == 0)
        {
            spin = IsingModel::isingSpin(state.at(up, col)) +
                   IsingModel::isingSpin(state.at(up, right)) +
                   IsingModel::isingSpin(state.at(row, left)) +
                   IsingModel::isingSpin(state.at(row, right)) +
                   IsingModel::isingSpin(state.at(down, col)) +
                   IsingModel::isingSpin(state.at(down, right));
        }
        else
        {
            spin = IsingModel::isingSpin(state.at(up, left)) +
                   IsingModel::isingSpin(state.at(up, col)) +
                   IsingModel::isingSpin(state.at(row, left)) +
                   IsingModel::isingSpin(state.at(row, right)) +
                   IsingModel::isingSpin(state.at(down, left)) +
                   IsingModel::isingSpin(state.at(down, col));
        }
    }
    else
    {
        if constexpr(N > 2)
        {
            const int up = (row == 0) ? N - 2: row - 1;
            const int down = (row == N - 1) ? 1 : row + 1;

            spin += IsingModel::isingSpin(state.at(up, col)) +
                    IsingModel::isingSpin(state.at(down, col));
        }
        if constexpr(M > 2)
        {
            const int left = (col == 0) ? M - 2 : col - 1;
            const int right = (col == M - 1) ? 1 : col + 1;

            spin += IsingModel::isingSpin(state.at(row, left)) +
                    IsingModel::isingSpin(state.at(row, right));
        }
    }

    return spin;
}

template<>
template<size_t N, size_t M>
double IsingHeatBathMethod<LatticeType::Rhombus>::neighborSpin(const State<N, M, bool>& state,
                                                               const size_t& row,
                                                               const size_t& col)
{
    double spin = 0.0;
    if constexpr(N > 2 && M > 2)
    {
        const int up = (row == 0) ? N - 2 : row - 1;
        const int down = (row == N - 1) ? 1 : row + 1;
        const int left = (col == 0) ? M - 2 : col - 1;
        const int right = (col == M - 1) ? 1 : col + 1;

        if(row % 2 == 0)
        {
            spin = IsingModel::isingSpin(state.at(up, col)) +
                   IsingModel::isingSpin(state.at(up, right)) +
                   IsingModel::isingSpin(state.at(down, col)) +
                   IsingModel::isingSpin(state.at(down, right));
        }
        else
        {
            spin = IsingModel::isingSpin(state.at(up, left)) +
                   IsingModel::isingSpin(state.at(up, col)) +
                   IsingModel::isingSpin(state.at(down, left)) +
                   IsingModel::isingSpin(state.at(down, col));
        }
    }
    else
    {
        if constexpr(N > 2)
        {
            const int up = (row == 0) ? N - 2: row - 1;
            const int down = (row == N - 1) ? 1 : row + 1;

            spin += IsingModel::isingSpin(state.at(up, col)) +
                    IsingModel::isingSpin(state.at(down, col));
        }
        if constexpr(M > 2)
        {
            const int left = (col == 0) ? M - 2 : col - 1;
            const int right = (col == M - 1) ? 1 : col + 1;

            spin += IsingModel::isingSpin(state.at(row, left)) +
                    IsingModel::isingSpin(state.at(row, right));
        }
    }

    return spin;
}

template<>
template<size_t N, size_t M>
double IsingHeatBathMethod<LatticeType::Hexagonal>::neighborSpin(const State<N, M, bool>& state,
                                                               const size_t& row,
                                                               const size_t& col)
{
    double spin = 0.0;
    if constexpr(N > 2 && M > 2)
    {
        const int up = (row == 0) ? N - 2 : row - 1;
        const int down = (row == N - 1) ? 1 : row + 1;
        const int left = (col == 0) ? M - 2 : col - 1;
        const int right = (col == M - 1) ? 1 : col + 1;

        if(row % 4 == 0)
        {
            spin = IsingModel::isingSpin(state.at(up, col)) +
                   IsingModel::isingSpin(state.at(down, col)) +
                   IsingModel::isingSpin(state.at(down, right));
        }
        else if(row % 4 == 1)
        {
            spin = IsingModel::isingSpin(state.at(up, right)) +
                   IsingModel::isingSpin(state.at(up, col)) +
                   IsingModel::isingSpin(state.at(down, col));
        }
        else if(row % 4 == 2)
        {
            spin = IsingModel::isingSpin(state.at(up, col)) +
                   IsingModel::isingSpin(state.at(down, left)) +
                   IsingModel::isingSpin(state.at(down, col));
        }
        else
        {
            spin = IsingModel::isingSpin(state.at(up, col)) +
                   IsingModel::isingSpin(state.at(up, right)) +
                   IsingModel::isingSpin(state.at(down, col));
        }
    }
    else
    {
        if constexpr(N > 2)
        {
            const int up = (row == 0) ? N - 2: row - 1;
            const int down = (row == N - 1) ? 1 : row + 1;

            spin += IsingModel::isingSpin(state.at(up, col)) +
                    IsingModel::isingSpin(state.at(down, col));
        }
        if constexpr(M > 2)
        {
            const int left = (col == 0) ? M - 2 : col - 1;
            const int right = (col == M - 1) ? 1 : col + 1;

            spin += IsingModel::isingSpin(state.at(row, left)) +
                    IsingModel::isingSpin(state.at(row, right));
        }
    }

    return spin;
}




#endif // MATHUTIL_H
